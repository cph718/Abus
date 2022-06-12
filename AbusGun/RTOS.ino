
TaskHandle_t Task1;
int gTask1ActiveFlag = 0;
TaskHandle_t Task2;
int gTask2ActiveFlag = 0;

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

  gTask1ActiveFlag = 1;
} 

void RTOS_StopTask1()
{
  vTaskDelete(Task1);
  gTask1ActiveFlag = 0;
}

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
void RTOS_InitTask2()
{
  xTaskCreatePinnedToCore(
                    Task2code,    /* Task function. */
                    "Task2",      /* name of task. */
                    10000,            /* Stack size of task */
                    NULL,             /* parameter of the task */
                    1,                /* priority of the task */
                    &Task2,           /* Task handle to keep track of created task */
                    1);               /* pin task to core 0 */ 
  gTask2ActiveFlag = 1;
}

void RTOS_StopTask2()
{
  vTaskDelete(Task2);
  gTask2ActiveFlag = 0;
}

void RTOS_StopActiveTasks()
{
  if(gTask1ActiveFlag == 1)
  {
    RTOS_StopTask1();
  }

  if(gTask2ActiveFlag == 1)
  {
    RTOS_StopTask2();
  }
}


void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());
  
//Task1code: Acivates next stepper operation
  for(;;)
  {
    Reload_StepperRunMoveTo();
  } 
}

void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());
  
//Task1code: Acivates next stepper operation
  for(;;)
  {
    Reload_NextStepperContinuousStep();
  } 
}
