/***********************************************
 * 
 * METEORINO (c)2018 Giovanni Bernardo
 * 
 * https://www.settorezero.com
 * https://www.instagram.com/cyb3rn0id
 * 
 * Libraries to install first:
 * - ESP8266 and ESP32 Oled Driver for SSD1306 display (by Daniel Eichorn, Fabrice Weinberg)
 * - Time (by Michael Margolis)
 * - EasyNTPClient (by Harsha Alva)
 * - ArduinoJSON (by Benoit Blanchon)
 * 
 * Thanks to:
 * https://github.com/RoboUlbricht/arduinoslovakia/blob/master/openweathermap/esp8266/oled_09_weather_parsed/oled_09_weather_parsed.ino
 * https://techtutorialsx.com/2018/03/17/esp32-arduino-getting-weather-data-from-api/
 * 
 * 
 ***********************************************/

#define METEORINO_VERSION "1.00B"

#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
#include <EasyNTPClient.h>
#include <TimeLib.h>
#include <ArduinoJson.h>
#include "SSD1306Wire.h"
#include "OLEDDisplayUi.h"

#include "myData.h"
#include "myFonts.h"
#include "myMeteo.h"
#include "calendar.h"

SSD1306Wire display(0x3c, 5, 4);
OLEDDisplayUi ui (&display);

WiFiUDP udp;
EasyNTPClient ntpClient(udp, NTPServer, (NTPAddHours*60*60));

// Display dimensions
int screenW = 128;
int screenH = 64;
int CenterX = screenW/2;
int CenterY = screenH/2;

// values obtained from OpenWeatherMap JSon
struct myWeather 
  {
  double temperature;
  int humidity;
  int pressure;
  double wind;
  int weatherID;
  };
  
myWeather weatherNow;

// OpenWeatherMap values transformed in string
char myTemp[6];
char myHumi[6];
char myPres[6];
char myCity[64];
char myWeather[30];

// used for weather refresh every half hour
bool tickWeatherFirst30=false;
bool tickWeatherSecond30=false;

// used for time refresh once a day
int8_t prevDay=-1;

/***********************************************************************************************************
 * 
 * FUNCTIONS
 *  
 ***********************************************************************************************************/
// utility function for digital clock display: prints leading 0
String twoDigits(int digits)
  {
  if(digits < 10) 
    {
    String i = '0'+String(digits);
    return i;
    }
  else 
    {
    return String(digits);
    }
  }

// simply print error message on serial. Since this is used very often, better write a function
void errorMessage(bool retry)
  {
  Serial.print("ERROR");
  if (retry) // it's an error that can be resolved next time
    {
    Serial.println(". I'll retry next time");  
    }
  else
    {
    Serial.println();  
    }
  }
  
// Update weather informations once every half hour
// I'll set the updated flag even if the connection gives error since sometimes openweathermap
// does not respond for long times and continuous retries will slow down the entire operations.
// Is not a problem since the free account doesn't update so often (<2 hours)
void UpdateWeather(void)
  {
  // refresh weather informations every 30 minutes
  if (minute()<30)
    {
    if (!tickWeatherFirst30) // I haven't refreshed weather infos in the first 30 minutes of this hour
        {
        Serial.println("First half hour. Getting weather informations");
        tickWeatherFirst30=true;
        tickWeatherSecond30=false;
        if (!GetWeather())
          {
          errorMessage(true); 
          }
        }
    }
  else // minute() is >30
    {
    if (!tickWeatherSecond30) // I haven't refreshed weather infos in the second 30 minutes of this hour
      {
      Serial.println("Second half hour. Getting weather informations");
      tickWeatherFirst30=false;
      tickWeatherSecond30=true; 
       if (!GetWeather())
          {
          errorMessage(true);
          }   
        }
    }  
  }

// Update system time from NTP server once a day
void UpdateTime(void)
  {
  // today is no yesterday!
  if (weekday() != prevDay)
    {
    Serial.print("A day has passed, updating time...");
    bool t=GetTime();
    if (t)
      {
      prevDay=weekday(); // this will prevent further time updating for today
      Serial.println("OK");  
      }
    else
      {
       errorMessage(true); 
      }
    }
  }

// connects to NTP server for time updating
// returns 0 in case of error, 1 in case of success and update done
bool GetTime(void)
  {
  unsigned long t=ntpClient.getUnixTime();
  if (t>0)
    {
    setTime(t);
    return true;    
    }
  else
    {
    return false;  
    }
  }

