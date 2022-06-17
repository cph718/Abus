/*********
ReloadStepper.c 
Christian H
*********/

#include <AccelStepper.h>

#define StepPerRev    3200
#define motorInterfaceType 1

enum StepperPos{unknown, FindingHome, Load, Ready, Fire};

int gHomePositionFlag = 0;
AccelStepper stepper = AccelStepper(motorInterfaceType, STEPPER_STEPPIN, STEPPER_DIRPIN);

//Global Functions
void Reload_InitStepper()
{
  //Set Stepper drive anti-clockwise
  Reload_StepperDirIn();
  stepper.setMaxSpeed(150000);
  stepper.setAcceleration(300000);
  Serial.println("Stepper Initialised");
}

void Reload_UpdateStepperStateMachine()
{
  static StepperPos currPos = unknown;
  
  if(currPos == unknown)
  {
    Reload_FindHome();
    Serial.println("Stepper looking For home");
    currPos = FindingHome;
  } else if(currPos == FindingHome)
  {
    if(gHomePositionFlag == 1)
    {
      //Stop homing movement, set home position and start Standard running operation
      stepper.setCurrentPosition(0);     
      Serial.println("Stepper home position found");
      RTOS_InitTask1();
      Reload_MoveToPos1();
      currPos = Load;
    }
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
  } else
  {
    Reload_DisableHomingMovement();
    gHomePositionFlag = 0;
    currPos = unknown;
  }
}

void Reload_SetHomePositionFlag()  //Sets the current position as home
{
  gHomePositionFlag = 1;
}

void Reload_DisableHomingMovement()  //Disables and resets all movement
{
  RTOS_StopActiveTasks();
  Reload_StopContinuousStepper();
  Reload_StepperStopMoveTo();
}

//Local Functions
void Reload_FindHome()
{
  Reload_StepperDirIn(); //Set Direction to IN
  Reload_SetStepperContinuousSpeed(5000); //Set a slow speed to move towards limit switch
  RTOS_InitTask2();      //Enable continous homing movement
}

void Reload_MoveToPos1()  //Position1 is were the ball can drop down
{
  Reload_StepperSetMoveTo(-2000);
  Serial.println("Stepper Position1");
}

void Reload_MoveToPos2()  //Position1 is were the ball is ready to be launched
{
  Reload_StepperSetMoveTo(-9000);
  Serial.println("Stepper Position2");
}

void Reload_MoveToPos3()  //Position3 is were the ball is ready to be launched
{
  Reload_StepperSetMoveTo(-12800);
  Serial.println("Stepper Position3");
}

//Positional Movement
void Reload_StepperSetMoveTo(int PosInSteps)
{
  stepper.moveTo(PosInSteps);
}

void Reload_StepperStopMoveTo()
{
  stepper.stop();
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
  digitalWrite(STEPPER_DIRPIN, HIGH);
}

void Reload_StepperDirOut()
{
  digitalWrite(STEPPER_DIRPIN, LOW);
}
