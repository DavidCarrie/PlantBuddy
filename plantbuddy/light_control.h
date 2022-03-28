int OLight1 = 2;
int Pin2 = A1;
float light1 = 0;
float lightThreshold=550;

int previousOnTime;
int previousOffTime;

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
  if (currentHour >= 7 && currentHour < 19 && light1 < lightThreshold){
    	light_on(currentHour, currentDay, currentMinute);
  }
  //assume nighttime is in range of [19, 24) & [0, 7)
  else if (currentHour >= 0 && currentHour < 7 && light1 >= lightThreshold){
    	light_off(currentHour, currentDay, currentMinute);
  }
  else if (currentHour >= 19 && currentHour < 24 && light1 >= lightThreshold){
    	light_off(currentHour, currentDay, currentMinute);
 	}
}

void light_on(int currentHour, int currentDay, int currentMinute){
  digitalWrite(OLight1, HIGH);
  Serial.print("light turn up at day ");
  Serial.print(currentDay);
  Serial.print("   hour ");
  Serial.print(currentHour);
  Serial.print("   minute ");
  Serial.print(currentMinute);
  Serial.println();
}

void light_off(int currentHour, int currentDay, int currentMinute){
  digitalWrite(OLight1, LOW);
  Serial.print("light turn off at day ");
  Serial.print(currentDay);
  Serial.print("   hour ");
  Serial.print(currentHour);
  Serial.print("   minute ");
  Serial.print(currentMinute);
  Serial.println();
}


