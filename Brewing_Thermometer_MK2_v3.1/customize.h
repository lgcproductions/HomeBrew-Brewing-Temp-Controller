/**
 * Edit this file with Wifi details and pins used ect.
 * 
 * GPIO pin definitions are used otherwise the sketch will not work using the Dx pin
 * definitions. 
 */

//===========================================================================
//============================= Getting Started =============================
//===========================================================================
// 
// Amend the wifiName and WifiPass with your own WiFi details.
//
//===========================================================================
#define STASSID "ImMrWifiLookAtMe"
#define STAPSK  "ts2Wygkdsqky"

//===========================================================================
//============================= Heating/Cooling =============================
//===========================================================================
// 
// Heating and Cooling relays
//
//===========================================================================
#define HEATING 12
#define COOLING 14

//===========================================================================
//============================= Miscellaneous ===============================
//===========================================================================
// 
// Using a DS18B20 temperature probe.
//
//===========================================================================

#define TEMPPROBE 2                      //Pin the temp probe is connected to
#define LCDADDRESSE 0x3E                 //Address for the LCD
int TRIGTEMP = 25;                       //Temp you want to trig heating
int TEMPDELAY = 3;
