#include "Tools/KeyValuePairHelper.h"

#include "Models/KeyValuePair.h"

int McpNetwork::WeatherStation::Tools::KeyValuePairHelper::FindValue(std::array<Models::IntKeyValuePair, FULL_MAX> *values, int maxSize, std::string key) {

    for(int index=0; index < maxSize; index++) {
        
        if (values->at(index).key == key.c_str()) {
            return values->at(index).value;
        }
    }

    return -1;

}

std::string McpNetwork::WeatherStation::Tools::KeyValuePairHelper::FindValue(std::array<Models::StringKeyValuePair, FULL_MAX> *values, int maxSize, std::string key) {

    for(int index=0; index < maxSize; index++) {
        
        if (values->at(index).key == key.c_str()) {
            return values->at(index).value;
        }
    }

    return "";

}
