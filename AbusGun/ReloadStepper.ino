/*********
ReloadStepper.c 
Christian H
*********/

#include <AccelStepper.h>

#define StepPerRev    3200
#define motorInterfaceType 1



StepperState currState = defaultState;
AccelStepper stepper = AccelStepper(motorInterfaceType, STEPPER_STEPPIN, STEPPER_DIRPIN);

int gNextSpeed, gNextAccel, gNextPos, gConState;

//Global Functions


void Reload_UpdateStepperStateMachine()
{
  if(currState == defaultState)
  {

  } 
  else if(currState == positionalInit)
  {
    Reload_SetUpMoveTo(gNextSpeed, gNextAccel);
    Reload_StepperSetMoveTo(gNextPos);

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
    Reload_SetUpContinuous(gNextSpeed, gNextAccel);

    currState = continuousActive;
    
  }
  else if(currState == continuousActive)
  {
    if(gConState == 2) //move reverse state
    {
      //Set Direction
      currState = continuousMoving;
      
    } else if (gConState == 1) //move forward state
    {
      
      currState = continuousMoving;
    } else //stop state
    {
      Set Direction
      //stop state
    }
  }
  else if(currState == continuousMoving)
  {
    Reload_NextStepperContinuousStep();

    if(gConState == 0)
    {
      currState = continuousActive;
    }
  } else
  {

  }
}

int Reload_ChangeStateMachine(StepperState nextState)
{
  if((currState == defaultState) | (currState == continuousActive))
  {
    currState = nextState;
    Serial.print("New State:");
    Serial.println(nextState);
    return  1;
  }

  Serial.print("State change denied");
  return 0;
}

void Reload_UpdateContinuousState(int conState)
{
  gConState = conState;
  Serial.print("New State:");
  Serial.println(conState);
}

void Reload_SetNextParamaters(int speedValue, int accelValue, int posValue)
{
  gNextSpeed =  speedValue;
  gNextAccel = accelValue;
  gNextPos = posValue;
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

void Reload_SetUpContinuous(int conSpeed, int conAccel)
{
  //Set Stepper drive anti-clockwise
  stepper.setSpeed(conSpeed);
  Serial.print("Speed Set:");
  Serial.println(conSpeed);
  stepper.setAcceleration(conAccel);
  Serial.print("Accel Set:");
  Serial.println(conAccel);
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


//Continuous Running
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
void Reload_StepperDirUp()
{
  digitalWrite(STEPPER_DIRPIN, HIGH);
}

void Reload_StepperDirDown()
{
  digitalWrite(STEPPER_DIRPIN, LOW);
}