// connects to OpenWeatherMapp server for weather infos updating
// returns 0 in case of error, 1 in case of success and update done
bool GetWeather() 
  {
  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  String line;
  Serial.print("Connecting to ");
  Serial.println(OWMHost);
  if (!client.connect(OWMHost, OWMPort)) 
    {
    errorMessage(true);
    return false;
    }

  Serial.print("requesting URL: ");
  Serial.println(OWMParams);

  client.print(String("GET ") + OWMParams + " HTTP/1.1\r\n" +
               "Host: " + OWMHost + "\r\n" +
               "User-Agent: ESP32\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("Request sent");

  line = client.readStringUntil('\n');
  if (line != "HTTP/1.1 200 OK\r") 
    {
    errorMessage(true);
    Serial.println("Server response: ");
    Serial.println(line);
    return false;
    }

  if (client.find("\r\n\r\n")) 
    {
    DynamicJsonBuffer jsonBuffer(4096);
    JsonObject& root = jsonBuffer.parseObject(client);

    // https://openweathermap.org/current
    
    // city name
    const char* name = root["name"];
    strcpy(myCity, name);

    // temperature
    weatherNow.temperature = root["main"]["temp"];
    dtostrf(weatherNow.temperature, 2, 1, myTemp); // only 1 decimal

    // relative humidity
    weatherNow.humidity = root["main"]["humidity"];
    dtostrf(weatherNow.humidity, 2, 0, myHumi); // humidity has no decimals

    // weather detailed description
    const char* description = root["weather"][0]["description"];
    strcpy(myWeather, description);

    // id code of weather conditions, full list on:
    // https://openweathermap.org/weather-conditions
    weatherNow.weatherID = root["weather"][0]["id"];

    //weatherNow.pressure = root["main"]["pressure"];
    //weatherNow.wind = root["wind"]["speed"];

    Serial.println("OK. Weather informations updated");
    return true;
    }
  }

// returns easter date
void getEasterDate(int yr, uint8_t *p)
  {
  uint8_t a,b,c,d,e,da,mo;
  
  a=yr%19;
  b=yr%4; 
  c=yr%7;
  
  d=((19*a)+24)%30; 
  e=((2*b)+(4*c)+(6*d)+5)%7; 
  
  if ((d+e)<10)
    {
    da=d+e+22;
    mo=3;
    }
  else
    {
    da=d+e-9;
    mo=4;
    }
    
  if ((da==26) && (mo==4))
    {
    da=19;
    mo=4;
    }
  else if ((da==25) && (mo=4) && (d==28) && (e==6) && (a>10))
    {
    da=18;
    mo=4;
    }
    
  p[0]=da; // easter day
  p[1]=mo; // easter month
  }

/***********************************************************************************************************
 * 
 * FRAMES to be showed on the display with a sliding effect
 *  
 ***********************************************************************************************************/
// digital clock
void digitalClockFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) 
  {
  UpdateWeather();
  UpdateTime();
      
  String timenow=String(hour())+":"+twoDigits(minute())+":"+twoDigits(second());
  String datenow=twoDigits(day())+"/"+twoDigits(month())+"/"+String(year());
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  // time
  display->setFont(Roboto_Black_24);
  display->drawString(CenterX+x, 0, timenow );
  // day of week
  display->setFont(Lato_Black_16);
  display->drawString(CenterX+x, 23, DayOfWeek[weekday()-1]);
  // date
  display->setFont(Roboto_Black_24);
  display->drawString(CenterX+x, 37, datenow);
  }
  
// temperature and humidity
void tempHumidityFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) 
  {
  String Humidity="H: "+String(myHumi)+"%";
  String Temperature="T: "+String(myTemp)+"°C"; // this font has grade symbol! wow!
  // humidity
  display->setFont(Roboto_Black_24);
  display->drawString(CenterX+x, 2, Humidity);
  // temperature
  display->setFont(Roboto_Black_24);
  display->drawString(CenterX+x, 33, Temperature);
  }

// icon of generic weather conditions and string with detailed description
void weatherFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) 
  {
  String s="";
  // weatherID is an integer made by 3 numbers, first (most-left) number
  // identifies a general group, for showing icon I'll use only the group
  int weatherGroup=weatherNow.weatherID/100;
    /*
     * https://openweathermap.org/weather-conditions
     * 2xx = thunderstorm (tempesta)
     * 3xx = drizzle (pioggerellina)
     * 5xx = rain (pioggia)
     * 6xx = snow (neve)
     * 7xx = mist/fog/dust ecc (foschia,nebbia)
     * 800 = clear sky (cielo limpido)
     * 80x = clouds (nuvole)
     */
  // I'll use a font that has weather icons instead of letter glyph
  // so the letter "P" will show a cloud with lightning instead of a P
  // and so on...
  // http://www.alessioatzeni.com/meteocons/
  switch(weatherGroup)
    {
    case 2: // thunderstorm
      s="P";
      break;
    case 3: // drizzle
      s="Q";
      break;
    case 5: // rain
      s="R";
      break;
    case 6: // snow
      s="W";
      break;
    case 7: // mist
      s="J";
      break;
    case 8: // clear sky or clouds
      if (weatherNow.weatherID>800) // clouds
        {
        s="H";
        }
      else
        { // 800 is clear sky
        s="B";
        }
    break;
    }
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  // city
  display->setFont(ArialMT_Plain_10);
  display->drawString(CenterX+x, 0, String(myCity));
  // weather conditions icon 
  display->setFont(Meteocons_Regular_40); // this font has an height of 42pixels
  display->drawString(CenterX+x, 11, s);
  // detailed description of the weather, the font used will assure that
  // the longest sentence will fit the 128pixel row
  display->setFont(ArialMT_Plain_10);
  display->drawString(CenterX+x, 53, String(myWeather));
  }

