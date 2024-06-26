#pragma once

#include <Arduino.h>

#define _TASK_STD_FUNCTION

#define ARDUINO_ARCH_STM32
#include <TaskSchedulerDeclarations.h>
#undef ARDUIONO_ARCH_STM32

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
    Mux(String channel, int trig_pin, int sense_pin, long long retryTimeout);
    int trigPin;
    int sensePin;
private:
    long long retryTimeout;
    long long requestedTimestamp;
    HdmiSource getPinState();
    void setPinState(bool pinState);
};