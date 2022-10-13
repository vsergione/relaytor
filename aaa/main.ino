#include <StreamUtils.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>


ESP8266WebServer server(80);
#ifndef APSSID
#define APSSID "ESPap"
#define APPSK  "thereisnospoon"
#endif


class MyConfig {
private:
    static String ssid;
    static String psk;
    static String api;
    static String key;
        
    static MyConfig *instance;
    /**
     * @brief Construct a new config object by loading data from EEPROM
     * 
     */
    MyConfig() {
        if(!Serial) {
            Serial.begin(115200);
        }
        StaticJsonDocument<500> doc;
        EEPROM.begin(256);
        EepromStream eepromStream(0,256);
        DeserializationError err = deserializeJson(doc, eepromStream);
        ssid = String(doc["ssid"]);
        // for (JsonPair kv : root) {
        //     if(kv.key()=="ssid") {
        //         ssid = kv.value().as<char*>();
        //     }
        //     if(kv.key()=="psk") {
        //         psk = kv.value().as<char*>();
        //     }
        //     if(kv.key()=="api") {
        //         api = kv.value().as<char*>();
        //     }
        //     if(kv.key()=="key") {
        //         key = kv.value().as<char*>();
        //     }
        // }

    }
public:
    static MyConfig *load() {
        if(instance)
            return instance;
        else 
            return new MyConfig();
    }

    /**
     * @brief set config value
     * 
     * @param name 
     * @param value 
     * @return config* 
     */
    MyConfig *set(String name,char *value, bool persist=false) {
        // if(name=="ssid") {
        //     ssid = value;
        // }
        // if(name=="psk") {
        //     psk = value;
        // }
        // if(name=="api") {
        //     api = value;
        // }
        // if(name=="key") {
        //     key = value;
        // }
        return persist ? this->persist() : this;
    }

    /**
     * @brief save to eeprom
     * 
     * @return true 
     * @return false 
     */
    MyConfig *persist() {
        EepromStream eepromStream(0, 256);
        StaticJsonDocument<500> doc;
        doc["ssid"] = ssid;
        doc["psk"] = psk;
        doc["api"] = api;
        doc["key"] = key;
      
        serializeJson(doc, eepromStream);
        eepromStream.flush();
        return this;
    }

    /**
     * @brief get config value
     * 
     * @param name 
     * @return String 
     */
    String get(String name) {
        if(name=="ssid") {
            return ssid;
        }
        if(name=="psk") {
            return psk;
        }
        if(name=="api") {
            return api;
        }
        if(name=="key") {
            return key;
        }
    }
};
MyConfig *MyConfig::instance = 0;

MyConfig *myConfig = MyConfig::load();

void setup() {   

}

void loop() {
  server.handleClient();

}