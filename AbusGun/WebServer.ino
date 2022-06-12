#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

const char* PARAM_SPEED = "speedval";
const char* PARAM_RAMP = "rampval";
const char* PARAM_STEPSPEED = "stepspeedval";
const char* PARAM_STEPREVS = "steprevval";

/* Global Variables */
// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "Internat";
const char* password = "p1kachu1";

// replace staticIP and gateway address based on your home router settings
IPAddress staticIP(192, 168, 1, 200);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);

AsyncWebServer server(80);

void WebServer_Init()
{
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
    //Launcher_SetInitFlag();
    
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
      Launcher_SetRightGoalSpeed(speedValue);
      Launcher_SetLeftGoalSpeed(speedValue);
      Serial.print(" Goal speed set:");
      Serial.println(speedValue);
    }
    
    if(rampValue <= 1000 && rampValue >= 0)
    {
      Launcher_SetRightRamp(rampValue);
      Launcher_SetLeftRamp(rampValue);
      Serial.print("Ramp set:");
      Serial.println(rampValue);
    }
    
    request->send(200, "text/plain", "ok");
  });

    // Receive an HTTP GET request
  server.on("/stop", HTTP_GET, [] (AsyncWebServerRequest *request) 
  {
    Serial.println("Stop");
    Launcher_SetRightGoalSpeed(0);
    Launcher_SetLeftGoalSpeed(0);
    Launcher_SetRightRamp(50);
    Launcher_SetLeftRamp(50);
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
    
    Reload_StepperSetMoveTo(stepSpeedValue);

    request->send(200, "text/plain", "ok");
  });
  
 // server.onNotFound(handleNotFound);
  server.begin();

}
