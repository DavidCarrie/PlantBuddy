//Event Day Hour Min Sec
//Event:1 Water   2 Light on   3 Light off      4WaterErr    5LightErr 6Threshold change  7 Frequency inc 8 Freq dec 
//(0-255 num in byte, Code for event are draft. can be changed anyways , we are only storing numbers, the mapping of code and event can vary)
//Day num in byte
//Hour num in byte
//Min num in byte
//Sec num in byte
#include <EEPROM.h>

int addr=0;
int readaddr=0;
struct logType {
  byte event;
  byte day;
  byte hour;
  byte minute;
  byte second;
 };
logType logtowrite;
logType logread;

void writeLog(byte event, byte day, byte hour, byte minute, byte second)
{
  logtowrite=
  {
    event,day,hour,minute,second
    };//5-byte log struct

   EEPROM.put(addr,logtowrite);
   addr+= sizeof(logType);
   if(addr>=(EEPROM.length()-10)){addr=0;}
   //manueal reset address, prevent auto wrap-around that might break the format by causing dislocation and mismatch. 
   //something like [01234] [56789] would work, but 012] [34567] ...[1022 1023 would break the interpretation

   //Could use EEPROM.length()-1 address(last byte in eeprom) to store the write head position, so that the log wont overwrite from the start every time
   //But that would make one specific location to run out of life EEPROM.length() times faster?
  
  }

void readLog()
{
      Serial.println("\nLogs\n");
  while(EEPROM.read(readaddr)!=255)
  {
    EEPROM.get(readaddr,logread);
    readaddr+= sizeof(logType);

  Serial.print("Event: ");  Serial.print(logread.event);Serial.print("  At Time:  Day ");Serial.print(logread.day);Serial.print(" |");
  Serial.print(logread.hour);Serial.print(" : ");Serial.print(logread.minute);Serial.print(" : ");Serial.print(logread.second);
          Serial.println(" ");

    }
  
  
  }

/*#include <SPI.h>
#include <SD.h>
File logfile;
void writelog(String logcontent)
{
   //NEED sd.begin(#sspin) , after actually connecting an SD card
   logfile = SD.open("log.txt",FILE_WRITE);
  if(logfile)
  {
    logfile.println(logcontent);
    }
    logfile.close();
  
   
}*/
