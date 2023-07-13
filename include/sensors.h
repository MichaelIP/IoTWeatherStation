#ifndef Sensors_H
#define Sensors_H

#include <Arduino.h>

#include "const.h"
#include "enums.h"
#include "logger.h"
#include "Models/keyValuePair.h"
#include "Models/measureModel.h"

namespace McpNetwork {

    namespace WeatherStation {

        namespace Sensors {

            class ISensor {


                public:
                    virtual bool initialize(std::array<Models::IntKeyValuePair, MAX_PINS> pinLayout) = 0;
                    virtual Models::MeasureModel readData() = 0;
                
                protected:
                    McpNetwork::WeatherStation::EnumHelper enumHelper;
                    McpNetwork::WeatherStation::Logger::LoggerManager *loggerManager;

            };

        };
    }
}


#endif