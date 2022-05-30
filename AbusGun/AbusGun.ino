/*********
Abus.c 
Christian H
*********/

/*
 * TODO
 */



#define LimitSwitch   4

int gInitFlag = 0;

/*****************************************************************************************************************************/

void setup() 
{
  Serial.begin(115200);  

  Serial.println("XTAL Freq:");
  Serial.println(getXtalFrequencyMhz());
  Serial.println("CPU Freq:");
  Serial.println(getCpuFrequencyMhz());

  Launcher_InitPins();
  
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
  
  if(gInitFlag == 1)
  {
    Launcher_Init();
    gInitFlag = 2;
  }

  if(gInitFlag == 2)
  {
    Launcher_UpdateMotors();
  }
}

void Launcher_SetInitFlag()
{
  gInitFlag = 1;
}
