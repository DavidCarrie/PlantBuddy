#include "logger.h"
#include "service_info.h"
#include "light_control.h"
#include "user_input.h"
#include "water_control.h"
#include "timer.h"
unsigned long militime;
 
int OWater1 = 2;
int OLight1 = 3;

//Pin 1 for moisture sensor
int Pin1 = A0;

//Pin 2 For light Sensor
int Pin2 = A1;

float water1 = 0;//moisture lvl
float light1 = 0;

float waterThreshold=500;
float lightThreshold=550;
/*ORiginal var*/


//Start water variable overwrite

// Water variables
////int OWater1 = 3;//?????????We dont know why T switched pin 2 3 ; water light input pin
//@TODO 1 Water: why pin switch  2: How do we know it works, prev watered 

// Light variables
//int OLight1 = 2; // ??? 
 
int light = 0; //represents light status: 0 -> off, 1 -> on


unsigned long previousWateredTime = 0;
float relativeWateredTime = 0; // Time since last watered, in minutes for now
float waterThresholdRange=100; // the range +/- the waterThreshold that is okay. Configurable by user
float minWaterThreshold = waterThreshold - waterThresholdRange;
float maxWaterThreshold = waterThreshold + waterThresholdRange;
float waterInterval = 8; // Configurable by user + the system automatically, in minutes for now
float minWaterInterval = 4; // Configurable by user, in minutes for now
float maxWaterInterval = 12; // Configurable by user, in minutes for now

//end water var overwrite

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
  setTime(0,22,30,1);//SetTime(day, hour, minute,Clockspeed_multiplier)

//NEw setup
  setupWaterModule();

  setupLightModule();

   delay(500); 

/* Test WriteLog and ReadLog. The data should be kept even when we reset.
writeLog(3,0,8,30,57);
writeLog(2,0,8,35,58);
writeLog(6,3,10,30,01);*/

readLog();
 }
void loop() {
 delay(1000);
/*BEGIN WIP modules*/
userInputPrintGuide();
userInputInLoopCheck();

waterModule();
  lightModule(hour(),day(), minute());

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

//
//displayWaterVariables(); 

}//END of LOOP

/*BEGIN Water Function DEF*/
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------

//Sets up the water module. Called in the setup function in plantbuddy.ino
void setupWaterModule() {
  pinMode(OWater1, OUTPUT);
  pinMode(Pin1, INPUT);
  digitalWrite(OWater1, HIGH);
  previousWateredTime = militime/(unsigned long)60000;
}

//The main functionality for the water module. Called in the loop function in plantbuddy.ino
void waterModule() {

  Serial.println("minWaterThreshold: " + String(minWaterThreshold) + ", maxWaterThreshold: " + String(maxWaterThreshold) + ", waterInterval: " + String(waterInterval));
  //Water Sensing
  Serial.print("Moisture Level: ");
  water1 = analogRead(Pin1);
  Serial.println(water1);

   relativeWateredTime = (militime/(unsigned long)60000)-previousWateredTime;
   Serial.print("militime = "); Serial.println(militime/(unsigned long)60000);
   Serial.print("previousWateredTime = "); Serial.println(previousWateredTime);
   Serial.print("relativeWateredTime = "); Serial.println(relativeWateredTime);
   Serial.println("The last time the plant was watered was: " + String(relativeWateredTime) + " minutes ago. ");

  if(water1 < minWaterThreshold && relativeWateredTime >= waterInterval)
  {
      Serial.print("  Setting water level...");
      digitalWrite(OWater1, HIGH);
      Serial.println("Set Water level to HIGH.");
      previousWateredTime = militime/(unsigned long)60000;
  }
  else if(water1 < minWaterThreshold && relativeWateredTime < waterInterval)
  {
      Serial.print("  Adjusting water interval for next time... ");
      adjustWaterInterval(0);
      Serial.print("  Setting water level..."); digitalWrite(OWater1, HIGH);
      Serial.println("Set Water level to HIGH.");
      previousWateredTime = militime/(unsigned long)60000;
  }  
  else if(water1 > maxWaterThreshold && relativeWateredTime > waterInterval)
  {
      Serial.println("Water moisture is still moist...");
    adjustWaterInterval(1);
      Serial.print("  Setting water level..."); digitalWrite(OWater1, LOW); Serial.println("Set Water level to LOW.");
      previousWateredTime = militime/(unsigned long)60000;
  }   
  else
  {
      Serial.print("  Setting water level..."); digitalWrite(OWater1, LOW); Serial.println("Set Water level to LOW.");
  }

}

