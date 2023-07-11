
#if defined(ENV_ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ENV_ESP32)
  #include <WiFi.h>
#endif

#include <string>
#include "wifiManager.h"

#define BUILTIN_LED 2

McpNetwork::WeatherStation::Tools::WifiManager::WifiManager (std::string lSsid, std::string lPassword) {

  ssid = lSsid;
  password = lPassword;

}

bool McpNetwork::WeatherStation::Tools::WifiManager::connectWifi() {
  
  return connectWifi(ssid.c_str() , password.c_str());

}

bool McpNetwork::WeatherStation::Tools::WifiManager::connectWifi(std::string ssid, std::string password) {

  if (WiFi.status() == WL_CONNECTED) {
    // Connected to WiFi
    return true;
  }  
  
  int nbRetries = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    nbRetries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    // Connected to WiFi
    return true;
  } 
  else {
    // Not connected to WiFi
    Serial.println("Problem while connecting to WIFI.");
    WiFi.printDiag(Serial);
    return false;
  }

}

void McpNetwork::WeatherStation::Tools::WifiManager::disconnectWifi() {
  WiFi.disconnect();
}