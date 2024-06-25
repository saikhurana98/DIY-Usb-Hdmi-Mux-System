#pragma once

#include <Arduino.h>
#include <TaskSchedulerDeclarations.h>
#include <ArduinoJson.h>

class Mux
{
public:
    enum Source
    {
        INVALID,
        HDMI1,
        HDMI2
    };
   
    String channel;
    Source currentSource;
    Source requestedSource;
    Source defaultSource;
    String getSourceStringFromEnum(Source source);
    Source getSourceEnumfromString(String source);
    Source getDefaultSourceConfig();
    void setDefaultSourceConfig(Source source);
    void switchSource(Source source);
    void switchSource(String source);
    Source getCurrentSource();
    void init();
    void runtime();
    bool errorFlag = false;
    Mux(String channel,int trig_pin, int sense_pin,long long retryTimeout);


private:
    int trigPin;
    int sensePin;
    long long retryTimeout;
    long long requestedTimestamp;
    Source getPinState();
    void setPinState(bool pinState);
};