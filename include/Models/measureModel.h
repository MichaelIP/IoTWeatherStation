#ifndef  MeasureModel_H
#define MeasureModel_H

#include <Arduino.h>
#include <string>

#include "const.h"
#include "Models/KeyValuePair.h"

namespace McpNetwork {

    namespace WeatherStation {

        namespace Models {

            struct singleMeasureModel {

            };

            struct MeasureModel {
                std::string deviceName;
                std::string dateMeasure;
                int capability;
                std::array<std::string, MAX_MEASURES> measures;
            };
        }
    }
}

#endif