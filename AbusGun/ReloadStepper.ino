/*********
ReloadStepper.c 
Christian H
*********/

#include <AccelStepper.h>

#define StepPerRev    3200
#define motorInterfaceType 1



StepperState currState = defaultState;
AccelStepper stepper = AccelStepper(motorInterfaceType, STEPPER_STEPPIN, STEPPER_DIRPIN);

int nextSpeed, nextAccel, nextPos;

//Global Functions


void Reload_UpdateStepperStateMachine()
{
  if(currState == defaultState)
  {

  } 
  else if(currState == positionalInit)
  {
    Reload_SetUpMoveTo(nextSpeed, nextAccel);
    Reload_StepperSetMoveTo(nextPos);

    currState = positionalMoving;
    
  } 
  else if(currState == positionalMoving)
  {
    Reload_StepperRunMoveTo();
    
    if(!stepper.isRunning())
    {
      currState = positionalStop;
    }
    
  } 
  else if(currState == positionalStop)
  {
    currState = defaultState;
    
  } 
  else if(currState == continuousInit)
  {

  } 
  else if(currState == continousMoving)
  {

  } 
  else if(currState == ContinousActive)
  {

  } else
  {

  }
}

int Reload_ChangeStateMachine(StepperState nextState)
{
  if((currState == defaultState) | (currState == ContinousActive))
  {
    currState = nextState;
    Serial.print("New State:");
    Serial.println(nextState);
    return  1;
  }

  Serial.print("State change denied");
  return 0;
}

void Reload_SetNextParamaters(int speedValue, int accelValue, int posValue)
{
  nextSpeed =  speedValue;
  nextAccel = accelValue;
  nextPos = posValue;
}

//Local Functions
void Reload_SetUpMoveTo(int posSpeed, int posAccel)
{
  //Set Stepper drive anti-clockwise
  stepper.setMaxSpeed(posSpeed);
  Serial.print("Speed Set:");
  Serial.println(posSpeed);
  stepper.setAcceleration(posAccel);
  Serial.print("Accel Set:");
  Serial.println(posAccel);
}

void Reload_SetUpContinous(int conSpeed, int conAccel)
{
  //Set Stepper drive anti-clockwise
  stepper.setSpeed(conSpeed);
  stepper.setAcceleration(conAccel);
}

//Positional Movement
void Reload_StepperSetMoveTo(int PosInSteps)
{
  stepper.moveTo(PosInSteps);
    Serial.print("Move Position:");
  Serial.println(PosInSteps);
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
