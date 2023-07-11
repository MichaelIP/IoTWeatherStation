#ifndef MqttModel_H
#define MqttModel_H

#include <Arduino.h>
#include <string>

#include "const.h"
#include "Models/KeyValuePair.h"

namespace McpNetwork {

    namespace WeatherStation {

        namespace Models {

            class mqttModel {
                public:
                    int port;
                    std::string hostname;
                    std::array<StringKeyValuePair, MAX_CHANNELS> channels;
                    std::array<std::string, MAX_THUMBPRINTS> certificateThumbprints;

                    std::string getChannelPath(std::string name);
            };
        }
    }
}

#endif