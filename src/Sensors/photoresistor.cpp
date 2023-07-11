#include <Arduino.h>

#include "Sensors/photoresistor.h"


McpNetwork::WeatherStation::Sensors::PhotoResistor::PhotoResistor(McpNetwork::WeatherStation::Logger::LoggerManager *iLoggerManager) {
    loggerManager = iLoggerManager;
}

bool McpNetwork::WeatherStation::Sensors::PhotoResistor::initialize(std::array<Models::IntKeyValuePair, MAX_PINS> pinLayout) {

    char message[255];
    loggerManager->debug("Checking PhotoResistor sensor");
    
    Tools::KeyValuePairHelper kvpHelper;   
    sensorPin = kvpHelper.FindValue(&pinLayout, MAX_PINS, "Data");
    if (sensorPin == -1) {
        loggerManager->error("PhotoResistor pin not set !");
    } 
    else {
        snprintf(message, sizeof(message), "PhotoResistor pin declared to %i", sensorPin);
        loggerManager->debug(message);
    }
    return sensorPin != -1 ;

}

void McpNetwork::WeatherStation::Sensors::PhotoResistor::readData() {

    char message[255];
    loggerManager->debug("Reading PhotoResistor sensor");
    
    sensorValue = analogRead(sensorPin);
    snprintf(message, sizeof(message), "Value read: %i", sensorValue);
    loggerManager->debug(message);
}
