#include <Arduino.h>
#include <ArduinoJson.h>

#include "enums.h"
#include "Models/measureModel.h"

McpNetwork::WeatherStation::Models::MeasureModel::MeasureModel() {
    for(int index=0; index< MAX_MEASURES; index++) {
        measures.at(index).key = enumHelper.getCapability(McpNetwork::WeatherStation::ECapabilities::None);
        measures.at(index).value = 0;
    }
}

std::string McpNetwork::WeatherStation::Models::MeasureModel::serializeMesaures() {

    char value[10];
    McpNetwork::WeatherStation::EnumHelper enumHelper;
    StaticJsonDocument<512> doc;
    doc["deviceName"] = deviceName;
    doc["capabilities"] = capability;    
    doc["timesptamp"] = dateMeasure;

    int index=0;
    while(measures.at(index).key != enumHelper.getCapability(ECapabilities::None)) 
    {
        snprintf(value,sizeof(value),"%.2f", measures.at(index).value);
        doc["measures"][measures.at(index).key] = value;
        index++;
    }
  
    std::string result;
    serializeJson(doc, result);  

    char message[512];
    snprintf(message, sizeof(message), "Message: %s", result.c_str());

    return result;  

  
}