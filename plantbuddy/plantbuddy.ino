#include "logger.h"
#include "service_info.h"
#include "light_control.h"
#include "user_input.h"
#include "water_control.h"
#include "timer.h"

//START DEBUG VAR
bool useLog =false;//in relative section, set if(useLog && *log condition*) {  *logging the event* }
//bool testLight=true;//will bypass condition (if *light condition* || testLight) {light event}
bool waterExtraDebug=true;//extra water info
bool debugCommandLight=false;
bool debugCommandWater=false;
bool debugOverwriteMode=false;
// START VARIABLES SECTION
unsigned long militime;
 
int OWater1 = 2;
int OLight1 = 3;

//Pin 1 for moisture sensor
int Pin1 = A0;

//Pin 2 For light Sensor
int Pin2 = A1;

float light1 = 0;
float lightThreshold=550;
 
int light = 0; //represents light status: 0 -> off, 1 -> on

float water1 = 0; // Moisture level
int waterThreshold=600; // Configurable by user
int waterThresholdRange=300; // the range +/- the waterThreshold that is okay. Configurable by user
int minWaterThreshold = waterThreshold - waterThresholdRange;
int maxWaterThreshold = waterThreshold + waterThresholdRange;
unsigned long previousWateredTime = 0;
int relativeWateredTime = 0; // Time since last watered, in minutes for now
int waterPumpTime = 5000; // How long to water the plant when in watering mode
int waterReactTime = 10000;//delay : water takes time to inc moisture level
int waterSchedule = 12; // Configurable by user, in hours for now
int waterScheduleRange = 2; // Configurable by user, in hrs for now
int frameCounter=0;
int rawWater[5];
int rawLight[5];
// END VARIABLES SECTION

void setup() {
  Serial.begin(9600);
  while (!Serial) {}/*wait for serial*/
  pinMode(OWater1, OUTPUT);
  pinMode(Pin1, INPUT);
  pinMode(OLight1, OUTPUT);
  pinMode(Pin2, INPUT);
  
/*  digitalWrite(OWater1, HIGH);
  digitalWrite(OLight1, HIGH);*/
//Temp time setup we mentioned user input real time but its just for syncing purpose and not a service provided so it feels fine by me to just set time in code 
  setTime(0,8,30,1);//SetTime(day, hour, minute,Clockspeed_multiplier)
  Serial.println("\nThe time is: " + String(day()) + ":" + String(hour()) + ":" + String(minute()) + ":" + String(second()));
//NEw setup
  setupWaterModule();

  setupLightModule();

   delay(500); 

/* Test WriteLog and ReadLog. The data should be kept even when we reset.
writeLog(3,0,8,30,57);
writeLog(2,0,8,35,58);
writeLog(6,3,10,30,01);*/

//readLog();
 }
