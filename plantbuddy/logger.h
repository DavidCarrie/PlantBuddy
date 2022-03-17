#include <SPI.h>
#include <SD.h>
File logfile;
void writelog(String logcontent)
{
   logfile = SD.open("log.txt",FILE_WRITE);
  if(logfile)
  {
    logfile.println(logcontent);
    }
    logfile.close();
  
   
}
