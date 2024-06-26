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
    JsonDocument getSourceMulti(JsonDocument &payload);
    int rxPin;
    int txPin;
    double baud;
public:
    void runtime();
    void init();
    Task * getTask();
    SerialHandler(Config &config ,HdmiHandler &hdmiHandler ,SerialUART &serial);
};
