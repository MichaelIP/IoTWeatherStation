#ifndef Enums_H
#define Enums_H

#include <Arduino.h>
#include <string>
#include <typeinfo>
#include <unordered_map>

namespace McpNetwork {

    namespace WeatherStation {

        enum ELogLevel { 
            All, 
            Debug, 
            Info, 
            Warn, 
            Error, 
            Fatal, 
            Off 
        };

        enum ECapabilities {
            None = 0x00, 
            Temperature = 0x01, 
            Humidity = 0x02, 
            Pressure = 0x04, 
            Light = 0x08, 
            WindDirection = 0x10, 
            WindAvgSpeed = 0x20, 
            WindMaxSpeed = 0x40, 
            RainMeter = 0x80, 
            BatteryIndicator = 0x100,
            DateTime = 0x200,
        };

        enum EDevices {
            Unknown, 
            Clock,
            Dht11, 
            Bmp280, 
            Bme280, 
            PhotoResistor, 
            Anemometer, 
            Weathercock, 
            RainGauge, 
            Wh1080,
        };        
        
        class enumHelper {
            public:
                McpNetwork::WeatherStation::ELogLevel getLogLevel(std::string value) {
                    std::unordered_map<std::string, McpNetwork::WeatherStation::ELogLevel>::const_iterator result = logLevelMap.find(toLower(value));
                    if (result == logLevelMap.end()) {
                        return ELogLevel::Off;
                    } 
                    else {
                        return result->second;
                    }
                }
                
                std::string getLogLevel(McpNetwork::WeatherStation::ELogLevel logLevel) {
                    for (const auto& [key, value] : logLevelMap)
                    if (value == logLevel) {
                        return key;
                    }
                    std::string result = getLogLevel(ELogLevel::Off);
                    return result;
                }
                
                McpNetwork::WeatherStation::EDevices getDeviceType(std::string value) {
                    
                    std::unordered_map<std::string, McpNetwork::WeatherStation::EDevices>::const_iterator result = devicesMap.find(toLower(value));
                    if (result == devicesMap.end()) {
                        return EDevices::Unknown;
                    } 
                    else {
                        return result->second;
                    }
                }


            private:
                std::unordered_map<std::string, McpNetwork::WeatherStation::ELogLevel> logLevelMap = { 
                    { "all", McpNetwork::WeatherStation::ELogLevel::All }, 
                    { "debug", McpNetwork::WeatherStation::ELogLevel::Debug }, 
                    { "info", McpNetwork::WeatherStation::ELogLevel::Info},
                    { "warn", McpNetwork::WeatherStation::ELogLevel::Warn },
                    { "error", McpNetwork::WeatherStation::ELogLevel::Error }, 
                    { "fatal", McpNetwork::WeatherStation::ELogLevel::Fatal }, 
                    { "off", McpNetwork::WeatherStation::ELogLevel::Off }
                };

                std::unordered_map<std::string, McpNetwork::WeatherStation::ECapabilities> capabilitiesMap = {
                    { "none", ECapabilities::None }, 
                    { "temperature", ECapabilities::Temperature },
                    { "humidity", ECapabilities::Humidity },
                    { "pressure", ECapabilities::Pressure },
                    { "light", ECapabilities::Light },
                    { "winddirection", ECapabilities::WindDirection },
                    { "windavgspeed", ECapabilities::WindAvgSpeed },
                    { "windmaxspeed", ECapabilities::WindMaxSpeed },
                    { "rainmeter", ECapabilities::RainMeter },
                    { "batteryindicator", ECapabilities::BatteryIndicator },
                    { "datetime", ECapabilities::DateTime }
                };

                std::unordered_map<std::string, McpNetwork::WeatherStation::EDevices> devicesMap = {
                    { "unknwon", EDevices::Unknown }, 
                    { "clock", EDevices::Clock },
                    { "dht11", EDevices::Dht11 },
                    { "bmp280", EDevices::Bmp280 },
                    { "bme280", EDevices::Bme280 },
                    { "photoresistor", EDevices::PhotoResistor },
                    { "anemometer", EDevices::Anemometer },
                    { "weathercock", EDevices::Weathercock },
                    { "raingauge", EDevices::RainGauge },
                    { "wh1080", EDevices::Wh1080 }
                };                
        
                std::string toLower(std::string value) {

                     std::string result = "";
                     for (uint i = 0; i < value.length(); i++) {
                        char ch = tolower(value[i]);
                        result.push_back(ch);
                    }
                    return result;
                };
        
        };

    }
}


#endif