// System automatically configures water interval variable based on environment. Called by the waterModule().
void adjustWaterInterval(int status) {
  if (status == 1) {
    if (waterInterval >= maxWaterInterval) {
      Serial.println("Please check the system/water-levels. The plant was last watered " + String(relativeWateredTime) + " minutes ago, yet the environment is still moist.");
        exit(0);
    }
    else {
      waterInterval = waterInterval + 1;
        Serial.print("Water interval is now: "); Serial.println(waterInterval);  
    }
  }
  else if (status == 0) {
    if (waterInterval <= minWaterInterval) {
      Serial.println("Please check the system/water-levels. The plant was last watered " + String(relativeWateredTime) + " minutes ago, yet the environment is already dry.");
      exit(0);
    }
    else {
      waterInterval = waterInterval - 1;
      Serial.print("Water interval is now: "); Serial.println(waterInterval);  
    }  
  }
}

void displayWaterVariables() {

  Serial.println("OWater1: " + String(OWater1));
  Serial.println("Pin1: " + String(Pin1));
  Serial.println("water1: " + String(water1));
  Serial.println("previousWateredTime: " + String(previousWateredTime));
  Serial.println("relativeWateredTime: " + String(relativeWateredTime));
  Serial.println("waterThreshold: " + String(waterThreshold));
  Serial.println("waterThresholdRange: " + String(waterThresholdRange));
  Serial.println("minWaterThreshold: " + String(minWaterThreshold));
  Serial.println("maxWaterThreshold: " + String(maxWaterThreshold));
  Serial.println("waterInterval: " + String(waterInterval));
  Serial.println("minWaterInterval: " + String(minWaterInterval));
  Serial.println("maxWaterInterval: " + String(maxWaterInterval));

}


//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//End of water Function Definition
//Begin Light function Def

void setupLightModule(){
  pinMode(OLight1, OUTPUT);
  pinMode(Pin2, INPUT);
  digitalWrite(OLight1, HIGH);
}

void lightModule(int currentHour, int currentDay, int currentMinute){
  //light sensoring
  Serial.print("Light Level: ");
  light1 = analogRead(Pin2);
  Serial.println(light1);
  Serial.println();
  
  //assume daytime is in range of [7, 19]
  if (currentHour >= 7 && currentHour < 19 && light1 < lightThreshold && light == 0){
      light_on(currentHour, currentDay, currentMinute);
  }
  /*else if (currentHour >= 7 && currentHour < 19 && light1 >= lightThreshold && light == 1){
      light_off(currentHour, currentDay, currentMinute);
  }*/
  //assume nighttime is in range of [19, 24) & [0, 7)
  else if (currentHour >= 0 && currentHour < 7 && light == 1){
      light_off(currentHour, currentDay, currentMinute);
  }
  else if (currentHour >= 19 && currentHour < 24 && light == 1){
      light_off(currentHour, currentDay, currentMinute);
  }
}

void light_on(int currentHour, int currentDay, int currentMinute){
  digitalWrite(OLight1, HIGH);
  light = 1;
  Serial.print("light turn up at day ");
  Serial.print(currentDay);
  Serial.print("   hour ");
  Serial.print(currentHour);
  Serial.print("   minute ");
  Serial.print(currentMinute);
  Serial.println();
}

void light_off(int currentHour, int currentDay, int currentMinute){
  light = 0;
  digitalWrite(OLight1, LOW);
  Serial.print("light turn off at day ");
  Serial.print(currentDay);
  Serial.print("   hour ");
  Serial.print(currentHour);
  Serial.print("   minute ");
  Serial.print(currentMinute);
  Serial.println();
}

//End light func def;
