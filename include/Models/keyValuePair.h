#ifndef KeyValuePair_H
#define KeyValuePair_H

#include <Arduino.h>
#include <string>

namespace McpNetwork {

    namespace WeatherStation {

        namespace Models {

            struct StringKeyValuePair {
                std::string key;
                std::string value;
            };

            struct IntKeyValuePair {
                std::string key;
                int value;
            };

            template <class T>
            class KeyValuePair {
                std::string key;
                T value;
            };

        }
    }

}

#endif