/*********
ReloadStepper.c 
Christian H
*********/

#include <AccelStepper.h>

#define StepPin       18
#define DirPin        19
#define StepPerRev    200
#define motorInterfaceType 1

enum StepperPos{unknown, Home, Load, Ready, Fire};

AccelStepper stepper = AccelStepper(motorInterfaceType, StepPin, DirPin);

int gHomePosition = 0;

void Reload_InitStepper()
{
  //Set Stepper drive anti-clockwise
  digitalWrite(DirPin, LOW);
  stepper.setMaxSpeed(650);
  stepper.setAcceleration(1500);
  Serial.println("Stepper Initialised");
}

void Reload_SetHomePosition()  //Sets the current position as home
{
  gHomePosition = stepper.currentPosition();
}

void Reload_FindHome()
{
  Reload_StepperDirIn(); //Set Direction to IN
  Reload_SetStepperContinuousSpeed(100); //Set a slow speed to move towards limit switch
  while(gHomePosition == 0) //Move until home position set by
  {
    Reload_NextStepperContinuousStep();
  }
  Reload_StopContinuousStepper();
  Serial.print("Home set to:");
  Serial.println(gHomePosition);
  
  RTOS_InitTask1(); //Start positional running
}

void Reload_MoveToNextPos()
{
  static StepperPos currPos = unknown;

  if(currPos == unknown)
  {
    Reload_FindHome();
    currPos = Home;
  } else if(currPos == Home)
  {
    Reload_MoveToPos1();
    currPos = Load;
  } else if(currPos == Load)
  {
    Reload_MoveToPos2();
    currPos = Ready;
  } else if(currPos == Ready)
  {
    Reload_MoveToPos3();
    currPos = Fire;
  } else if(currPos == Fire)
  {
    Reload_MoveToPos1();
    currPos = Load;
  }
}

void Reload_MoveToPos1()  //Position1 is were the ball can drop down
{
  Reload_StepperSetMoveTo(gHomePosition + 200);
  Serial.println("Stepper Position1");
}

void Reload_MoveToPos2()  //Position1 is were the ball is ready to be launched
{
  Reload_StepperSetMoveTo(gHomePosition + 400);
  Serial.println("Stepper Position2");
}

void Reload_MoveToPos3()  //Position3 is were the ball is ready to be launched
{
  Reload_StepperSetMoveTo(gHomePosition + 600);
  Serial.println("Stepper Position3");
}

//Positional Movement
void Reload_StepperSetMoveTo(int PosInSteps)
{
  stepper.moveTo(PosInSteps);
}

void Reload_StepperRunMoveTo()
{
  stepper.run();
}


//Continous Running
void Reload_SetStepperContinuousSpeed(int Speed)
{
  stepper.setSpeed(Speed);
}

void Reload_StopContinuousStepper()
{
  stepper.setSpeed(0);
}

void Reload_NextStepperContinuousStep()
{
  stepper.runSpeed();
}

//Direction
void Reload_StepperDirIn()
{
  digitalWrite(DirPin, LOW);
}

void Reload_StepperDirOut()
{
  digitalWrite(DirPin, HIGH);
}
