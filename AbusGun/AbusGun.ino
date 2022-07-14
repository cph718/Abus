/*********
Abus.c 
Christian H
*********/

/*
 * TODO
 */

#include "definitions.h"


//Board Pins
#define ESC1_CTRLPIN         16
#define ESC2_CTRLPIN         17
#define LAUNCHER_SPEEDPIN    13
#define LAUNCHER_ENABLEPIN   21
#define FIRE_TRIGGERPIN      22
#define STEPPER_DIRPIN       18
#define STEPPER_STEPPIN      19
#define STEPPER_HOMEPIN      23

/*****************************************************************************************************************************/

void setup() 
{
  Serial.begin(115200);  

  Serial.println("XTAL Freq:");
  Serial.println(getXtalFrequencyMhz());
  Serial.println("CPU Freq:");
  Serial.println(getCpuFrequencyMhz());
  
  WebServer_Init();
}

//TODO Add a way of reporting the current set speed
/*****************************************************************************************************************************/
void loop() 
{  
  Reload_UpdateStepperStateMachine();
}
