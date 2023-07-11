#include <Arduino.h>
#include "DHT.h"

#include "Sensors/dht11.h"

McpNetwork::WeatherStation::Sensors::Dht11::Dht11(McpNetwork::WeatherStation::Logger::LoggerManager *iLoggerManager) {
    loggerManager = iLoggerManager;
}

bool McpNetwork::WeatherStation::Sensors::Dht11::initialize(std::array<Models::IntKeyValuePair, MAX_PINS> pinLayout) {

    char message[255];
    loggerManager->debug("Checking DHT11 sensor");
    
    Tools::KeyValuePairHelper kvpHelper;   
    sensorPin = kvpHelper.FindValue(&pinLayout, MAX_PINS, "Out");
    if (sensorPin == -1) {
        loggerManager->error("DHT11 signal pin not set !");
    } 
    else {
        dhtSensor = new DHT(sensorPin, DHT11);
        snprintf(message, sizeof(message), "DHT11 signal pin declared to %i", sensorPin);
        loggerManager->debug(message);
    }

    return sensorPin != -1 ;

}

void McpNetwork::WeatherStation::Sensors::Dht11::readData() {

    char message[255];
    loggerManager->debug("Reading DHT11 sensor");
    
    dhtSensor->begin();
    temp  = dhtSensor->readTemperature();
    humidity = dhtSensor->readHumidity();
    snprintf(message, sizeof(message), "Temp: %f - Humidity: %f", temp, humidity);
    loggerManager->debug(message);
}

