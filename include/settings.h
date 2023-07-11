#ifndef Settings_H
#define Settings_H

#include <Arduino.h>
#include <string>
#include "Models/settingsModel.h"

namespace McpNetwork {

    namespace WeatherStation {

        namespace Settings {

            class SettingManager {
                public: 
                    std::string errorMessage;
                    bool parseSetttings(std::string fileName, McpNetwork::WeatherStation::Models::settingsModel &settings);
                private:
                    bool readFile(std::string fileName, std::string &content);
            };
        };
    }
}

#endif