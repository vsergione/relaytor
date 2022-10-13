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