void loop() {
    delay(2000);
    /*BEGIN WIP modules*/
    userInputPrintGuide();
    userInputInLoopCheck();
    
    Serial.println("\nThe time is: " + String(day()) + ":" + String(hour()) + ":" + String(minute()) + ":" + String(second()));
   
    if(frameCounter>=5){frameCounter=0;}
    if(!debugOverwriteMode){
    waterModule();
    lightModule(hour(),day(), minute());
    }
    /*END WIP modules*/
 frameCounter+=1;
    /*BEGIN original raw material from David  

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

    if (light1 < light){
        digitalWrite(OLight1, HIGH);
    }else{
        digitalWrite(OLight1, LOW);
    }

    */

    /*DEBUG:SHOW ALL VAR*/
    Serial.println("------------------- PRINTING ALL VARIABLES -------------------");


    // timer.h provides get functions e.g. day()   second()   hour()  minute()
    Serial.print("day ");
    Serial.print(day());
    Serial.print("   hour ");
    Serial.print(hour());
    Serial.print("   minute ");
    Serial.print(minute());
    Serial.print("   second ");
    Serial.println(second());

    //
    displayWaterVariables(); 
    displayLightVariables();
    
    /*DEBUG Command On Off */
    if(debugCommandWater){  digitalWrite(OWater1, HIGH);}
    else{digitalWrite(OWater1, LOW);}
    if(debugCommandLight){  digitalWrite(OLight1, HIGH);
}
    else{digitalWrite(OLight1, LOW);}
    Serial.println("--------------------------------------------------------------");

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
  //Water Sensing and 5 value avg
  rawWater[frameCounter] = analogRead(Pin1);
  if(frameCounter==4)
  {water1=(rawWater[0]+rawWater[1]+rawWater[2]+rawWater[3]+rawWater[4])/5; 
  
  relativeWateredTime = (militime/(unsigned long)60000)-previousWateredTime;
  
  Serial.print("Moisture Level: ");
  Serial.println(water1);
  Serial.print("militime = "); Serial.println(militime/(unsigned long)60000);
  Serial.print("previousWateredTime = "); Serial.println(previousWateredTime);
  Serial.print("relativeWateredTime = "); Serial.println(relativeWateredTime);
  Serial.println("The last time the plant was watered was: " + String(relativeWateredTime) + " minutes ago. ");
  
  Serial.println("Checking if the plant needs to be watered... ");
  
  // If the water level goes below the minimum threshold.
  if(water1 < minWaterThreshold) {
        if(waterExtraDebug){Serial.println("water reading < min water threshold");}
    // Increase water pumping time if the last time watered was too recent.
    if (relativeWateredTime < (waterSchedule-waterScheduleRange)) {
        if(waterExtraDebug){Serial.println("Time since last watered too short, add pump time");}  
    	waterPumpTime = waterPumpTime + 500;	  	
    }
    // Decrease water pumping time if the last time watered was too long ago.
    else if (relativeWateredTime > (waterSchedule+waterScheduleRange)) {
              if(waterExtraDebug){Serial.println("Time since last watered too long, subtract pump time");}  

    	waterPumpTime = waterPumpTime - 500;	
    }
    // Water the plant
    Serial.println("  Watering the plant...");
    digitalWrite(OWater1, HIGH);
    delay(waterPumpTime);
    digitalWrite(OWater1, LOW);
    //@Todo log instead of print
    delay(waterReactTime);//delay : water takes time to inc moisture level

    Serial.println("  Plant has been watered now.\n");
    Serial.println("\nThe time is: " + String(day()) + ":" + String(hour()) + ":" + String(minute()) + ":" + String(second()));
    previousWateredTime = militime/(unsigned long)60000;
    Serial.println("Previous watered time: " + previousWateredTime);
  }
  // Do nothing if the water level is between the min and max thresholds
  else if (water1 >= minWaterThreshold && water1 <= maxWaterThreshold) {
            if(waterExtraDebug){Serial.print("min<water<max");}  

  	Serial.println("  Watering not needed currently.");
  }
  // Inform user to check the system if there is too much water in the environment.
  else if (water1 > maxWaterThreshold) {
    digitalWrite(OWater1, LOW); // Ensure the system is set to no-watering mode.
    Serial.println("  Water>maxWaterThreshold Please check the system, there is too much water in the environment.");
    // exit(0) and print error log
  }
  }// End of if framecounter==4  (can get avg from full array)
}

void displayWaterVariables() {
  Serial.println("OWater1: " + String(OWater1));
  Serial.println("Pin1: " + String(Pin1));
  Serial.println("water1: " + String(water1));
  //Serial.println("waterRawReading:" + String(rawWater[0]));
 // Serial.println(","+ String(rawWater[1])+","+  String(rawWater[2])+","+  String(rawWater[3])+","+  String(rawWater[4]));
  Serial.println("previousWateredTime: " + String(previousWateredTime));
  Serial.println("relativeWateredTime: " + String(relativeWateredTime));
  Serial.println("waterThreshold: " + String(waterThreshold));
  Serial.println("waterThresholdRange: " + String(waterThresholdRange));
  Serial.println("minWaterThreshold: " + String(minWaterThreshold));
  Serial.println("maxWaterThreshold: " + String(maxWaterThreshold));
  Serial.println("waterSchedule: " + String(waterSchedule));
  Serial.println("waterScheduleRange: " + String(waterScheduleRange));
  Serial.println("waterPumpTime: " + String(waterPumpTime));
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
   //light1 = analogRead(Pin2);

rawLight[frameCounter] = analogRead(Pin2);
  if(frameCounter==4)
  {light1=(rawLight[0]+rawLight[1]+rawLight[2]+rawLight[3]+rawLight[4])/5;

  /**/
 
  
  //assume daytime is in range of [7, 19]
  if (currentHour >= 7 && currentHour < 19 && (light1 < lightThreshold) && light == 0){
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
  }//end of if fC==4 array filled
}

void light_on(int currentHour, int currentDay, int currentMinute){
  digitalWrite(OLight1, HIGH);
  light = 1;
      //@Todo log instead of print

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
void displayLightVariables()
{
  Serial.print("light threshold: ");
  Serial.println(lightThreshold);
  Serial.println("light1: " + String(light1));
  //Serial.println("lightRawReading:" + String(rawLight[0]));
  //Serial.println(","+ String(rawLight[1])+","+  String(rawLight[2])+","+  String(rawLight[3])+","+  String(rawLight[4]));
  Serial.print("light status: ");
  Serial.println(String(light));
  }

//End light func def;
