
#include "mqtt.h"

// See https://github.com/Charry2014/ESP8266_MQTT_Username_Password/blob/master/sketch_ESP8266.ino
// and https://pubsubclient.knolleary.net/api

bool McpNetwork::WeatherStation::Mqtt::MqttManager::initialize(McpNetwork::WeatherStation::Models::settingsModel *settings, Logger::LoggerManager *loggerManager) {

  IPAddress ipAddress;
  this->settings = settings;
  this->loggerManager = loggerManager;
  if (!ipAddress.fromString(settings->mqtt.hostname.c_str())) {
    loggerManager->error("Settings MQTT/hostname should be an IP V4 address");
    return false;
  }

  mqttClient.setBufferSize(512);
  mqttClient.setSocketTimeout(30);
  mqttClient.setClient(espClient);
  mqttClient.setServer(ipAddress, settings->mqtt.port);
  return true;
}

bool McpNetwork::WeatherStation::Mqtt::MqttManager::connect() {
  
  int  nbRetries = 0;
  //loggerManager.trace("Connecting to MQTT...", false);
  
  string clientId = "client-" + settings->device.deviceName;
  mqttClient.connect(clientId.c_str());

  while(!mqttClient.connected() && nbRetries <= 200) {
    nbRetries++;
    delay(100);
  }
  
  if (!mqttClient.connected()) {
    loggerManager->error("Unable to connect to MQTT !");
    return false;
  }
  else {
    loggerManager->debug("Connected to MQTT !");
    mqttClient.publish(settings->mqtt.getChannelPath("Data").c_str(), "Connected");
    return true;
  }


}

void McpNetwork::WeatherStation::Mqtt::MqttManager::disconnect() {
    mqttClient.disconnect();
}

bool McpNetwork::WeatherStation::Mqtt::MqttManager::publish(std::string channel, std::string message) {
    return mqttClient.publish(channel.c_str(), message.c_str());    
}

bool McpNetwork::WeatherStation::Mqtt::MqttManager::verifyTls() {
    
  bool result = true;

  char message[250];
  snprintf(message, sizeof(message), "Verifying TLS connection. Server: %s, Port: %i", settings->mqtt.hostname.c_str(), settings->mqtt.port); 
  loggerManager->debug(message);

    //espClient.setFingerprint(settings->mqtt.certificateThumbprints.at(0).c_str());

    //espClient.setCACert(settings->mqtt.caContent.c_str());
    espClient.allowSelfSignedCerts();

  int verifyTls = espClient.connect(settings->mqtt.hostname.c_str(), settings->mqtt.port);
  if (verifyTls == 0) {
     loggerManager->debug("verifytls: WiFiClientSecure connection failed");
  }
  else {
    loggerManager->debug("verifytls: Connection successfull");
  }

  return result;
}
