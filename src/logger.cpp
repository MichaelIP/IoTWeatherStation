
#include <Arduino.h>

#include "logger.h"

void McpNetwork::WeatherStation::Logger::LoggerManager::initialize(int speed) {

   Models::loggerModel lSettings;
   lSettings.logLevel = ELogLevel::All;
   lSettings.speed = speed;
  initialize(&lSettings);

}

void McpNetwork::WeatherStation::Logger::LoggerManager::initialize(Models::loggerModel *lSettings) {

  settings = lSettings;
  Serial.flush(); 
  Serial.begin(settings->speed);
  while(Serial.available()) Serial.read(); 

}

void McpNetwork::WeatherStation::Logger::LoggerManager::debug(const char *message) {
    trace(McpNetwork::WeatherStation::ELogLevel::Debug, message);
}

void McpNetwork::WeatherStation::Logger::LoggerManager::info(const char *message) {
    trace(McpNetwork::WeatherStation::ELogLevel::Info, message);
}

void McpNetwork::WeatherStation::Logger::LoggerManager::warn(const char *message) {
    trace(McpNetwork::WeatherStation::ELogLevel::Warn, message);
}

void McpNetwork::WeatherStation::Logger::LoggerManager::error(const char *message) {
    trace(McpNetwork::WeatherStation::ELogLevel::Error, message);
}

void McpNetwork::WeatherStation::Logger::LoggerManager::fatal(const char *message) {
    trace(McpNetwork::WeatherStation::ELogLevel::Fatal, message);
}

void McpNetwork::WeatherStation::Logger::LoggerManager::trace(McpNetwork::WeatherStation::ELogLevel logLevel, const char *message) {

  if (canLogMessage(logLevel)) {
    
    enumHelper enumHelper;

    char fullMessage[255];
    std::string currentDateTime = ntpHelper.getDateTime();
    snprintf(fullMessage, sizeof(fullMessage), "%s | [%s] | %s",  currentDateTime.c_str(), enumHelper.getLogLevel(logLevel).c_str(), message);
    Serial.println(fullMessage);
   	
  }

}

bool McpNetwork::WeatherStation::Logger::LoggerManager::canLogMessage(McpNetwork::WeatherStation::ELogLevel requestedLevel) {
  return requestedLevel >= logLevel;
}
