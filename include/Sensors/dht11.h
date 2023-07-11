#ifndef Dht11_H
#define Dht11_H

#include <Arduino.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

#include "sensors.h"
#include "Tools/KeyValuePairHelper.h"


namespace McpNetwork {

    namespace WeatherStation {

        namespace Sensors {

            class Dht11 : public ISensor
            {
                public:
                    Dht11(McpNetwork::WeatherStation::Logger::LoggerManager *iLoggerManager);
                    
                    bool initialize(std::array<Models::IntKeyValuePair, MAX_PINS> pinLayout);
                    void readData();                

                private:
                    DHT *dhtSensor;
                    int sensorPin;
                    float temp;
                    float humidity;
            };

        }
    }
}


#endif