/*********
Abus.c 
Christian H
*********/

/*
 * TODO
 */

//Board Pins
#define ESC1_CTRLPIN         16
#define ESC2_CTRLPIN         17
#define LAUNCHER_SPEEDPIN    13
#define LAUNCHER_ENABLEPIN   21
#define FIRE_TRIGGERPIN      22
#define STEPPER_DIRPIN       18
#define STEPPER_STEPPIN      19
#define STEPPER_HOMEPIN      23

int gTriggerFlag = 0;
int gStepperHomeFlag = 0;
int debounceTime = 500;
unsigned long triggerLastDetection = 0;
 
void IRAM_ATTR ISR_FirePin() 
{
  gTriggerFlag = 1;
}

void IRAM_ATTR ISR_StepperHome() 
{
  gStepperHomeFlag = 1;
}

/*****************************************************************************************************************************/

void setup() 
{
  Serial.begin(115200);  

  Serial.println("XTAL Freq:");
  Serial.println(getXtalFrequencyMhz());
  Serial.println("CPU Freq:");
  Serial.println(getCpuFrequencyMhz());

  pinMode(STEPPER_HOMEPIN, INPUT);
  attachInterrupt(STEPPER_HOMEPIN, ISR_StepperHome, FALLING);
  
  pinMode(FIRE_TRIGGERPIN, INPUT_PULLUP);
  attachInterrupt(FIRE_TRIGGERPIN, ISR_FirePin, FALLING);

  pinMode(LAUNCHER_ENABLEPIN, INPUT_PULLUP);

 // Launcher_Init();
  
  Reload_InitStepper();

  //WebServer_Init();
}

//TODO Add a way of reporting the current set speed
/*****************************************************************************************************************************/
void loop() 
{  
//  if(digitalRead(LAUNCHER_ENABLEPIN) == 0)
//  {
//    Launcher_SetPotSpeed();
//  } else
//  {
//    Launcher_SetOffSpeed();
//  }
  
  //Launcher_UpdateMotors();

  if(gStepperHomeFlag == 1)
  {
    Reload_DisableHomingMovement();
    Reload_SetHomePositionFlag();
    detachInterrupt(STEPPER_HOMEPIN); 
    gStepperHomeFlag = 0;
  }
  
  if(gTriggerFlag == 1)
  {
    if(millis() - triggerLastDetection > debounceTime)
    {
      Reload_UpdateStepperStateMachine();
    
     triggerLastDetection = millis();
    }
    gTriggerFlag = 0;
  }
}
