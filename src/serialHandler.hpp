#include <Arduino.h>
#include <mux.hpp>
#include <ArduinoJson.h>

class SerialHandler
{
private:
    Mux **mux_arr;
    SerialUART *serial;
    void setSource(JsonDocument &payload);
    JsonDocument getSource(JsonDocument &payload);
    void setSourceMulti(JsonDocument &payload);
    JsonDocument getSourceMulti(JsonDocument &payload);
    Mux *findMuxByChannel(String id);
    int rxPin;
    int txPin;
    double baud;
public:
    void runtime();
    void init();
    SerialHandler(int rxPin, int txPin, double baud, Mux* muxArr[],SerialUART &serial);
};
