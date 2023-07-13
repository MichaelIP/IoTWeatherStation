#include <Arduino.h>

#include "enums.h"
#include "Models/measureModel.h"
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

McpNetwork::WeatherStation::Models::MeasureModel McpNetwork::WeatherStation::Sensors::PhotoResistor::readData() {

    Models::MeasureModel result;
    result.capability = (int)ECapabilities::Light;

    loggerManager->debug("Reading PhotoResistor sensor");    
    sensorValue = analogRead(sensorPin);

    result.measures.at(0).key = enumHelper.getCapability(ECapabilities::Light);
    result.measures.at(0).value = (float)sensorValue;

    return result;

}
