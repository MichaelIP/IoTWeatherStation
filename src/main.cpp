#include <Arduino.h>

#include "sensors.h"
#include "Sensors/dht11.h"
#include "Sensors/photoresistor.h"

#include "Models/measureModel.h"

#include "enums.h"
#include "logger.h"
#include "settings.h"
#include "ntp.h"
#include "mqtt.h"

#ifdef WITH_GDB
#include <GDBStub.h>
#endif

struct McpNetwork::WeatherStation::Models::settingsModel settings;

McpNetwork::WeatherStation::Mqtt::MqttManager mqttManager;
McpNetwork::WeatherStation::Logger::LoggerManager loggerManager;
McpNetwork::WeatherStation::Settings::SettingManager settingsManager;

McpNetwork::WeatherStation::EnumHelper enumHelper;
McpNetwork::WeatherStation::Tools::NtpHelper ntpHelper;
McpNetwork::WeatherStation::Tools::WifiManager *wifiManager;

McpNetwork::WeatherStation::Sensors::ISensor *sensors[MAX_SENSORS];

void checkDevices();
void sleepForEver();
void displaySettings();
void publishMessage(std::string message);

McpNetwork::WeatherStation::Models::MeasureModel readSensors();

void setup() {

  #ifdef WITH_GDB
  gdbstub_init();
  #endif

  if (!settingsManager.parseSetttings("/settings.json", settings)) {
    loggerManager.initialize(115200);
    loggerManager.error("Error while retrieving settings !");

    char message[255];
    snprintf(message, sizeof(message), "Error: %s", settingsManager.errorMessage.c_str());
    loggerManager.error(message); 
    loggerManager.fatal("STOPPED");
    sleepForEver();
  }
  
  wifiManager = new McpNetwork::WeatherStation::Tools::WifiManager(settings.wiFi.ssid, settings.wiFi.password);
  ntpHelper.initialize(wifiManager);

  loggerManager.initialize(&settings.logger);

  loggerManager.info("");
  loggerManager.info("/=============================================\\");
  loggerManager.info("|                                             |");
  loggerManager.info("|    W E A T H E R       S T A T I O N        |");
  loggerManager.info("|                                             |");
  loggerManager.info("\\=============================================/");

  displaySettings();
  mqttManager.initialize(&settings, &loggerManager);

  checkDevices();

}

void loop() {
  
  McpNetwork::WeatherStation::Models::MeasureModel measure = readSensors();

  loggerManager.info("Building MQTT message");
  std::string mqttMessage = measure.serializeMesaures();
  loggerManager.debug(mqttMessage.c_str());

  loggerManager.info("Sending MQTT message");
  publishMessage(mqttMessage);

  loggerManager.info("Done. Going to deepsleep");
  delay(settings.device.sleepTimeSeconds * 1000);
}

McpNetwork::WeatherStation::Models::MeasureModel readSensors() {

  McpNetwork::WeatherStation::Models::MeasureModel result;
  result.deviceName = settings.device.deviceName;
  result.dateMeasure = ntpHelper.getDateTime();
  result.capability = (int)McpNetwork::WeatherStation::ECapabilities::None;

  int measureNb = 0;
  loggerManager.info("Reading sensors.");
  for(int index = 0; index < MAX_SENSORS; index++) {
    
    if (settings.device.sensors[index].sensorName.empty()) {
      break;
    }

    if (settings.device.sensors[index].isValid) {
      char message[255];
      snprintf(message, sizeof(message), " > Sensor %s", settings.device.sensors[index].sensorName.c_str());
      loggerManager.debug(message);

      McpNetwork::WeatherStation::Models::MeasureModel sensorMeasure = sensors[index]->readData();
      
      result.capability += sensorMeasure.capability;

      int indexMeasure = 0;
      while(sensorMeasure.measures.at(indexMeasure).key != enumHelper.getCapability(McpNetwork::WeatherStation::ECapabilities::None)) {
        result.measures.at(measureNb++) = sensorMeasure.measures.at(indexMeasure++);
      }

    }

  }

  return result;

}

void publishMessage(std::string message) {

  
  loggerManager.debug("Connecting to WIFI");
  if (wifiManager->connectWifi()) {
    loggerManager.debug("Veryfing TLS connection");    
    if (mqttManager.verifyTls()) {
      loggerManager.debug("Connecting to MQTT server");
      if (mqttManager.connect()) {
        loggerManager.debug("Publishing message");
        mqttManager.publish(settings.mqtt.getChannelPath("Data"), message);
      } 
      else {
        loggerManager.error("Error while veryfing TLS connection");
      }
      mqttManager.disconnect();
    }    
    else {
      loggerManager.error("Unable to connect to MQTT server");
    }
    wifiManager->disconnectWifi();
  }
  else {
    loggerManager.error("Unable to connect to WIFI");
  };

}


