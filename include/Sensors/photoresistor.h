#ifndef PhotoResistor_H
#define PhotoResistor_H

#include <Arduino.h>

#include "sensors.h"
#include "Models/measureModel.h"
#include "Tools/KeyValuePairHelper.h"


namespace McpNetwork {

    namespace WeatherStation {

        namespace Sensors {

            class PhotoResistor : public ISensor
            {
                public:
                    PhotoResistor(McpNetwork::WeatherStation::Logger::LoggerManager *iLoggerManager);
                    
                    bool initialize(std::array<Models::IntKeyValuePair, MAX_PINS> pinLayout);
                    Models::MeasureModel readData();                

                private:
                    int sensorPin;
                    int sensorValue;
            };

        }
    }
}


#endif