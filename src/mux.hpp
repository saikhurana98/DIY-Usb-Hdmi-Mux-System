#pragma once

#include <Arduino.h>
#include <TaskSchedulerDeclarations.h>
#include <ArduinoJson.h>

typedef enum {
    NONE, 
    LAST_STATE,
    CUSTOM
} RestoreMode;

typedef enum {
    INVALID,
    HDMI1,
    HDMI2
} Source;

typedef struct {
    String channel;
    String source;
} ChannelConfig;

typedef struct {
    int numChannels;
    ChannelConfig channels[];
} ChannelsConfig;

struct DefaultConfig
{
    RestoreMode restoreMode;
    ChannelsConfig lastState;
    ChannelConfig customState;
};
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
    DefaultConfig defaultConfig;
    long long retryTimeout;
    long long requestedTimestamp;
    Source getPinState();
    bool loadStoredConfig();
    bool saveStoredConfig();
    void setPinState(bool pinState);
};