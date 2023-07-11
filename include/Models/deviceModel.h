#ifndef DeviceModel_H
#define DeviceModel_H

#include <Arduino.h>
#include <string>

#include "const.h"
#include "Models/sensorModel.h"

namespace McpNetwork {

    namespace WeatherStation {

        namespace Models {
            struct deviceModel {
                std::string deviceName;
                std::array<sensorModel, MAX_SENSORS> sensors;
                int sleepTimeSeconds;
            };
        }
    }
}


#endif