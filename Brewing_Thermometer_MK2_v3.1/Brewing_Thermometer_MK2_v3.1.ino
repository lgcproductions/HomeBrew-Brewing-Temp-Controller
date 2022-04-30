/**
   This is my 'knockoff' version of a inkbird. Originally it was built upon an Arduino Uno.
   MK2 is susequently built upon a Wemos D1 Mini ti give webcontrol.

   EEPROM is handled by a AT24c256 module
   EEPROM Block Locations
   0-5 = Temperature to trigger act 
   6-7 = Delay for temperature
   8-
*/
#include <Wire.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <LiquidCrystal_I2C.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include "customize.h"             // User custimization on defining pins/parameters
#include "index.h"                 // Stores the webpage
//eeprom
#include "extEEPROM.h"
extEEPROM myEEPROM(kbits_256, 1, 32, 0x50);

ESP8266WebServer webserver(80);

LiquidCrystal_I2C lcd(LCDADDRESSE, 16, 2);
int nextUpdate = 0;                       // Variable used to get temp data
int addr = 0;
bool testHeatingState;
bool testCoolingState;


// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(TEMPPROBE);
DallasTemperature sensors(&oneWire);      // Pass our oneWire reference to Dallas Temperature.

//---------------------------------------------------------
void handleRoot() {
  webserver.send(200, "text/html", MAIN_PAGE); //Send web page
}

//---------------------------------------------------------
void handleTEMP() {
  float temp = sensors.getTempCByIndex(0);
  sensors.requestTemperatures();
  String tempValue = String(temp);

  webserver.send(200, "text/plane", tempValue); //Send ADC value only to client ajax request
}

//---------------------------------------------------------
void handleGETTRIGTEMP() {
  int t_t = TRIGTEMP;
  String trig_Value = String(t_t);

  webserver.send(200, "text/plane", trig_Value); //Send Trig Temp to client ajax request
}

//---------------------------------------------------------
IRAM_ATTR void handleTRIGTEMP() {
  String t_value = webserver.arg("trigtempdata");
  TRIGTEMP = t_value.toInt();
  myEEPROM.write(0, TRIGTEMP);
  String s = "<br><br><br><a href='/'><h1 style='text-align: center;' a href='/'>Go Back</h1></a>";
  webserver.send(200, "text/html", s); //Send web page
}

//---------------------------------------------------------
void handleGETTEMPDELAY() {
  int t_d = TEMPDELAY;
  String tempdelay_Value = String(t_d);

  webserver.send(200, "text/plane", tempdelay_Value); //Send Trig Temp to client ajax request
}

//---------------------------------------------------------
IRAM_ATTR void handleTEMPDELAY() {
  String d_value = webserver.arg("tempdelaydata");
  TEMPDELAY = d_value.toInt();
  myEEPROM.write(6, TEMPDELAY);
  String s = "<br><br><br><a href='/'><h1 style='text-align: center;' a href='/'>Go Back</h1></a>";
  webserver.send(200, "text/html", s); //Send web page
}

