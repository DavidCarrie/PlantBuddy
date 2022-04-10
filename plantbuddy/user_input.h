extern int waterThreshold;
extern float lightThreshold;
extern bool debugCommandLight;
extern bool debugCommandWater;
extern bool debugOverwriteMode;
extern int loopdelay;
void userInputPrintGuide()
{
      Serial.println("\n\n-----------\nCommands guideline:");
    //Serial.println("type 'check' to check the current water/light threshold value."); // We need to decide if the status output is by default or if the user has to type command to see the status
    Serial.println("type 'set water' to change the water threshold value.");
    Serial.println("type 'set light' to change the light threshold value.");
    //Serial.println("type 'status' to check the current plant value.");
    Serial.println();
  }


  void userInputInLoopCheck()
  {
     if(Serial.available()){
String command = Serial.readStringUntil('\n');

        if(command.equals("set water")){    
  Serial.println("Type a number in 5 seconds!");
  delay(5000);
  if(Serial.available()){
    int new_water_value = Serial.parseInt();
    waterThreshold = new_water_value;
    Serial.print("The new default water value is: " );
    Serial.println(waterThreshold);
    Serial.println("Successful! You can give next command.");
    Serial.println();
  }
  
  else{
    Serial.println("No input received. Please give a new command.");
    Serial.println();
  }
        }
/**/
        else if(command.equals("set delay")){    
  Serial.println("Type a number in 5 seconds!");
  delay(5000);
  if(Serial.available()){
     loopdelay = Serial.parseInt();
     
    Serial.print("The new loop sleep time: " );
    Serial.println(loopdelay);
    Serial.println("Successful! You can give next command.");
    Serial.println();
  }
  
  else{
    Serial.println("No input received. Please give a new command.");
    Serial.println();
  }
        }

/**/


        
        else if(command.equals("set light")){
  Serial.println("Type a number in 5 seconds!");
  delay(5000);
  if(Serial.available()){
    int new_light_value = Serial.parseInt();
    lightThreshold = new_light_value;
    Serial.print("The new default light value is: " );
    Serial.println(lightThreshold);
    Serial.println("Successful! You can give next command.");
    Serial.println();
  }
  else{
    Serial.println("No input received. Please give a new command.");
    Serial.println();
  }        }
        else if(command.equals("check")){
  Serial.print("The current water threshold value is: " );
  Serial.println(waterThreshold);
  Serial.println();
  Serial.print("The current light threshold value is: " );
  Serial.println(lightThreshold);
  Serial.println();        }
        //else if(command.equals("status")){   }//NoNeed Status as already showing status in main code(plantbuddy.ino)
      else if (command.equals("overwrite")){debugOverwriteMode=!debugOverwriteMode;}
 
 
      else if (command.equals("lon") && debugOverwriteMode){debugCommandLight=true;}
      else if (command.equals("loff")&& debugOverwriteMode){debugCommandLight=false;}
      else if (command.equals("won")&& debugOverwriteMode){debugCommandWater=true;}
      else if (command.equals("woff")&& debugOverwriteMode){debugCommandWater=false;}
      else{
            Serial.println("Invalid command");
        }
            }
    }


//Directly copied from the User command . ini into .h file  but I think it ll work

 

 
    
