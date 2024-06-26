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
    Config *config;
    bool errorFlag = false;
    int trigPin;
    int sensePin;
    void init();
    void runtime();
    HdmiSource getCurrentSource();
    void switchSource(HdmiSource source);
    void switchSource(String source);
    Mux(String channel, int trig_pin, int sense_pin, long long retryTimeout, Config &config);

private:
    long long retryTimeout;
    long long requestedTimestamp;
    HdmiSource getPinState();
    void setPinState(bool pinState);
};