//---------------------------------------------------------
IRAM_ATTR void handleCOOLING() {
  String COOLINGState = "OFF";
  String c_state = webserver.arg("COOLINGstate"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
  Serial.println(c_state);
  if (c_state == "1")
  {
    digitalWrite(COOLING, LOW); //LED ON
    COOLINGState = "ON"; //Feedback parameter
  }
  else
  {
    digitalWrite(COOLING, HIGH); //LED OFF
    COOLINGState = "OFF"; //Feedback parameter
  }

  webserver.send(200, "text/plane", COOLINGState); //Send web page
}

//---------------------------------------------------------
void sendCOOLING() {
  String x;
  if (testCoolingState == true)
  {
    x = "ON"; //Feedback parameter
  }
  else
  {
    x = "OFF"; //Feedback parameter
  }

  webserver.send(200, "text/plane", x); //Send web page
}

//---------------------------------------------------------
IRAM_ATTR void handleHEATING() {
  String HEATINGState = "OFF";
  String h_state = webserver.arg("HEATINGstate"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
  Serial.println(h_state);
  if (h_state == "1")
  {
    digitalWrite(HEATING, LOW); //LED ON
    HEATINGState = "ON"; //Feedback parameter
  }
  else
  {
    digitalWrite(HEATING, HIGH); //LED OFF
    HEATINGState = "OFF"; //Feedback parameter
  }

  webserver.send(200, "text/plane", HEATINGState); //Send web page
}

//---------------------------------------------------------
void sendHEATING() {
  String x;
  if (testHeatingState == true)
  {
    x = "ON"; //Feedback parameter
  }
  else
  {
    x = "OFF"; //Feedback parameter
  }

  webserver.send(200, "text/plane", x); //Send web page
}

//---------------------------------------------------------

void screen(float x) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Brew Temp:"));
  lcd.print(x);
  lcd.print(F("c"));
  lcd.setCursor(0, 1);
  lcd.print(F("Trigger:"));
  lcd.print(TRIGTEMP);
  lcd.print(F("c +/-"));
  lcd.print(TEMPDELAY);

}

void setup()
{
  Serial.begin(9600);
  Serial.println(F("Sketch:   "));   Serial.println(__FILE__);

  lcd.init();                      // initialize the lcd
  lcd.backlight();
  lcd.print("Loading!");
  //Init ESPBrowser
  Serial.print(F("Connecting to ")); Serial.println(STASSID);
  WiFi.begin(STASSID, STAPSK);
  // Connect to Wifi network.
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500); Serial.print(F("."));
  }
  webserver.on("/", handleRoot);
  webserver.on("/setHEATING", handleHEATING);
  webserver.on("/setCOOLING", handleCOOLING);
  webserver.on("/getCOOLING", sendCOOLING);
  webserver.on("/getHEATING", sendHEATING);
  webserver.on("/readTEMP", handleTEMP);
  webserver.on("/readTRIGTEMP", handleGETTRIGTEMP);
  webserver.on("/readGETTEMPDELAY", handleGETTEMPDELAY);
  webserver.on("/action_page", handleTRIGTEMP);
  webserver.on("/temp_delay", handleTEMPDELAY);
  webserver.begin();
  Serial.println();
  Serial.println(F("ESPBrowser initialized"));
  Serial.print(F("IP Address: "));
  Serial.println(WiFi.localIP());
  sensors.begin();                // Start up the library for temp sensor

  //initiate relay pins
  pinMode(HEATING, OUTPUT);
  pinMode(COOLING, OUTPUT);
  digitalWrite(HEATING, HIGH);
  digitalWrite(COOLING, HIGH);

  //Initialize EEPROM
  byte i2cStat = myEEPROM.begin(myEEPROM.twiClock400kHz);
  if ( i2cStat != 0 ) {
    Serial.println(F("I2C load issue"));
  }
  TRIGTEMP = myEEPROM.read(0);
  TEMPDELAY = myEEPROM.read(6);

  delay(1000); //Give a little time to load
  lcd.clear();
}

//---------------------------------------------------------
void loop()
{
  float temp = sensors.getTempCByIndex(0);   // Variable to store temp. Why "byIndex"?
  int masterTemp;                            // Used to store both the trigtemp and offset
  bool hasRun = false;
  // You can have more than one DS18B20 on the same bus. 0 refers to the first IC on the wire
  int ms = millis();
  if (ms >= nextUpdate)
  {
    sensors.requestTemperatures();            // Send the command to get temperature readings
    screen(temp);
    nextUpdate = ms + 5000;
    masterTemp = TRIGTEMP + TEMPDELAY;

    if (temp <= TRIGTEMP - TEMPDELAY) {
      digitalWrite(HEATING, LOW);
      testHeatingState = true;
    }
    else if (temp >= TRIGTEMP + TEMPDELAY) {
      digitalWrite(COOLING, LOW);
      testCoolingState = true;
    }
    else {
      digitalWrite(HEATING, HIGH);
      testHeatingState = false;
      digitalWrite(COOLING, HIGH);
      testCoolingState = false;
    }

    //Next four lines is for debugging
    Serial.println(masterTemp);
    Serial.print("Temp - ");                  // Print the current temp to console
    Serial.println(temp);
    Serial.println(TRIGTEMP);
  }

  webserver.handleClient();                   //Webpage

  //delay(1000); //keeps the code from crashing.
}


//===========================================================================
//================================== Debug ==================================
//===========================================================================
//
// you can use this code to debug/test certain parts of the ""
//
//===========================================================================

void relayTest() {
  digitalWrite(HEATING, LOW);
  Serial.println("On");
  delay(1000);
  digitalWrite(HEATING, HIGH);
  Serial.println("off");
  delay(1000);
}

void tempSensorTest() {
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  /********************************************************************/
  Serial.print(" Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperature readings
  Serial.println("DONE");
  /********************************************************************/
  Serial.print("Temperature is: ");
  Serial.println(sensors.getTempCByIndex(0));
  // Why "byIndex"?
  // You can have more than one DS18B20 on the same bus.
  // 0 refers to the first IC on the wire
}
