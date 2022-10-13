#include <StreamUtils.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>


void(* resetFunc) (void) = 0;//declare reset function at address 0

class Setup {
private: 
    static bool ledMode;
    static StaticJsonDocument<512> config;
    static String corsHeaderTxt;
    static String version;
    /**
     * @brief 
     * 
     * @param thisType 
     * @return String 
     */
    static String encType(int thisType) {
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
    static void getRoot() {
        server.sendHeader(corsHeaderTxt,"*");
        StaticJsonDocument<128> doc;
        doc["mode"] = "setup";
        doc["version"] = version;
        String page;
        serializeJson(doc,page);
        server.send(200,"application/json",page);
    }

    /**
     * @brief connects to WiFI using provided SSID and WPA Key
     * 
     */
    static void connectWifi() {
        server.sendHeader(corsHeaderTxt,"*");

        for (uint8_t i = 0; i < server.args(); i++) { 
            if(server.argName(i)=="ssid") {
                config["ssid"] = server.arg(i);
            }
            if(server.argName(i)=="psk") {
                config["psk"] = server.arg(i);
            }
        }
        String ssid = config["ssid"];
        String psk = config["psk"];
        Serial.println(ssid+" / "+psk);
        
        WiFi.begin(ssid,psk);
        server.send(204);
    }


    /**
     * @brief return connection status
     * 
     */
    static void connectionStatus() {
        server.sendHeader(corsHeaderTxt,"*");

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
    }

    static void testApi() {
        server.sendHeader(corsHeaderTxt,"*");

        for (uint8_t i = 0; i < server.args(); i++) { 
            if(server.argName(i)=="api") {
                config["ssid"] = server.arg(i);
            }
            if(server.argName(i)=="key") {
                config["key"] = server.arg(i);
            }
        }

        WiFiClient client;
        HTTPClient http;

        if (!http.begin(client, config["api"])) {  
            server.send(500, "text/html","Unable to connect");
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
            config["api"] = "";
            Serial.print(F("API not working"));
            server.send(500, "text/plain","API not working");
        }

        http.end();
        client.stop();
    }

    static void toggleLed() {
        server.sendHeader(corsHeaderTxt,"*");
        ledMode = !ledMode;
        digitalWrite(LED_BUILTIN,ledMode);
        server.send(200, "text/plain",!ledMode ? "HIGH" : "LOW");
    }

    static void listSSIDs() {
        server.sendHeader(corsHeaderTxt,"*");

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
        //server.sendHeader("Content-type","application/json");
        server.send(200, "application/json",page);
    }


    static void reset() {
        server.sendHeader(corsHeaderTxt,"*");
        server.send(204);
        resetFunc();
    }

    static void saveConfig() {
        server.sendHeader(corsHeaderTxt,"*");

        EepromStream eepromStream(0, 256);
        serializeJson(config, eepromStream);
        eepromStream.flush();

        char output[256];
        serializeJson(config,output);
        Serial.println(output);
        server.send(200, "text/plain",output);
    }
public:
    /**
     * @brief Construct a new Setup object
     * 
     * @param srv 
     */
    Setup(StaticJsonDocument<512> &cfg) {
        ledMode = false;
        corsHeaderTxt = F("Access-Control-Allow-Origin");
        config = cfg;

        server.on("/", getRoot);
        server.on("/ssids",listSSIDs);
        server.on("/connect",connectWifi);
        server.on("/connstatus",connectionStatus);
        server.on("/testapi",testApi);
        server.on("/save",saveConfig);
        server.on("/tglled",toggleLed);
        server.on("/reset",reset);
    }
};

