#include "logger.h"
#include "service_info.h"
#include "light_control.h"
#include "user_input.h"
#include "water_control.h"

int OWater1 = 2;
int OLight1 = 3;

//Pin 1 for moisture sensor
int Pin1 = A0;

//Pin 2 For light Sensor
int Pin2 = A1;

float water1 = 0;
float light1 = 0;
void setup() {
  Serial.begin(9600);
  pinMode(OWater1, OUTPUT);
  pinMode(Pin1, INPUT);
  pinMode(OLight1, OUTPUT);
  pinMode(Pin2, INPUT);
  
  digitalWrite(OWater1, HIGH);
  digitalWrite(OLight1, HIGH);
  delay(500);
}
void loop() {
  Serial.print("MOISTURE LEVEL: ");
  water1 = analogRead(Pin1);
  Serial.println(water1);
  if(water1>500)
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
  if (light1 < 550){
    digitalWrite(OLight1, HIGH);
  }else{
    digitalWrite(OLight1, LOW);
  }
  delay(1000);
}
