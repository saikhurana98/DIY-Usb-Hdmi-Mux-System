#pragma once


#include <ArduinoJson.h>
#include <LittleFS.h>
#include <Arduino.h>
#include <vector>
#include <map>

using namespace std;

typedef struct
{
    int txPin;
    int rxPin;
    double baudRate;
} SerialConfig;


enum HdmiSource
{
    INVALID,
    HDMI1,
    HDMI2
};

enum RestoreMode
{
    NONE,
    RESTORE_PREVIOUS,
    CUSTOM
};

typedef std::map<String, pair<int, int>> HdmiChannelPinoutMap;
typedef std::map<String, HdmiSource> HdmiChannelSourceMap;
class Config
{
public:
    std::map<HdmiSource, String> hdmiSourceStringMap = {
        {HdmiSource::INVALID, "INVALID"},
        {HdmiSource::HDMI1, "HDMI1"},
        {HdmiSource::HDMI2, "HDMI2"},
    };
    std::map<String, HdmiSource> hdmiStringSourceMap = {
        {"INVALID", HdmiSource::INVALID},
        {"HDMI1", HdmiSource::HDMI1},
        {"HDMI2", HdmiSource::HDMI2},
    };
    std::map<RestoreMode, String> RestoreModeEnumToString = {
        {RestoreMode::NONE, "NONE"},
        {RestoreMode::RESTORE_PREVIOUS, "RESTORE_PREVIOUS"},
        {RestoreMode::CUSTOM, "CUSTOM"},
    };
    std::map<String, RestoreMode> RestoreModeStringToEnum = {
        {"NONE",RestoreMode::NONE},
        {"RESTORE_PREVIOUS",RestoreMode::RESTORE_PREVIOUS},
        {"CUSTOM",RestoreMode::CUSTOM},
    };
    SerialConfig serialConfig;
    HdmiChannelPinoutMap *hdmiChannelPinouts;
    HdmiChannelSourceMap restoreState;
    RestoreMode currentRestoreMode;
    void setRestoreState(RestoreMode mode, HdmiChannelSourceMap map);
    void addHdmiPinout(String channelId, int trigPin, int sensePin);
    void init();
    Config(int serialRxPin, int serialTxPin, double baud, HdmiChannelPinoutMap &pinout, RestoreMode restoreMode);

private:
    String flashConfigFilename = "/config.json";
    void load();
    void save();
};
