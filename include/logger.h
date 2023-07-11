#ifndef Logger_H
#define Logger_H

#include <Arduino.h>
#include <string>

#include "ntp.h"
#include "enums.h"
#include "Models/loggerModel.h"

namespace McpNetwork {

    namespace WeatherStation {

        namespace Logger {

            class LoggerManager {
                public:

                    LoggerManager() { };

                    void initialize(int speed);
                    void initialize(Models::loggerModel *settings);

                    void debug(const char *message);
                    void info(const char *message);
                    void warn(const char *message);
                    void error(const char *message);
                    void fatal(const char *message);

                    ELogLevel logLevel;

                private:

                    Tools::NtpHelper ntpHelper;
                    Models::loggerModel *settings;

                    void trace(McpNetwork::WeatherStation::ELogLevel logLevel, const char *message);
                    bool canLogMessage(McpNetwork::WeatherStation::ELogLevel requestedLevel);
            };
        }
    }
}
#endif