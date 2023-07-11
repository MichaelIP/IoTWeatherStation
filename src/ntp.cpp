
#if defined(ENV_ESP8266)
  #include <ESP8266WiFi.h>
#endif
#if defined(ENV_ESP32)
  #include <WiFi.h>
#endif

#include <time.h>
#include <ctime>

#include "ntp.h"
#include "wifiManager.h"

#ifdef WITH_GDB
#include <GDBStub.h>
#endif

void McpNetwork::WeatherStation::Tools::NtpHelper::initialize(Tools::WifiManager * lWifiManager) {
  wifiManager = lWifiManager;
  SynchronizeNtp();
}

std::string McpNetwork::WeatherStation::Tools::NtpHelper::getDateTime() {

    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    timeinfo = localtime (&rawtime);
    char buffer[80];
    strftime(buffer,sizeof(buffer),"%Y-%m-%d %H:%M:%S", timeinfo);
    std::string str(buffer);
    return str;

}

void McpNetwork::WeatherStation::Tools::NtpHelper::SynchronizeNtp() {

    if (wifiManager->connectWifi()) {
    
      NTPClient timeClient(ntpUDP);
      timeClient.begin();
      timeClient.update();
      timeClient.getEpochTime();
      timeClient.end();
      wifiManager->disconnectWifi();
    }
    
}
