#include <Arduino.h>


#include "sensors.h"
#include "Sensors/photoresistor.h"

#include "enums.h"
#include "logger.h"
#include "settings.h"
#include "ntp.h"

#ifdef WITH_GDB
#include <GDBStub.h>
#endif

struct McpNetwork::WeatherStation::Models::settingsModel settings;

McpNetwork::WeatherStation::Settings::SettingManager settingsManager;
McpNetwork::WeatherStation::Logger::LoggerManager loggerManager;

McpNetwork::WeatherStation::Tools::WifiManager *wifiManager;
McpNetwork::WeatherStation::enumHelper enumHelper;

McpNetwork::WeatherStation::Sensors::ISensor *sensors[MAX_SENSORS];


void checkDevices();
void sleepForEver();
void displaySettings();

void setup() {

  Serial.begin(115600);
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
  McpNetwork::WeatherStation::Tools::NtpHelper ntpHelper;
  ntpHelper.initialize(wifiManager);

  loggerManager.initialize(&settings.logger);

  loggerManager.info("");
  loggerManager.info("/=============================================\\");
  loggerManager.info("|                                             |");
  loggerManager.info("|    W E A T H E R       S T A T I O N        |");
  loggerManager.info("|                                             |");
  loggerManager.info("\\=============================================/");

  displaySettings();
  

  checkDevices();

}

void loop() {
  
  loggerManager.debug("Looping...");
  for(int index = 0; index < MAX_SENSORS; index++) {
    
    if (settings.device.sensors[index].sensorName.empty()) {
      break;
    }
    if (settings.device.sensors[index].isValid) {
      char message[255];
      snprintf(message, sizeof(message), "Reading sensor %s", settings.device.sensors[index].sensorName.c_str());
      loggerManager.debug(message);
      
      sensors[index]->readData();
    }

    
  }

  delay(settings.device.sleepTimeSeconds * 1000);
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