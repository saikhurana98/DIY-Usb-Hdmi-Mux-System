#include <ArduinoJson.h>
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

class Config
{
public:
    enum HdmiSource
    {
        INVALID,
        HDMI1,
        HDMI2
    };

    enum RestoreMode
    {
        NONE,
        LAST_STATE,
        CUSTOM
    };

    std::map<HdmiSource, String> hdmiSourceStringMap = {
        {HdmiSource::INVALID, "INVALID"},
        {HdmiSource::HDMI1, "HDMI1"},
        {HdmiSource::HDMI2, "HDMI2"},
    };
    std::map<RestoreMode, String> RestoreModeStringMap = {
        {RestoreMode::NONE, "NONE"},
        {RestoreMode::LAST_STATE, "LAST_STATE"},
        {RestoreMode::CUSTOM, "CUSTOM"},
    };
    SerialConfig serialConfig;
    std::map<String, pair<int, int>> hdmiChannelPinouts;
    std::map<String, HdmiSource> restoreState;
    RestoreMode currentRestoreMode;
    void addHdmiPinout(String channelId, int trigPin, int sensePin);
    void load();
    void save();
    Config(int serialTxPin, int serialRxPin, double baud, RestoreMode restoreMode);

private:
};
