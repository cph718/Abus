/*********
ReloadStepper.c 
Christian H
*********/

#include <AccelStepper.h>




void Reload_InitStepper(int DirectionPin)
{
  //Set Stepper drive clockwise
  digitalWrite(DirPin, HIGH);
  stepper.setMaxSpeed(5000);
  stepper.setAcceleration(30);
  Serial.println("Stepper Initialised");
}
