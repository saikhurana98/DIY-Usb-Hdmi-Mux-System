
#include <Arduino.h>
#include "mux.hpp"


extern std::map<HdmiSource, String> hdmiSourceStringMap;
extern std::map<RestoreMode, String> RestoreModeEnumToString;
extern std::map<String, RestoreMode> RestoreModeStringToEnum;
class HdmiHandler
{
private:
    std::map<String, Mux *> channelMuxMap;
    Config *appConfig;

public:
    std::vector<Task *> getJobs();
    Mux *getMuxById(String channelId);
    void setBootRestoreMode(RestoreMode mode, HdmiChannelSourceMap map);
    void setBootRestoreMode(String mode, JsonDocument map);
    void setSource(String channel, HdmiSource source);
    void setSource(String channel, String source);
    void init();
    String getSourceString(String channel);
    HdmiSource getSource(String channel);
    void runtime();
    HdmiHandler(Config &config);
};
