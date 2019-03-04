/***********************************************
 * 
 * METEORINO (c)2018 Giovanni Bernardo
 * 
 *  * https://www.settorezero.com
 * https://www.facebook.com/settorezero
 * 
 * Parameter List
 * Edit this file putting in your data
 * 
 ***********************************************/

// Wi-Fi Settings
const char* ssid="[YOUR SSID]";
const char* password="[YOUR PASSPHRASE]";

// OpenWeatherMap Settings
// you must subscribe to http://www.openweathermap.org
// to obtain a free AppID:
const char* OWMAppID="[YOUR OPENWEATHERMAP APPID]"; // your AppID
// City ID
// you can obtain CityID looking into the JSON file (city.list.json.gz) downloadable at:
// http://bulk.openweathermap.org/sample/
const char* OWMCityID = "6541869"; // your city ID
// Call to OpenWeatherMap will return a JSON file, example:
// http://api.openweathermap.org/data/2.5/weather?id=6541869&units=metric&APPID=[YOUR OPENWEATHERMAP APPID]

// NTP Server
#define NTPAddHours 2 // hours to add/subract from NTP Time 

// other settings
#define TIMEFRAMESHOW 3500 // every frame showed for 3.5 seconds
#define TIMETRANSITION 300 // every frame transition will last 0.3 seconds

/*****************************************************************************************************************
 * DO NOT CHANGE NOTHING BELOW THIS LINE!
 *****************************************************************************************************************/
// OpenWeatherMap standard Parameters
const char* OWMHost = "api.openweathermap.org";
const int OWMPort = 443;
const char* OWMUrl = "/data/2.5/weather";
String OWMParams = String(OWMUrl) +
               "?id=" + OWMCityID +
               "&units=metric" +
               "&APPID=" + OWMAppID;

// NTP standard parameters
// (or change the server if you know what you're doing)
const char* NTPServer="it.pool.ntp.org";
