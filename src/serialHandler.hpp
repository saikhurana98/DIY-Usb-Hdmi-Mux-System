#include <Arduino.h>
#include <mux.hpp>
#include <ArduinoJson.h>

class SerialHandler
{
private:
    Mux *mux_arr;
    SerialUART *serial;
    void setSource(JsonObject payload);
    JsonObject getSource(JsonObject payload);
    void setSourceMulti(JsonObject payload);
    JsonObject getSourceMulti(JsonObject payload);
    int rxPin;
    int txPin;
    double baud;
public:
    void runtime();
    void init();
    SerialHandler(int rxPin, int txPin, double baud, Mux *muxArr[],SerialUART &serial);
};
