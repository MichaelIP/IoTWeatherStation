#include <Arduino.h>
 #include <ArduinoJson.h>
 #include <LittleFS.h>

 #include "settings.h"

using namespace std;

bool McpNetwork::WeatherStation::Settings::SettingManager::parseSetttings(std::string fileName, Models::settingsModel &settings) {

    std::string jsonContent;
    if (!readFile(fileName, jsonContent)) {
        return false;
    }

    StaticJsonDocument<2048> json;
    DeserializationError error = deserializeJson(json, jsonContent);
    if (error) {
        errorMessage = error.c_str();
        return false;
    }

    JsonVariantConst value;

    // Management of LOGGER
    value = json["Logger"]["Speed"];
    settings.logger.speed = value.as<int>();

    value = json["Logger"]["LogLevel"];
    McpNetwork::WeatherStation::EnumHelper enumHelper;
    settings.logger.logLevel = enumHelper.getLogLevel(value.as<std::string>());
    
    // Management of WIFI connection
    value = json["Wifi"]["SSID"];
    settings.wiFi.ssid = value.as<std::string>();
    value = json["Wifi"]["Password"];
    settings.wiFi.password = value.as<std::string>();
   
    // Management of MQTT connection
    value = json["Mqtt"]["HostName"];
    settings.mqtt.hostname = value.as<std::string>();
    settings.mqtt.port = json["Mqtt"]["Port"];

    int nbItems = json["Mqtt"]["Channels"].size();
    for(int index=0; index < nbItems; index++) {
        value = json["Mqtt"]["Channels"][index]["Key"];
        if (!value.isNull()) {
            settings.mqtt.channels[index].key = value.as<std::string>();
            value = json["Mqtt"]["Channels"][index]["Value"];
            settings.mqtt.channels[index].value = value.as<std::string>();
        }
    }

    nbItems = json["Mqtt"]["CertificateThumbPrints"].size();
    for(int index=0; index < nbItems; index++) {
        value = json["Mqtt"]["CertificateThumbPrints"][index];
        if (!value.isNull()) {
            settings.mqtt.certificateThumbprints[index] = value.as<std::string>();
        }
    }

    // Management of DEVICE
    value = json["Device"]["DeviceName"];
    settings.device.deviceName = value.as<std::string>();

    value = json["Device"]["SleepTimeSeconds"];
    settings.device.sleepTimeSeconds = value.as<int>();

    // settings.device.nbSensors = 0;
    JsonArray sensorNames = json["Device"]["Sensors"];

    nbItems = sensorNames.size();
     for(int indexSensor=0; indexSensor < nbItems; indexSensor++) {
         
         value = json["Device"]["Sensors"][indexSensor]["SensorName"];
         settings.device.sensors.at(indexSensor).sensorName = value.as<std::string>();

         int nbPins = json["Device"]["Sensors"][indexSensor]["PinLayout"].size();
         for(int indexPin=0; indexPin < nbPins; indexPin++) {
            value = json["Device"]["Sensors"][indexSensor]["PinLayout"][indexPin]["Key"];
            settings.device.sensors.at(indexSensor).pinLayout.at(indexPin).key = value.as<std::string>();
            value = json["Device"]["Sensors"][indexSensor]["PinLayout"][indexPin]["Value"];
            settings.device.sensors.at(indexSensor).pinLayout.at(indexPin).value = value.as<int>();
         }
    }


    value = json["SSL"]["caFile"];
    settings.mqtt.certificateFileName  = value.as<std::string>();
   
    if (!readFile(fileName, settings.mqtt.caContent)) {
        return false;
    }

    return true;

}

bool McpNetwork::WeatherStation::Settings::SettingManager::readFile(string fileName, std::string &content) {

    // Initialize LittleFS
    if(!LittleFS.begin()){
        errorMessage = "An Error has occurred while mounting SPIFFS";
        return false;
    }

    File file = LittleFS.open(fileName.c_str(), "r");
    if(!file) {
        errorMessage = "Failed to open file for reading";
        return false;
    }

    content = file.readString().c_str();
    file.close();   
    LittleFS.end();   

    return true;
  

}

std::string McpNetwork::WeatherStation::Models::mqttModel::getChannelPath(std::string name) {
   
    std::string result = NULL;
    for(int index=0; index<= MAX_CHANNELS; index++) {
        
        if (channels[index].key.empty()) {
            break;
        }        
        
        if (channels[index].key == name) {
            result = channels[index].value;
        }
    }

    return result;

}
