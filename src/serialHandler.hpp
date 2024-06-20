#include <Arduino.h>
#include <mux.hpp>
#include <ArduinoJson.h>

class SerialHandler
{
private:
    Mux *mux_arr;
    void setSource(JsonObject payload);
    JsonObject getSource(JsonObject payload);
    void setSourceMulti(JsonObject payload);
    JsonObject getSourceMulti(JsonObject payload);

public:
    void runtime();
    void init();
    SerialHandler(Mux *muxArr[]);
};
