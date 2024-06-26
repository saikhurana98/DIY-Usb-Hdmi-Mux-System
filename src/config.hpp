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
