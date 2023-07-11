#ifndef Sensors_H
#define Sensors_H

#include <Arduino.h>

#include "const.h"
#include "logger.h"
#include "Models/KeyValuePair.h"

namespace McpNetwork {

    namespace WeatherStation {

        namespace Sensors {

            class ISensor {


                public:
                    virtual bool initialize(std::array<Models::IntKeyValuePair, MAX_PINS> pinLayout) = 0;
                    virtual void readData() = 0;
                
                protected:
                    McpNetwork::WeatherStation::Logger::LoggerManager *loggerManager;

            };

        };
    }
}


#endif