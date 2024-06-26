#pragma once

#include <Arduino.h>
#include "mux.hpp"
#include "config.hpp"

class HdmiHandler
{
private:
    std::map<String, Mux *> channelMuxMap;
    Config *appConfig;

public:
    std::vector<Task *> getJobs();
    Mux *getMuxById(String channelId);
    void setBootRestoreMode(RestoreMode mode);
    void setSource(String channel, HdmiSource source);
    void setSourceByString(String channel, String source);
    String getSourceString(String channel);
    HdmiSource getSource(String channel);
    HdmiHandler(Config &config);
};
