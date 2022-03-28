unsigned long realTimeOffset;//in miliseconds, will add to milis()
extern unsigned long militime;
unsigned long clockSpeedMultiplier = 1;//Faster clock time for dev/demo purpose

void setTime(unsigned long day, unsigned long hour, unsigned long minute,unsigned long csm)
{
  realTimeOffset = minute*(unsigned long)60000   +    hour*(unsigned long)3600000   +   day*(unsigned long)86400000;
  clockSpeedMultiplier=csm;
  }



int hour()
{
 militime= realTimeOffset + millis()*clockSpeedMultiplier;
unsigned long rawhours =  (militime/(unsigned long)3600000);
int hourinday = (int)(rawhours % (unsigned long)24);
return hourinday;
}

int minute()
{
 militime= realTimeOffset + millis()*clockSpeedMultiplier;
unsigned long rawmin = militime/(unsigned long)60000;
int minuteinhour = (int)(rawmin % (unsigned long)60);
return minuteinhour;
}

int second()
{
 militime= realTimeOffset + millis()*clockSpeedMultiplier;
unsigned long rawsec = (militime/(unsigned long)1000);
int secinmin = (int)(rawsec % (unsigned long)60);
return secinmin;
}


int day()
{
   militime= realTimeOffset + millis()*clockSpeedMultiplier;
int rawday = (int)(militime/(unsigned long)86400000);
return rawday;  
  }
