
TaskHandle_t Task1;

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 1
void RTOS_InitTask1()
{
  xTaskCreatePinnedToCore(
                    Task1code,    /* Task function. */
                    "Task1",      /* name of task. */
                    10000,            /* Stack size of task */
                    NULL,             /* parameter of the task */
                    1,                /* priority of the task */
                    &Task1,           /* Task handle to keep track of created task */
                    1);               /* pin task to core 0 */ 
} 



void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());
  
//Task1code: blinks an LED every 1000 ms
  for(;;)
  {
    Reload_StepperRunMoveTo();
  } 
}
