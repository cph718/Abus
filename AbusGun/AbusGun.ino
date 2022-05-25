/*********
Abus.c 
Christian H
*********/

/* Included libraries */
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Servo.h>

/* User defines */
//Motor Pins
#define m1Pin         22
#define m2Pin         23
#define MOTDEFAULT    1000

/* User Type defines */
struct Motor_t {
  Servo motorServo;
  int Pin;
  int goalSpeed;
  int curSpeed;
  int Ramp;
};

/* Global Variables */
// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "Internat";
const char* password = "p1kachu1";

// replace staticIP and gateway address based on your home router settings
IPAddress staticIP(192, 168, 1, 200);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);

const char* PARAM_SPEED = "speedval";
const char* PARAM_RAMP = "rampval";
const char* PARAM_STEPSPEED = "stepspeedval";
const char* PARAM_STEPREVS = "steprevval";

Motor_t rightMotor;
Motor_t leftMotor;

int gInitFlag = 0;
int gStepFlag = 0;
int gStepDelay = 0;

AsyncWebServer server(80);

AccelStepper stepper = AccelStepper(motorInterfaceType, StepPin, DirPin);

/*****************************************************************************************************************************/

void setup() {  
//  // Initialise Motor Pins and start values
  rightMotor.Pin = m1Pin;
  leftMotor.Pin = m2Pin;

  //Set Stepper drive clockwise
  digitalWrite(DirPin, HIGH);
  stepper.setMaxSpeed(5000);
  stepper.setAcceleration(30);

  Serial.begin(115200);

    // Initialize SPIFFS
  if(!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  WiFi.mode(WIFI_STA);
  WiFi.config(staticIP, gateway, subnet);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) 
  {
    Serial.println("WiFi Failed!");
    return;
  }
  
  Serial.println();
  Serial.print("ESP IP Address: http://");
  Serial.println(WiFi.localIP());
  
  // Send web page to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/index.html", String(), false);
  });

    // Send phone page to client
  server.on("/phone", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/phone.html", String(), false);
  });

      // Receive an HTTP GET request
  server.on("/init", HTTP_GET, [] (AsyncWebServerRequest *request) 
  {
    Serial.println("Initialising Motor Control");
    gInitFlag = 1;
    
    request->send(200, "text/plain", "ok");
  });

  // Receive an HTTP GET request
  server.on("/start", HTTP_GET, [] (AsyncWebServerRequest *request) 
  {
    Serial.println("Start");
    String speedValueS, rampValueS;
    int speedValue, rampValue;
    speedValueS = request->getParam(PARAM_SPEED)->value();
    speedValue = speedValueS.toInt();
    rampValueS = request->getParam(PARAM_RAMP)->value();
    rampValue = rampValueS.toInt();
    
    if(speedValue <= 1000 && speedValue >= 0)
    {
      rightMotor.goalSpeed = speedValue;
      leftMotor.goalSpeed = speedValue;
      Serial.print(" Goal speed set:");
      Serial.println(speedValue);
    }
    
    if(rampValue <= 1000 && rampValue >= 0)
    {
      rightMotor.Ramp = rampValue;
      leftMotor.Ramp = rampValue;
      Serial.print("Ramp set:");
      Serial.println(rampValue);
    }
    
    request->send(200, "text/plain", "ok");
  });

    // Receive an HTTP GET request
  server.on("/stop", HTTP_GET, [] (AsyncWebServerRequest *request) 
  {
    Serial.println("Stop");
    rightMotor.goalSpeed = 0;
    leftMotor.goalSpeed = 0;
    rightMotor.Ramp = 50;
    leftMotor.Ramp = 50;
    request->send(200, "text/plain", "ok");
  });

  server.on("/updateR", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", String(rightMotor.curSpeed));
  });

    server.on("/updateL", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", String(leftMotor.curSpeed));
  });

     server.on("/step", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("Step");

    String stepSpeed, stepRevs;
    int stepSpeedValue, stepRevsValue;
    stepSpeed = request->getParam(PARAM_STEPSPEED)->value();
    stepSpeedValue = stepSpeed.toInt();
    stepRevs = request->getParam(PARAM_STEPREVS)->value();
    stepRevsValue = stepRevs.toInt();

    Serial.print("Step Speed:");
    Serial.println(stepSpeedValue);
    
    stepper.setSpeed(stepSpeedValue);
    
    
//    gStepDelay = stepSpeedValue; 
//    gStepFlag = 1;

    request->send(200, "text/plain", "ok");
  });
  
 // server.onNotFound(handleNotFound);
  server.begin();
}

//TODO Add a way of reporting the current set speed
/*****************************************************************************************************************************/
void loop() 
{
//  stepper.runSpeed();
//  if(gStepFlag == 1)
//  {
//      // Spin 1 revolution
//    for(int x = 0; x < StepPerRev; x++)
//    {
//      digitalWrite(StepPin, HIGH);
//     delayMicroseconds(gStepDelay);
//      digitalWrite(StepPin, LOW);
//      delayMicroseconds(gStepDelay);
//    }
//    delay(100); // Wait 100ms
//  
//    gStepFlag = 0;
//  }
  
  if(gInitFlag == 1)
  {
    rightMotor.goalSpeed = 0;
    leftMotor.goalSpeed = 0;
    rightMotor.curSpeed = 0;
    leftMotor.curSpeed = 0;
    rightMotor.Ramp = 0;
    leftMotor.Ramp = 0;
    
    rightMotor.motorServo.attach(rightMotor.Pin);
    leftMotor.motorServo.attach(leftMotor.Pin);
    
    rightMotor.motorServo.writeMicroseconds(MOTDEFAULT);
    leftMotor.motorServo.writeMicroseconds(MOTDEFAULT);

    delay(500);
    
    gInitFlag = 2;
  }

  if(gInitFlag == 2)
  {
    rightMotor.curSpeed = CalculateMotorSpeed(rightMotor.goalSpeed, rightMotor.curSpeed, rightMotor.Ramp);
    rightMotor.motorServo.writeMicroseconds(rightMotor.curSpeed + MOTDEFAULT);
    Serial.print("Right Motor Current Speed:");
    Serial.println(rightMotor.curSpeed);
    
    leftMotor.curSpeed = CalculateMotorSpeed(leftMotor.goalSpeed, leftMotor.curSpeed, leftMotor.Ramp);
    leftMotor.motorServo.writeMicroseconds(leftMotor.curSpeed + MOTDEFAULT);
    Serial.print("Left Motor Current Speed:");
    Serial.println(leftMotor.curSpeed);
    delay(200);
  }
}

int CalculateMotorSpeed(int goalSpeed, int curSpeed, int ramp)
{
  if(goalSpeed > 1000 || goalSpeed < 0)
  {
    return MOTDEFAULT;
  }
  
  if(curSpeed == goalSpeed)
  {
    return (goalSpeed);
  } 
  else if((curSpeed + ramp > goalSpeed) && (curSpeed - ramp < goalSpeed))
  {
    Serial.println("test");
    return (goalSpeed);
  }
  else if(curSpeed < goalSpeed)
  {
    return (curSpeed + ramp);
  }
  else if(curSpeed > goalSpeed)
  {
    return (curSpeed - ramp);
  }
  
}
