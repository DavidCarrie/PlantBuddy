#include "logger.h"
#include "service_info.h"
#include "light_control.h"
#include "user_input.h"
#include "water_control.h"
#include "timer.h"
 
int OWater1 = 2;
int OLight1 = 3;

//Pin 1 for moisture sensor
int Pin1 = A0;

//Pin 2 For light Sensor
int Pin2 = A1;

float water1 = 0;
float light1 = 0;

float waterThreshold=500;
float lightThreshold=550;


void setup() {
  Serial.begin(9600);
  while (!Serial) {}/*wait for serial*/
  pinMode(OWater1, OUTPUT);
  pinMode(Pin1, INPUT);
  pinMode(OLight1, OUTPUT);
  pinMode(Pin2, INPUT);
  
  digitalWrite(OWater1, HIGH);
  digitalWrite(OLight1, HIGH);
//Temp time setup we mentioned user input real time but its just for syncing purpose and not a service provided so it feels fine by me to just set time in code 
  setTime(0,22,30,19);//SetTime(day, hour, minute,Clockspeed_multiplier)
  delay(500);



 }
void loop() {
 delay(1000);
/*BEGIN WIP modules*/
userInputPrintGuide();
userInputInLoopCheck();
/*END WIP modules*/


/*BEGIN original raw material from David  */
  Serial.print("MOISTURE LEVEL: ");
  water1 = analogRead(Pin1);
  Serial.println(water1);
 
  if(water1>waterThreshold)
  {
      digitalWrite(OWater1, HIGH);
  }
  else
  {
    digitalWrite(OWater1, LOW);
  }
    

   //Light Sensing
  Serial.print("Light Level: ");
  light1 = analogRead(Pin2);
  Serial.println(light1);
  Serial.println(); 

  if (light1 < lightThreshold){
    digitalWrite(OLight1, HIGH);
  }else{
    digitalWrite(OLight1, LOW);
  }

/*DEBUG:SHOW ALL VAR*/
Serial.print("light threshold: ");
Serial.println(lightThreshold);
Serial.print("water threshold: ");
Serial.println(waterThreshold);

// timer.h provides get functions e.g. day()   second()   hour()  minute()
Serial.print("day ");
Serial.print(day());
Serial.print("   hour ");
Serial.print(hour());
Serial.print("   minute ");
Serial.print(minute());
Serial.print("   second ");
Serial.print(second());

}
