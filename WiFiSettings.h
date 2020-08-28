#ifndef WiFiSettings_h
#define WiFiSettings_h

#include <Arduino.h>
#include <functional>

#ifdef ESP32
    #define ESPFS SPIFFS
    #include <SPIFFS.h>
    #include <WiFi.h>
    #include <WebServer.h>
    #include <esp_task_wdt.h>
#elif ESP8266
    #define ESPFS LittleFS
    #include <LittleFS.h>
    #include <ESP8266WiFi.h>
    #include <ESP8266WebServer.h>
    #define WebServer ESP8266WebServer
    #define esp_task_wdt_reset wdt_reset
    #define wifi_auth_mode_t uint8_t    // wl_enc_type
    #define WIFI_AUTH_OPEN ENC_TYPE_NONE
    #define WIFI_AUTH_WPA2_ENTERPRISE -1337 // not available on ESP8266
    #define setHostname hostname
    #define INADDR_NONE IPAddress(0,0,0,0)
#else
    #error "This library only supports ESP32 and ESP8266"
#endif
#include <DNSServer.h>
#include <limits.h>

class WiFiSettingsClass {
    public:
        typedef std::function<void(void)> TCallback;
        typedef std::function<int(void)> TCallbackInt;

        WiFiSettingsClass(WebServer* Wserver,DNSServer* dns);
        void begin();
        bool connect(bool portal = true, int wait_seconds = 30);
        void portal(bool apmode); // true - start on AP mode else only web server
        String string(const String& name, const String& init = "", const String& label = "");
        String string(const String& name, unsigned int max_length, const String& init = "", const String& label = "");
        String string(const String& name, unsigned int min_length, unsigned int max_length, const String& init = "", const String& label = "");
        long integer(const String& name, long init = 0, const String& label = "");
        long integer(const String& name, long min, long max, long init = 0, const String& label = "");
        bool checkbox(const String& name, bool init = false, const String& label = "");

        String hostname;
        String password;
        bool secure;

        TCallback onConnect;
        TCallbackInt onWaitLoop;
        TCallback onSuccess;
        TCallback onFailure;
        TCallback onPortal;
        TCallback onConfigSaved;
        TCallback onRestart;
        TCallback onPortalWaitLoop;
        
    private:
        bool begun;
        WebServer* _server;
        DNSServer* _dnsServer;
        
};

//extern WiFiSettingsClass WiFiSettings;

#endif
