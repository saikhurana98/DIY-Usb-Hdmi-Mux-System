#include <Arduino.h>
#include <ArduinoJson.h>
#include "HdmiHandler.hpp"

class SerialHandler
{
private:
    HdmiHandler *hdmiHandler;
    void setSource(JsonDocument &payload);
    JsonDocument getSource(JsonDocument &payload);
    JsonDocument getSourceMulti(JsonDocument &payload);
    void setRestoreMode(JsonDocument &payload);
    String getConfig();
    int rxPin;
    int txPin;
    double baud;
public:
    SerialUART *serial;
    void runtime();
    void init();
    Task * getTask();
    SerialHandler(Config &config ,HdmiHandler &hdmiHandler ,SerialUART &serial);
};
