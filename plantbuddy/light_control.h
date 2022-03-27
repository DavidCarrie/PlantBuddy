unsigned long time;
String command;
unsigned long previousBlinkTime;
int blinkInterval = 5000;
int light = 0;

void setup(){
  Serial.begin(9600);
}

void loop(){
  unsigned long currentMillis = millis(); // get current time
  if (light == 1){
    millisBlink(currentMillis);
  }
  
  if(Serial.available()){
      command = Serial.readStringUntil('\n');
      if(command.equals("time")){
          print_time();
      }
      else if(command.equals("set light")){
          set_light();
      }
  } 
  delay(1000);
}

void set_light(){
  Serial.println("light turn up");
  light = 1;
  previousBlinkTime = millis();
}

void millisBlink(unsigned long currentTime) { 
  //check time interval
  if (currentTime - previousBlinkTime >= blinkInterval) {    //if ture
    Serial.println("light off");                       //turn off light
    light = 0;
    previousBlinkTime = currentTime;  //store new previousBlinkTime    
    
             
  }  else if (currentTime - previousBlinkTime <= 0) {  //if time overflow 
    previousBlinkTime = currentTime;
  }
}
