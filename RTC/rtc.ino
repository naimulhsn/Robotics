// Date and time functions using a DS3231 RTC connected via I2C and Wire lib

#include <Wire.h>
#include "Sodaq_DS3231.h"

char weekDay[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

//year, month, date, hour, min, sec and week-day(starts from 0 and goes to 6)
//writing any non-existent time-data may interfere with normal operation of the RTC.
//Take care of week-day also.
DateTime dt(2022, 1, 22, 12, 25, 0, 6);

void setup ()
{
    Serial.begin(57600);
    Wire.begin();
    if (! rtc.begin()) {
      Serial.println("Couldn't find RTC");
      while (1);
    }
    rtc.setDateTime(dt); //Adjust date-time as defined 'dt' above

    
    /*
    if (rtc.lostPower()) {
      Serial.println("RTC lost power, lets set the time!");
    
    // Comment out below lines once you set the date & time.
      // Following line sets the RTC to the date & time this sketch was compiled
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    
      // Following line sets the RTC with an explicit date & time
      // for example to set January 27 2017 at 12:56 you would call:
      // rtc.adjust(DateTime(2017, 1, 27, 12, 56, 0));
    } 
    */
  
}

void loop ()
{
    DateTime now = rtc.now(); //get the current date-time
    int y = now.year();
    Serial.print(y, DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.date(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.print("   ------- ");
    Serial.print(weekDay[now.dayOfWeek()]);
    Serial.println();
    delay(1000);
}
