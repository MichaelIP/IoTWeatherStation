#ifndef SettingsModel_H
#define SettingsModel_H

#include <Arduino.h>

#include "Models/loggerModel.h"
#include "Models/wiFiModel.h"
#include "Models/mqttModel.h"
#include "Models/deviceModel.h"

namespace McpNetwork {

    namespace WeatherStation {

        namespace Models {

            struct settingsModel {
                loggerModel logger;
                wiFiModel wiFi;
                mqttModel mqtt;
                deviceModel device;
            };

        }
    }
}
#endif