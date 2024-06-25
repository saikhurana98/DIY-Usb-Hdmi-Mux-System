#pragma once

#include <Arduino.h>
#include <TaskSchedulerDeclarations.h>
#include <ArduinoJson.h>
#include "config.hpp"

class Mux
{
public:
    
   
    String channel;
    HdmiSource currentSource;
    HdmiSource requestedSource;
    HdmiSource defaultSource;
    String getSourceStringFromEnum(HdmiSource source);
    HdmiSource getSourceEnumfromString(String source);
    HdmiSource getDefaultSourceConfig();
    void setDefaultSourceConfig(HdmiSource source);
    void switchSource(HdmiSource source);
    void switchSource(String source);
    HdmiSource getCurrentSource();
    void init();
    void runtime();
    bool errorFlag = false;
    Mux(String channel,int trig_pin, int sense_pin,long long retryTimeout);


private:
    int trigPin;
    int sensePin;
    long long retryTimeout;
    long long requestedTimestamp;
    HdmiSource getPinState();
    void setPinState(bool pinState);
};