#include <Arduino.h>
#line 1 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
#include <StreamUtils.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#ifndef APSSID
#define APSSID "ESPap"
#define APPSK  "thereisnospoon"
#endif

ESP8266WebServer server(80);
bool ledStatus;
/**
 * @brief Construct a new Setup object
 * 
 * @param srv 
 */
#line 20 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
void startSetup();
#line 43 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
void setupLoop();
#line 58 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
String encType(int thisType);
#line 81 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
void getRoot();
#line 96 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
void connectWifi();
#line 124 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
void connectionStatus();
#line 164 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
void testApi();
#line 213 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
void toggleLed();
#line 219 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
void listSSIDs();
#line 245 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
void reset();
#line 250 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
void saveConfig();
#line 57 "/home/vsergiu/work/relaytor/stuff/main.ino"
void appLoop();
#line 89 "/home/vsergiu/work/relaytor/stuff/main.ino"
void displayCurrentProject();
#line 90 "/home/vsergiu/work/relaytor/stuff/main.ino"
void unknownCard();
#line 91 "/home/vsergiu/work/relaytor/stuff/main.ino"
void confirmSelection();
#line 92 "/home/vsergiu/work/relaytor/stuff/main.ino"
void confirmStop();
#line 94 "/home/vsergiu/work/relaytor/stuff/main.ino"
void apiReq(String url, byte method, char *headerKeys, char *headerValues, String data, void (*callback)(int, String));
#line 131 "/home/vsergiu/work/relaytor/stuff/main.ino"
void stateListProjects();
#line 138 "/home/vsergiu/work/relaytor/stuff/main.ino"
void stateIdle();
#line 160 "/home/vsergiu/work/relaytor/stuff/main.ino"
bool loadConfig();
#line 184 "/home/vsergiu/work/relaytor/stuff/main.ino"
void connectDevice();
#line 222 "/home/vsergiu/work/relaytor/stuff/main.ino"
void loopFunc();
#line 226 "/home/vsergiu/work/relaytor/stuff/main.ino"
void disabledLoop();
#line 233 "/home/vsergiu/work/relaytor/stuff/main.ino"
void disconnectedLoop();
#line 241 "/home/vsergiu/work/relaytor/stuff/main.ino"
void setup();
#line 258 "/home/vsergiu/work/relaytor/stuff/main.ino"
void loop();
#line 20 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
void startSetup() {
    pinMode(LED_BUILTIN,OUTPUT);
    Serial.begin(115200);
    WiFi.softAP(APSSID,APPSK);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP: ");
    Serial.println(myIP);

    server.begin();
    Serial.println("HTTP srv on");
    
    server.on("/", getRoot);
    server.on("/ssids",listSSIDs);
    server.on("/connect",connectWifi);
    server.on("/connstatus",connectionStatus);
    server.on("/testapi",testApi);
    server.on("/save",saveConfig);
    server.on("/tglled",toggleLed);
    server.on("/reset",reset);
    Serial.println("end start setup");

}

void setupLoop() {
    server.handleClient();
}



void(* resetFunc) (void) = 0;//declare reset function at address 0


/**
 * @brief 
 * 
 * @param thisType 
 * @return String 
 */
String encType(int thisType) {
    // read the encryption type and print out the name:
    switch (thisType) {
        case ENC_TYPE_WEP:
        return "WEP";
        case ENC_TYPE_TKIP:
        return "WPA";
        case ENC_TYPE_CCMP:
        return "WPA2";
        case ENC_TYPE_NONE:
        return "None";
        case ENC_TYPE_AUTO:
        return "Auto";
    }
    return "Unknown";
}

/**
 * @brief 
 * 
 * @param mode 
 * @param version 
 */
void getRoot() {
    Serial.print("getRoot ");
    //server.sendHeader(corsHeaderTxt,"*");
    StaticJsonDocument<128> doc;
    doc["mode"] = "setup";
    String page;
    serializeJson(doc,page);
    server.send(200,"application/json",page);
    Serial.println("serverd");
}

/**
 * @brief connects to WiFI using provided SSID and WPA Key
 * 
 */
void connectWifi() {
    Serial.print("connectWifi ");


    String ssid="",psk="";

    for (uint8_t i = 0; i < server.args(); i++) { 
        if(server.argName(i)=="ssid") {
            Serial.print("SSID ");
            Serial.println(server.arg(i));
            ssid = server.arg(i);
        }
        if(server.argName(i)=="psk") {
            Serial.print("PSK ");
            Serial.println(server.arg(i));
            psk = server.arg(i);
        }
    }
    Serial.println(psk + "/" + ssid);
    WiFi.begin(ssid,psk);
    server.send(204);
}


/**
 * @brief return connection status
 * 
 */
