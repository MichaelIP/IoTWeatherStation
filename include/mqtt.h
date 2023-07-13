#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <String>

#include "logger.h"
#include "Models/settingsModel.h"

using namespace std;

namespace McpNetwork {

    namespace WeatherStation {

        namespace Mqtt {
            
            class  MqttManager
            {
                public:
                    bool connect();
                    void disconnect();
                    bool verifyTls(); 
                    bool initialize(McpNetwork::WeatherStation::Models::settingsModel *settings, McpNetwork::WeatherStation::Logger::LoggerManager *loggerManager);
                    bool publish(string channel, string message);

                private:
                    McpNetwork::WeatherStation::Models::settingsModel *settings;
                    McpNetwork::WeatherStation::Logger::LoggerManager *loggerManager;
                    PubSubClient mqttClient;
                    WiFiClientSecure espClient;

            };
        } 
    }
}
#endif