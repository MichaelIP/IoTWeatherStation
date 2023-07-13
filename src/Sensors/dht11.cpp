#include <Arduino.h>
#include "DHT.h"

#include "enums.h"
#include "Models/keyValuePair.h"
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

McpNetwork::WeatherStation::Models::MeasureModel McpNetwork::WeatherStation::Sensors::Dht11::readData() {

    Models::MeasureModel result;
    result.capability = (int)ECapabilities::Temperature & (int)ECapabilities::Humidity;

    loggerManager->debug("Reading DHT11 sensor");
    
    dhtSensor->begin();
    temp  = dhtSensor->readTemperature();
    humidity = dhtSensor->readHumidity();

    result.measures.at(0).key = enumHelper.getCapability(ECapabilities::Temperature);
    result.measures.at(0).value = temp;

    result.measures.at(1).key = enumHelper.getCapability(ECapabilities::Humidity);
    result.measures.at(1).value = humidity;

    return result;


}

