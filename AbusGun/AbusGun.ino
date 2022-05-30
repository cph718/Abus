/*********
Abus.c 
Christian H
*********/

/*
 * TODO
 */

/* Included libraries */
#include <Servo.h>

/* User defines */
//Motor Pins
#define m1Pin         22
#define m2Pin         23
#define MOTDEFAULT    1000

#define LimitSwitch   4

/* User Type defines */
struct Motor_t {
  Servo motorServo;
  int Pin;
  int goalSpeed;
  int curSpeed;
  int Ramp;
};

Motor_t rightMotor;
Motor_t leftMotor;

int gInitFlag = 0;

/*****************************************************************************************************************************/

void setup() 
{
  Serial.begin(115200);  

  Serial.println("XTAL Freq:");
  Serial.println(getXtalFrequencyMhz());
  Serial.println("CPU Freq:");
  Serial.println(getCpuFrequencyMhz());
  
  // Initialise Motor Pins and start values
  rightMotor.Pin = m1Pin;
  leftMotor.Pin = m2Pin;

  Reload_InitStepper();

  pinMode(LimitSwitch, INPUT_PULLDOWN);

  WebServer_Init();
}

//TODO Add a way of reporting the current set speed
/*****************************************************************************************************************************/
void loop() 
{
  
  if(!digitalRead(LimitSwitch))
  {
    Reload_StopContinuousStepper();
  }
  
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

void Launcher_SetInitFlag()
{
  gInitFlag = 1;
}

void Launcher_SetRightGoalSpeed(int rSpeed)
{
  rightMotor.goalSpeed = rSpeed;
}

void Launcher_SetLeftGoalSpeed(int lSpeed)
{
  leftMotor.goalSpeed = lSpeed;
}

void Launcher_SetRightRamp(int rRamp)
{
  rightMotor.Ramp = rRamp;
}

void Launcher_SetLeftRamp(int lRamp)
{
  leftMotor.Ramp = lRamp;
}