void connectionStatus() {
    Serial.print("connectionStatus ");

    byte status = WiFi.status();
    String statusTxt;
    
    switch (status) {
        case WL_DISCONNECTED:
        statusTxt = F("WL_DISCONNECTED");
        break;
        case WL_IDLE_STATUS:
        statusTxt = F("WL_IDLE_STATUS");
        break;
        case WL_NO_SSID_AVAIL:
        statusTxt = F("WL_NO_SSID_AVAIL");
        break;
        case WL_SCAN_COMPLETED:
        statusTxt = F("WL_SCAN_COMPLETED");
        break;
        case WL_CONNECTED:
        statusTxt = F("WL_CONNECTED");
        break;
        case WL_CONNECT_FAILED:
        statusTxt = F("WL_CONNECT_FAILED");
        break;
        case WL_CONNECTION_LOST:
        statusTxt = F("WL_CONNECTION_LOST");
        break;
        case WL_WRONG_PASSWORD:
        statusTxt = F("WL_WRONG_PASSWORD");
        break;
        default:
        statusTxt = F("UNKNOWN");
        break;
    }
    server.send(200, "text/plain",statusTxt);
    Serial.println("served");

}

void testApi() {
    Serial.print("testApi ");

    String api="",key="";

    for (uint8_t i = 0; i < server.args(); i++) { 
        if(server.argName(i)=="api") {
            api = server.arg(i);
        }
        if(server.argName(i)=="key") {
            key = server.arg(i);
        }
    }

    WiFiClient client;
    HTTPClient http;

    if (!http.begin(client, api)) {  
        server.send(500, "text/html","Unable to connect");
        api = "";
        key = "";
    }

    int httpCode = http.GET();
    if (httpCode > 0) {
        DynamicJsonDocument doc(2048);
        String payload = http.getString();
        DeserializationError err = deserializeJson(doc, payload);
        if (err.code()!=DeserializationError::Ok) {
            Serial.print(F("API response - deserializeJson() failed: "));
            Serial.println(err.f_str());
            server.send(400, "text/plain",payload);
            return;
        }
        server.send(200, "text/plain",doc["message"].as<char*>());
        Serial.println(doc["message"].as<char*>());
    } else {
        api = "";
        key = "";
        Serial.print(F("API not working"));
        server.send(500, "text/plain","API not working");
    }

    http.end();
    client.stop();
    Serial.println("served");

}

void toggleLed() {
    ledStatus = !ledStatus;
    digitalWrite(LED_BUILTIN,ledStatus);
    server.send(200, "text/plain",!ledStatus ? "HIGH" : "LOW");
}

void listSSIDs() {
    Serial.print("listSSIDs ");

    StaticJsonDocument<1024> doc;
    JsonArray ssids = doc.to<JsonArray>();
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; i++)
    {
        //StaticJsonDocument<100> item;
        //item['ssid'] = WiFi.SSID(i);
        //item['rssi'] = WiFi.RSSI(i);
        char buf[20];
        ltoa(WiFi.RSSI(i), buf, 10);
        ssids.add(WiFi.SSID(i)+"|"+buf+"|"+encType(WiFi.encryptionType(i)));
        Serial.print(WiFi.SSID(i));
        Serial.print(WiFi.RSSI(i));
        Serial.println(encType(WiFi.encryptionType(i)));
        
    }
    String page;
    serializeJson(doc,page);
    server.send(200, "application/json",page);
    Serial.println("served");
}


void reset() {
    server.send(204);
    resetFunc();
}

void saveConfig() {
    String ssid="",psk="",api="",key="";
    char ch_ssid[50], ch_psk[50], ch_api[150], ch_key[50];

    for (uint8_t i = 0; i < server.args(); i++) { 
        if(server.argName(i)=="ssid") {
            Serial.print("SSID ");
            Serial.println(server.arg(i));
            ssid = server.arg(i);
        }
        if(server.argName(i)=="psk") {
            Serial.print("PSK ");
            Serial.println(server.arg(i));
            psk = server.arg(i);
        }
        if(server.argName(i)=="api") {
            Serial.print("API ");
            Serial.println(server.arg(i));
            api = server.arg(i);
        }
        if(server.argName(i)=="key") {
            Serial.print("KEY ");
            Serial.println(server.arg(i));
            key = server.arg(i);
        }
    }

    EepromStream eepromStream(0, 256);
    StaticJsonDocument<256> config;

    ssid.toCharArray(ch_ssid,50);
    psk.toCharArray(ch_psk,50);
    api.toCharArray(ch_api,50);
    key.toCharArray(ch_key,50);

    config["ssid"] = ch_ssid;
    config["psk"] = ch_psk;
    config["api"] = ch_api;
    config["key"] = ch_key;
    serializeJson(config, eepromStream);
    eepromStream.flush();

    char output[256];
    serializeJson(config,output);
    Serial.println(output);
    server.send(200, "text/plain",output);
}




#line 1 "/home/vsergiu/work/relaytor/stuff/main.ino"
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
