#include <Arduino.h>
#include <ArduinoJson.h>
#include "HdmiHandler.hpp"

class SerialHandler
{
private:
    HdmiHandler *hdmiHandler;
    SerialUART *serial;
    void setSource(JsonDocument &payload);
    JsonDocument getSource(JsonDocument &payload);
    void setSourceMulti(JsonDocument &payload);
    JsonDocument getSourceMulti(JsonDocument &payload);
    int rxPin;
    int txPin;
    double baud;
public:
    void runtime();
    void init();
    SerialHandler(int rxPin, int txPin, double baud, HdmiHandler &hdmiHandler ,SerialUART &serial);
};
