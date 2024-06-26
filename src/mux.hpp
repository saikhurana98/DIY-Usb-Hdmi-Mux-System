
#include <Arduino.h>

#define _TASK_STD_FUNCTION

#define ARDUINO_ARCH_STM32
#include <TaskSchedulerDeclarations.h>
#undef ARDUIONO_ARCH_STM32

#include <ArduinoJson.h>
#include "LittleFS.h"
#include "config.hpp"


extern std::map<HdmiSource, String> hdmiSourceStringMap;
extern HdmiChannelSourceMap hdmiStringSourceMap;
class Mux
{
public:
    String channel;
    HdmiSource currentSource;
    HdmiSource requestedSource;
    bool errorFlag = false;
    int trigPin;
    int sensePin;
    void init();
    void runtime();
    HdmiSource getCurrentSource();
    void switchSource(HdmiSource source);
    void switchSource(String source);
    Mux(String channel, int trig_pin, int sense_pin, long long retryTimeout);

private:
    long long retryTimeout;
    long long requestedTimestamp;
    HdmiSource getPinState();
    void setPinState(bool pinState);
};