// daily Event: saint of the day, anniversary, birthday... ecc
// to set-up in calendar.h
void dailyEventFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) 
  {
    // obtain easter date for this year
   uint8_t ed[2];
   getEasterDate(year(),ed);

   display->setTextAlignment(TEXT_ALIGN_CENTER);
   display->setFont(Roboto_Black_24);

   if (showEaster && (day()==ed[0]) && (month()==ed[1]))
      {
      // today is easter and I want to show it
      display->drawStringMaxWidth(CenterX+x, 0, 128, eas);
      }
    else
      {
      int po=startIndex[month()-1]+day()-1;
      // daily event
      display->drawStringMaxWidth(CenterX+x, 0, 128, String(dp[po]));
      }
  }

// separate waste collection
void wasteCollectionFrame(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) 
  {
   // separate waste collection indication
    int8_t n=0;
    display->setFont(ArialMT_Plain_10);
    display->drawStringMaxWidth(CenterX+x, 0, 128, swc);
    // sentence for tomorrow, big
    display->setFont(Lato_Black_16);
    weekday()==7?n=0:n=weekday();
    display->drawStringMaxWidth(CenterX+x, 12, 128, tom+String(fw[n]));
    // sentence for today, small
    display->setFont(ArialMT_Plain_10);
    n=weekday()-1;
    display->drawStringMaxWidth(CenterX+x, 46, 128, tod+String(fw[n]));
  }
  
FrameCallback frameArray[] = {digitalClockFrame,dailyEventFrame,wasteCollectionFrame,tempHumidityFrame,weatherFrame};
uint8_t frameNumber=5;

  
/***********************************************************************************************************
 * 
 * SETUP AND MAIN program
 *  
 ***********************************************************************************************************/
void setup() 
  {
  Serial.begin(9600);
  
  Serial.println();
  Serial.println();
  Serial.print("Meteorino ");
  Serial.println(String(METEORINO_VERSION));
  Serial.println("Copyright 2018 Giovanni Bernardo (CyB3rn0id)");
  Serial.println("https://www.settorezero.com");
  Serial.println();
  
  Serial.print("Init display for boot rendering...");
  display.init();
  display.setFont(ArialMT_Plain_10);
  Serial.println("OK");
  
  Serial.print("Connecting to WiFi SSID: ");
  Serial.print(ssid);
  Serial.print("...");
  
  display.drawString(0,0,"Meteorino "+String(METEORINO_VERSION));
  display.drawString(0, 10, "Connecting to:");
  display.drawString(0, 20, String(ssid));
  display.display();
  
  char retries=0; // number of WiFi connection retries
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
    {
	  Serial.print(".");
	  delay(300);
	  retries++;
	  if (retries>10) // to many retries for my taste!
      {
      display.drawString(0, 30, "ERROR! Check SSID/PSW");
      display.display();
      errorMessage(false);
      Serial.println("System Halted - Please check WiFi Settings and/or WiFi availability, then reset");
      while(1); // stay blocked forever  
      }
	  }
  display.drawString(0, 30, "Connected!");
  display.display();
  Serial.println("OK");
  delay(1000);
  
  Serial.print("Getting NTP Time...");
  display.drawString(0, 40, "Getting NTP Time");
  display.display();

  // Time Updating success is important!
  // if updating doesn't get right, system will not start
  if (GetTime())
    {
    display.drawString(0, 50, "OK!");
    display.display();
    Serial.println("OK");
    prevDay=weekday(); // this will prevent further time updating for today
    }
  else
    {
    display.drawString(0, 50, "NTP ERROR");
    display.display();
    errorMessage(false);
    Serial.println("System Halted - Please reset");
    while(1); // stay blocked forever 
    }
  
  display.display();
  delay(1000);

  Serial.print("Init Oled User Interface...");
  // The ESP is capable of rendering 60fps in 80Mhz mode
  // but that won't give you much time for anything else
  // run it in 160Mhz mode or just set it to 30 fps
  ui.setTargetFPS(60);
  ui.disableAllIndicators();
  ui.enableAutoTransition();
  ui.setTimePerFrame(TIMEFRAMESHOW);
  ui.setTimePerTransition(TIMETRANSITION);
  // You can change the transition that is used
  // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_UP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);
  //display.flipScreenVertically(); // uncomment this if you want rotate display by 180 degrees
  ui.setFrames(frameArray, frameNumber);  
  // Initialising the UI will init the display too.
  ui.init();
  Serial.println("OK");
  Serial.println("Setup Finished");
  Serial.println("Meteorino Started");
  Serial.println();
  }

void loop() 
  {
  int remainingTimeBudget = ui.update();
  if (remainingTimeBudget > 0) 
      {
      // functions will update weather infos and time only if needed
      // (weather every half hour, time once a day)
      //delay(remainingTimeBudget);
      }
  }
