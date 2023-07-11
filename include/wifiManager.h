#ifndef WifiManager_H
#define WifiManager_H

#include <Arduino.h>
#include <string>

namespace McpNetwork
{
    namespace WeatherStation {

        namespace Tools {

            class WifiManager {
                public:

                    WifiManager(std::string ssid, std::string password);

                    bool connectWifi();
                    void disconnectWifi();

                    bool connectWifi(std::string ssid, std::string password);
                private:
                    std::string ssid;
                    std::string password;
            };
        }
    }
} 

#endif