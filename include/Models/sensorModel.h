#ifndef SensorModel_H
#define SensorModel_H

#include <Arduino.h>
#include <string>

#include "enums.h"
#include "Models/KeyValuePair.h"


namespace McpNetwork {

    namespace WeatherStation {

        namespace Models {

            struct sensorModel {
                std::string sensorName;
                EDevices sensorType;
                bool isValid;
                std::array<IntKeyValuePair, MAX_PINS> pinLayout;
            };
        }
    }
}

#endif