#ifndef LoggerModel_H
#define LoggerModel_H

#include <Arduino.h>
#include <string>

namespace McpNetwork {

    namespace WeatherStation {

        namespace Models {

            struct loggerModel {
                int speed;
                std::string logLevel;
            };
        }
    }
}
#endif