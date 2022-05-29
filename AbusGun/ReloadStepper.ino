/*********
ReloadStepper.c 
Christian H
*********/

#include <AccelStepper.h>

#define StepPin       18
#define DirPin        19
#define StepPerRev    200
#define motorInterfaceType 1

AccelStepper stepper = AccelStepper(motorInterfaceType, StepPin, DirPin);


void Reload_InitStepper()
{
  //Set Stepper drive clockwise
  digitalWrite(DirPin, HIGH);
  stepper.setMaxSpeed(650);
  stepper.setAcceleration(1500);

  RTOS_InitTask1();
  
  Serial.println("Stepper Initialised");
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
