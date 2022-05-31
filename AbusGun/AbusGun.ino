/*********
Abus.c 
Christian H
*********/

/*
 * TODO
 */

 

#define FireTriggerPin   21
#define HomeLimitSwitch   4
#define LauncherSpeedPin   2

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

int gInitFlag = 0;

unsigned long lastDetection = 0;
unsigned long debounceTime = 250;
 
SemaphoreHandle_t syncSemaphore;
 
void IRAM_ATTR ISR_FirePin() {
    xSemaphoreGiveFromISR(syncSemaphore, NULL);
}

void IRAM_ATTR ISR_HomeSwitch() 
{
  Reload_SetHomePosition();
}

/*****************************************************************************************************************************/

void setup() 
{
  Serial.begin(115200);  

  Serial.println("XTAL Freq:");
  Serial.println(getXtalFrequencyMhz());
  Serial.println("CPU Freq:");
  Serial.println(getCpuFrequencyMhz());

  pinMode(HomeLimitSwitch, INPUT_PULLDOWN);
  attachInterrupt(HomeLimitSwitch, ISR_HomeSwitch, FALLING);

  syncSemaphore = xSemaphoreCreateBinary();

  pinMode(FireTriggerPin, INPUT_PULLUP);
  attachInterrupt(FireTriggerPin, ISR_FirePin, FALLING);

  Launcher_InitPins();
  
  Reload_InitStepper();

  //WebServer_Init();

  Reload_FindHome();
}

//TODO Add a way of reporting the current set speed
/*****************************************************************************************************************************/
void loop() 
{  
  if(gInitFlag == 1)
  {
    Launcher_Init();
    gInitFlag = 2;
  }

  if(gInitFlag == 2)
  {
    Launcher_UpdateMotors();
  }
  
   xSemaphoreTake(syncSemaphore, portMAX_DELAY);
 
  if(millis() - lastDetection > debounceTime)
  {
    Reload_MoveToNextPos();
    Serial.println("flag reset");
    
    lastDetection = millis();
  }
}

void Launcher_SetInitFlag()
{
  gInitFlag = 1;
}