void sleepForEver() {
  loggerManager.info("Going to deep sleep mode until reset");
  ESP.deepSleep(0); 
}

void displaySettings() {

  char message[255];
  snprintf(message, sizeof(message), "Device Name: %s", settings.device.deviceName.c_str());
  loggerManager.debug(message);
  snprintf(message, sizeof(message), "MQTT Server: %s", settings.mqtt.hostname.c_str());
  loggerManager.debug(message);
  snprintf(message, sizeof(message), "MQTT Port: %d", settings.mqtt.port);
  loggerManager.debug(message);

  snprintf(message, sizeof(message), "WIFI SSID: %s", settings.wiFi.ssid.c_str());
  loggerManager.debug(message);

  for(int index=0; index < MAX_CHANNELS; index++) {
    if (settings.mqtt.channels[index].key.empty()) {
      loggerManager.debug("No more channels");
      break;
    }
    snprintf(message, sizeof(message), "MQTT Data channels #%d: %s. Path [%s]", index, settings.mqtt.channels[index].key.c_str(), settings.mqtt.channels[index].value.c_str());
    loggerManager.debug(message);
  }

  for(int index=0; index < MAX_THUMBPRINTS; index++) {
    if (settings.mqtt.certificateThumbprints[index].empty()) {
      loggerManager.debug("No more thumprint...");
      break;
    }
    snprintf(message, sizeof(message), "MQTT certificate thumbprint #%d: %s", index, settings.mqtt.certificateThumbprints[index].c_str());
    loggerManager.debug(message);
  }

  for(int index = 0; index < MAX_SENSORS; index++) {
    if (settings.device.sensors[index].sensorName.empty()) {
      loggerManager.debug("No more sensors...");
      break;
    }
    snprintf(message, sizeof(message), "Sensor #%i: %s", index, settings.device.sensors.at(index).sensorName.c_str());
    loggerManager.info(message);
    int nbPins = settings.device.sensors.at(index).pinLayout.size();
    for(int indexPin=0; indexPin < nbPins; indexPin++) {
        if (settings.device.sensors.at(index).pinLayout.at(indexPin).key.empty()) {
          loggerManager.debug("No more pins...");
          break;
        }
        snprintf(message, sizeof(message), " + Pin #%i: %s -> %i", indexPin, settings.device.sensors.at(index).pinLayout.at(indexPin).key.c_str(), settings.device.sensors.at(index).pinLayout.at(indexPin).value);
        loggerManager.debug(message);
    }

  }
  
}

void checkDevices() {

  for(int index = 0; index < MAX_SENSORS; index++) {
    
    if (settings.device.sensors[index].sensorName.empty()) {
      break;
    }

    McpNetwork::WeatherStation::EDevices deviceType = enumHelper.getDeviceType(settings.device.sensors[index].sensorName);
    if (deviceType == McpNetwork::WeatherStation::EDevices::Unknown) {
      settings.device.sensors[index].isValid = false;
    }
    else {

      settings.device.sensors[index].sensorType = deviceType;
      switch (settings.device.sensors[index].sensorType) {

        case McpNetwork::WeatherStation::EDevices::PhotoResistor:
        {
          McpNetwork::WeatherStation::Sensors::PhotoResistor *sensor = new McpNetwork::WeatherStation::Sensors::PhotoResistor(&loggerManager);
          sensors[index] = sensor;
          settings.device.sensors[index].isValid = sensors[index]->initialize(settings.device.sensors[index].pinLayout);
          break;
        }
        case McpNetwork::WeatherStation::EDevices::Dht11:
        {
          McpNetwork::WeatherStation::Sensors::Dht11 *sensor = new McpNetwork::WeatherStation::Sensors::Dht11(&loggerManager);
          sensors[index] = sensor;
          settings.device.sensors[index].isValid = sensors[index]->initialize(settings.device.sensors[index].pinLayout);
          break;
        }
      
        default:
          settings.device.sensors[index].isValid = false;
          break;
      }
      
    }


      char message[50];
      snprintf(message, sizeof(message), "Sensor [%s] is %ssupported.", settings.device.sensors[index].sensorName.c_str(), settings.device.sensors[index].isValid ? "" : "not ");
      loggerManager.warn(message);

  }
}