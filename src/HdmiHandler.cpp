#include "HdmiHandler.hpp"

HdmiHandler::HdmiHandler(Config &config)
{
    for (auto channel_pinout : *config.hdmiChannelPinouts)
    {
        this->channelMuxMap[channel_pinout.first.c_str()] =
            new Mux(channel_pinout.first, channel_pinout.second.first, channel_pinout.second.second, 500);
    }
}

Mux *HdmiHandler::getMuxById(String channelId)
{
    if (this->channelMuxMap.count(channelId) != 1)
        return NULL;
    return this->channelMuxMap[channelId];
}

void HdmiHandler::setSource(String channel, HdmiSource source)
{
    Mux *mux = this->channelMuxMap[channel.c_str()];
    // if (mux == NULL) return false;
    mux->requestedSource = source;
}
void HdmiHandler::setSourceByString(String channel, String source)
{
    Mux *mux = this->channelMuxMap[channel.c_str()];
    
    mux->requestedSource = this->appConfig->hdmiStringSourceMap[source];
}
String HdmiHandler::getSourceString(String channel)
{
    Mux *mux = this->channelMuxMap[channel.c_str()];
    if (mux == NULL) return this->appConfig->hdmiSourceStringMap[HdmiSource::INVALID]; 
    return this->appConfig->hdmiSourceStringMap[mux->currentSource];  
}

HdmiSource HdmiHandler::getSource(String channel)
{
    Mux *mux = this->channelMuxMap[channel.c_str()];
    if (mux == NULL) return HdmiSource::INVALID; 
    return mux->currentSource;  
}


void HdmiHandler::setBootRestoreMode(RestoreMode mode)
{
    this->appConfig->currentRestoreMode = mode;
}

std::vector<Task *> HdmiHandler::getJobs()
{
    std::vector<Task *> jobs;

    if (this->channelMuxMap.size() == 0) {
        Serial.printf("No Tasks were generated");
    } 

    for (auto channel_mux : this->channelMuxMap)
    {
        jobs.push_back(
            new Task(TASK_IMMEDIATE,TASK_ONCE,[channel_mux](){
                channel_mux.second->runtime();
            })
        );
    }

    return jobs;
}

