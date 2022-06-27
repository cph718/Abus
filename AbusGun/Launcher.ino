/* Included libraries */
#include <Servo.h>

/* User defines */
#define MOTDEFAULT    1000

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

void Launcher_Init()
{
  rightMotor.goalSpeed = 0;
  leftMotor.goalSpeed = 0;
  rightMotor.curSpeed = 0;
  leftMotor.curSpeed = 0;
  rightMotor.Ramp = 10;
  leftMotor.Ramp = 10;

  rightMotor.Pin = ESC1_CTRLPIN;
  leftMotor.Pin = ESC2_CTRLPIN;
  
  rightMotor.motorServo.attach(rightMotor.Pin);
  leftMotor.motorServo.attach(leftMotor.Pin);
  
  rightMotor.motorServo.writeMicroseconds(MOTDEFAULT);
  leftMotor.motorServo.writeMicroseconds(MOTDEFAULT);

  delay(500);
}

void Launcher_UpdateMotors()
{
  rightMotor.curSpeed = Reload_CalculateMotorSpeed(rightMotor.goalSpeed, rightMotor.curSpeed, rightMotor.Ramp);
  rightMotor.motorServo.writeMicroseconds(rightMotor.curSpeed + MOTDEFAULT);
  //Serial.print("Right Motor Current Speed:");
  //Serial.println(rightMotor.curSpeed);
    
  leftMotor.curSpeed = Reload_CalculateMotorSpeed(leftMotor.goalSpeed, leftMotor.curSpeed, leftMotor.Ramp);
  leftMotor.motorServo.writeMicroseconds(leftMotor.curSpeed + MOTDEFAULT);
  //Serial.print("Left Motor Current Speed:");
  //Serial.println(leftMotor.curSpeed);
  delay(200);
}

void Launcher_SetPotSpeed()
{
  int potSpeed = map(analogRead(LAUNCHER_SPEEDPIN), 0, 4095, 0, 1000);

  Serial.println("Calculated Speed is:");
  Serial.println(potSpeed);
  
  Launcher_SetRightGoalSpeed(potSpeed);
  Launcher_SetLeftGoalSpeed(potSpeed);
}

void Launcher_SetOffSpeed()
{
  Launcher_SetRightGoalSpeed(0);
  Launcher_SetLeftGoalSpeed(0);
}

int Reload_CalculateMotorSpeed(int goalSpeed, int curSpeed, int ramp)
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

//Speed is a value 0 to 1000, were 1000 is max speed
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
