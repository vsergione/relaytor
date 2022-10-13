# 1 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
# 2 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 2
# 3 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 2
# 4 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 2
# 5 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 2
# 6 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 2
# 7 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 2






ESP8266WebServer server(80);
bool ledStatus;
/**
 * @brief Construct a new Setup object
 * 
 * @param srv 
 */
void startSetup() {
    pinMode(2,0x01);
    Serial.begin(115200);
    WiFi.softAP("ESPap","thereisnospoon");
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
        statusTxt = ((reinterpret_cast<const __FlashStringHelper *>(
# 132 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 3
                   (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "10_setup.ino" "." "132" "." "136" "\", \"aSM\", @progbits, 1 #"))) = (
# 132 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
                   "WL_DISCONNECTED"
# 132 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 3
                   ); &__pstr__[0];}))
# 132 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
                   )));
        break;
        case WL_IDLE_STATUS:
        statusTxt = ((reinterpret_cast<const __FlashStringHelper *>(
# 135 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 3
                   (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "10_setup.ino" "." "135" "." "137" "\", \"aSM\", @progbits, 1 #"))) = (
# 135 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
                   "WL_IDLE_STATUS"
# 135 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 3
                   ); &__pstr__[0];}))
# 135 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
                   )));
        break;
        case WL_NO_SSID_AVAIL:
        statusTxt = ((reinterpret_cast<const __FlashStringHelper *>(
# 138 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 3
                   (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "10_setup.ino" "." "138" "." "138" "\", \"aSM\", @progbits, 1 #"))) = (
# 138 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
                   "WL_NO_SSID_AVAIL"
# 138 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 3
                   ); &__pstr__[0];}))
# 138 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
                   )));
        break;
        case WL_SCAN_COMPLETED:
        statusTxt = ((reinterpret_cast<const __FlashStringHelper *>(
# 141 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 3
                   (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "10_setup.ino" "." "141" "." "139" "\", \"aSM\", @progbits, 1 #"))) = (
# 141 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
                   "WL_SCAN_COMPLETED"
# 141 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 3
                   ); &__pstr__[0];}))
# 141 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
                   )));
        break;
        case WL_CONNECTED:
        statusTxt = ((reinterpret_cast<const __FlashStringHelper *>(
# 144 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 3
                   (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "10_setup.ino" "." "144" "." "140" "\", \"aSM\", @progbits, 1 #"))) = (
# 144 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
                   "WL_CONNECTED"
# 144 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 3
                   ); &__pstr__[0];}))
# 144 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
                   )));
        break;
        case WL_CONNECT_FAILED:
        statusTxt = ((reinterpret_cast<const __FlashStringHelper *>(
# 147 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 3
                   (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "10_setup.ino" "." "147" "." "141" "\", \"aSM\", @progbits, 1 #"))) = (
# 147 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
                   "WL_CONNECT_FAILED"
# 147 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 3
                   ); &__pstr__[0];}))
# 147 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
                   )));
        break;
        case WL_CONNECTION_LOST:
        statusTxt = ((reinterpret_cast<const __FlashStringHelper *>(
# 150 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 3
                   (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "10_setup.ino" "." "150" "." "142" "\", \"aSM\", @progbits, 1 #"))) = (
# 150 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
                   "WL_CONNECTION_LOST"
# 150 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 3
                   ); &__pstr__[0];}))
# 150 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
                   )));
        break;
        case WL_WRONG_PASSWORD:
        statusTxt = ((reinterpret_cast<const __FlashStringHelper *>(
# 153 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 3
                   (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "10_setup.ino" "." "153" "." "143" "\", \"aSM\", @progbits, 1 #"))) = (
# 153 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
                   "WL_WRONG_PASSWORD"
# 153 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 3
                   ); &__pstr__[0];}))
# 153 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
                   )));
        break;
        default:
        statusTxt = ((reinterpret_cast<const __FlashStringHelper *>(
# 156 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 3
                   (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "10_setup.ino" "." "156" "." "144" "\", \"aSM\", @progbits, 1 #"))) = (
# 156 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
                   "UNKNOWN"
# 156 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 3
                   ); &__pstr__[0];}))
# 156 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
                   )));
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
            Serial.print(((reinterpret_cast<const __FlashStringHelper *>(
# 193 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 3
                        (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "10_setup.ino" "." "193" "." "145" "\", \"aSM\", @progbits, 1 #"))) = (
# 193 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
                        "API response - deserializeJson() failed: "
# 193 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 3
                        ); &__pstr__[0];}))
# 193 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
                        ))));
            Serial.println(err.f_str());
            server.send(400, "text/plain",payload);
            return;
        }
        server.send(200, "text/plain",doc["message"].as<char*>());
        Serial.println(doc["message"].as<char*>());
    } else {
        api = "";
        key = "";
        Serial.print(((reinterpret_cast<const __FlashStringHelper *>(
# 203 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 3
                    (__extension__({static const char __pstr__[] __attribute__((__aligned__(4))) __attribute__((section( "\".irom0.pstr." "10_setup.ino" "." "203" "." "146" "\", \"aSM\", @progbits, 1 #"))) = (
# 203 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
                    "API not working"
# 203 "/home/vsergiu/work/relaytor/stuff/10_setup.ino" 3
                    ); &__pstr__[0];}))
# 203 "/home/vsergiu/work/relaytor/stuff/10_setup.ino"
                    ))));
        server.send(500, "text/plain","API not working");
    }

    http.end();
    client.stop();
    Serial.println("served");

}

void toggleLed() {
    ledStatus = !ledStatus;
    digitalWrite(2,ledStatus);
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
# 1 "/home/vsergiu/work/relaytor/stuff/main.ino"
// #include <ESP8266_ISR_Timer-Impl.h>
// #include <ESP8266_ISR_Timer.h>
// #include <ESP8266TimerInterrupt.h>
// #include <TimerInterrupt_Generic_Debug.h>
# 6 "/home/vsergiu/work/relaytor/stuff/main.ino" 2

# 8 "/home/vsergiu/work/relaytor/stuff/main.ino" 2
# 9 "/home/vsergiu/work/relaytor/stuff/main.ino" 2
# 10 "/home/vsergiu/work/relaytor/stuff/main.ino" 2
# 11 "/home/vsergiu/work/relaytor/stuff/main.ino" 2
# 12 "/home/vsergiu/work/relaytor/stuff/main.ino" 2
# 13 "/home/vsergiu/work/relaytor/stuff/main.ino" 2
# 14 "/home/vsergiu/work/relaytor/stuff/main.ino" 2
# 15 "/home/vsergiu/work/relaytor/stuff/main.ino" 2

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
bool ledMode = 0x0;

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
  lcd.init(); // initialize the lcd 
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
