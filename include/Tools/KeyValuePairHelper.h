#ifndef KeyValuePairHelper_H
#define KeyValuePairHelper_H

#include <Arduino.h>
#include <string>

#include "const.h"
#include "Models/KeyValuePair.h"

namespace McpNetwork {

    namespace WeatherStation {

        namespace Tools {

            class KeyValuePairHelper {
                public:
                    int FindValue(std::array<Models::IntKeyValuePair, FULL_MAX> *values, int maxSize, std::string key);
                    std::string FindValue(std::array<Models::StringKeyValuePair, FULL_MAX> *values, int maxSize, std::string key);
            };

        }
    }
}

#endif