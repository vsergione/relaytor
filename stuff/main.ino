// #include <ESP8266_ISR_Timer-Impl.h>
// #include <ESP8266_ISR_Timer.h>
// #include <ESP8266TimerInterrupt.h>
// #include <TimerInterrupt_Generic_Debug.h>
#include <StreamUtils.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// #define TIMER_INTERVAL_MS 1000

// ESP8266Timer ITimer;
LiquidCrystal_I2C lcd(0x27, 20, 4);

//0x27
WiFiUDP ntpUDP;

// By default 'pool.ntp.org' is used with 60 seconds update interval and
// no offset
NTPClient timeClient(ntpUDP,"162.159.200.123");

String ssid,psk,api,key;

const byte MODE_SETUP=0;
const byte MODE_RUN=1;
const byte MODE_DISABLED=2;
const byte MODE_INIT=3;
const byte MODE_DISC=4;

/* Set these to your desired credentials. */
bool ledMode = LOW;

String deviceModeTxt[4] = {"setup","running","disable","init"};
byte deviceMode=MODE_INIT;
String version="1.0.1";
String corsHeader;

byte currentState=0,oldState=-1;

const byte STATE_IDLE=0;
const byte STATE_API_REQ=1;
const byte STATE_LIST_PROJECTS=2;
const byte STATE_DISPLAY_CURRENT_TASK=3;
const byte STATE_UNKNOWN_CARD=4;
const byte STATE_CONFIRM_SELECTION=5;
const byte STATE_CONFIRM_STOP=6;

unsigned long lastConnectTry,lastScreenUpdate; 


void appLoop() {
  switch(currentState)
  {
    case STATE_IDLE:
      stateIdle();
      break;
    // case STATE_API_REQ:
    //   break;
    case STATE_LIST_PROJECTS:
      stateListProjects();
      break;
    case STATE_DISPLAY_CURRENT_TASK:
      displayCurrentProject();
      break;
    case STATE_UNKNOWN_CARD:
      unknownCard();
      break;
    case STATE_CONFIRM_SELECTION:
      confirmSelection();
      break;
    case STATE_CONFIRM_STOP:
      confirmStop();
      break;
  }
}

const byte METHOD_GET = 0;
const byte METHOD_POST = 1;
const byte METHOD_PUT = 2;
const byte METHOD_PATCH = 3;
const byte METHOD_DELETE = 4;

void displayCurrentProject(){}
void unknownCard(){}
void confirmSelection(){}
void confirmStop(){}

void apiReq(String url, byte method, char *headerKeys, char *headerValues, String data, void (*callback)(int, String)) {
  WiFiClient client;
  HTTPClient http;
  http.useHTTP10(true);
  http.begin(client, url);
  http.addHeader("Content-type","application/vnd.api+json");
  
  int respCode;
  switch (method)
  {
    case METHOD_GET:
      respCode = http.GET();
      break;
    case METHOD_POST:
      respCode = http.POST(data);
      break;
    case METHOD_PUT:
      respCode = http.PUT(data);
      break;
    case METHOD_PATCH:
      respCode = http.PATCH(data);
      break;
    case METHOD_DELETE:
      respCode = http.DELETE();
      break;
  }
  if(respCode > 0) {
    String payload = http.getString();
    http.end();
    callback(respCode,payload);
  }
  else {
    http.end();
    callback(-1,"");
  }
}

void stateListProjects()
{
  if(currentState!=oldState) {
    lcd.clear();
  }
}

void stateIdle()
{
  // state entry
  if(oldState!=currentState) {
    // set listen for card
    //attachInterrupt()
    oldState = currentState;
    lcd.clear();
  }


  // state loop
  if(micros()-lastScreenUpdate>100) {
    lcd.setCursor(0,0);
    lcd.print(timeClient.getFormattedTime());
    lastScreenUpdate = micros();
  }
}




bool loadConfig() {
  EEPROM.begin(512);
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  // Serial.println("Configuring access point...");

  StaticJsonDocument<500> doc;
  EepromStream eepromStream(0,256);
  DeserializationError err = deserializeJson(doc, eepromStream);
  if(err.code()==DeserializationError::Ok) {
    ssid = doc["ssid"].as<String>();;
    psk = doc["psk"].as<String>();;
    api = doc["api"].as<String>();;
    key = doc["key"].as<String>();;
    Serial.println("Configuration found");
    return true;
  }

  Serial.println("No configuration found");
  return false;
}


void connectDevice() {
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to SSID "+ssid);
  if(psk=="") {
    WiFi.begin(ssid);
  }
  else {
    WiFi.begin(ssid,psk);
  }
  byte wd = 20;
  while (WiFi.status() != WL_CONNECTED && wd>0) {
    delay(500);
    wd--;
    Serial.print(".");
  }
  
  if(WiFi.status() != WL_CONNECTED) {
    lastConnectTry = micros();
    deviceMode = MODE_DISC;
    return;
  }

  deviceMode = MODE_RUN;
  Serial.println();
  Serial.print("Client IP: ");
  Serial.println(WiFi.localIP());
  timeClient.begin();
  timeClient.update();

  lcd.setCursor(0,0);
  lcd.print("Connected");
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP().toString());
  lcd.setCursor(0,2);
  lcd.print(timeClient.getFormattedTime());
  delay(2000);
}

void loopFunc() {
  
}

void disabledLoop() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Terminal inactiv");
  delay(1000);
}

void disconnectedLoop() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Terminal deconectat");
  delay(10000);
  connectDevice();
}

void setup() {
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Hello, world!");

  Serial.begin(115200);
  if(loadConfig()) {
    connectDevice();
  }
  else {
    deviceMode = MODE_SETUP;
    startSetup();
  }
}

void loop() {
  switch (deviceMode){
    case MODE_DISABLED:
      disabledLoop();
      break;
    case MODE_SETUP:
      setupLoop();
      break;
    case MODE_RUN:
      appLoop();
      break;
    case MODE_DISC:
      disconnectedLoop();
      break;
  }
}