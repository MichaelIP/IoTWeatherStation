#ifndef WifiModel_H
#define WifiModel_H

#include <Arduino.h>
#include <string>

namespace McpNetwork {

    namespace WeatherStation {

        namespace Models {

            struct wiFiModel {
                std::string ssid;
                std::string password;
            };
        }
    }
}

#endif