#ifndef  MeasureModel_H
#define MeasureModel_H

#include <Arduino.h>
#include <string>

#include "const.h"
#include "Models/KeyValuePair.h"

namespace McpNetwork {

    namespace WeatherStation {

        namespace Models {

            class MeasureModel {
                public:
                    MeasureModel();
                    std::string serializeMesaures();
                    
                    int capability;
                    std::string deviceName;
                    std::string dateMeasure;
                    std::array<KeyValuePair<float>, MAX_MEASURES> measures;
                private:
                    McpNetwork::WeatherStation::EnumHelper enumHelper;

            };
        }
    }
}

#endif