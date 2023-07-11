#ifndef Ntp_H
#define Ntp_H

#include <Arduino.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#include "WifiManager.h"

namespace  McpNetwork
{
    namespace WeatherStation {

        namespace Tools {

            class NtpHelper {
                public:
                    void initialize(McpNetwork::WeatherStation::Tools::WifiManager *wifiManager);
                    std::string getDateTime();

                private:

                    WiFiUDP ntpUDP;
                    WifiManager *wifiManager;
                    
                    void SynchronizeNtp();
            };

        }

    }

}

#endif