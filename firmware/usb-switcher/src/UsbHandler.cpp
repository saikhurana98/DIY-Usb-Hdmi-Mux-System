#include "UsbHandler.hpp"

UsbHandler::UsbHandler(Config &config)
{
    this->appConfig = &config;
    for (auto channel_pinout : *(config.usbChannelPinouts))
    {
        String channel = channel_pinout.first.c_str();
        pair<int, pair<int,int>> pinout = channel_pinout.second;
        Mux *mux = new Mux(channel_pinout.first.c_str(), pinout.first, pinout.second.first,pinout.second.second, 500);
        this->channelMuxMap[channel] = mux;
    }
}

Mux *UsbHandler::getMuxById(String channelId)
{
    if (this->channelMuxMap.count(channelId) != 1)
    {
        return NULL;
    }
    return this->channelMuxMap[channelId];
}

void UsbHandler::setSource(String channel, UsbSource source)
{
    Mux *mux = this->channelMuxMap[channel.c_str()];
    mux->switchSource(source);

    if (this->appConfig->currentRestoreMode == RestoreMode::RESTORE_PREVIOUS) {
        this->appConfig->restoreState[channel] = source;
        this->appConfig->setRestoreState(this->appConfig->currentRestoreMode,this->appConfig->restoreState);
    }
}
void UsbHandler::setSource(String channel, String source)
{
    Mux *mux = this->channelMuxMap[channel.c_str()];
    mux->switchSource(source);

    if (this->appConfig->currentRestoreMode == RestoreMode::RESTORE_PREVIOUS) {
        this->appConfig->restoreState[channel] = usbStringSourceMap[source];
        this->appConfig->setRestoreState(this->appConfig->currentRestoreMode,this->appConfig->restoreState);
    }

}
String UsbHandler::getSourceString(String channel)
{
    Mux *mux = this->getMuxById(channel);
    if (mux == NULL)
    {
        return usbSourceStringMap[UsbSource::INVALID];
    }
    return usbSourceStringMap[mux->currentSource];
}

UsbSource UsbHandler::getSource(String channel)
{
    Mux *mux = this->channelMuxMap[channel.c_str()];
    if (mux == NULL)
        return UsbSource::INVALID;
    return mux->currentSource;
}

void UsbHandler::setBootRestoreMode(RestoreMode mode, UsbChannelSourceMap map)
{
    this->appConfig->setRestoreState(mode, map);
}
void UsbHandler::setBootRestoreMode(String mode, JsonDocument map)
{
    auto restoreMode = RestoreModeStringToEnum[mode.c_str()];
    UsbChannelSourceMap sourceMap;

    if (restoreMode == RestoreMode::CUSTOM)
    {
        for (JsonPair channel_source : map.as<JsonObject>())
        {
            sourceMap[channel_source.key().c_str()] = usbStringSourceMap[channel_source.value().as<const char *>()];
        }
    }
    this->appConfig->setRestoreState(restoreMode, sourceMap);
}


void UsbHandler::init()
{
    for (auto mux : this->channelMuxMap)
    {
        mux.second->init();
    }
}


std::vector<Task *> UsbHandler::getJobs()
{
    std::vector<Task *> jobs;

    if (this->channelMuxMap.size() == 0)
    {
        Serial.printf("No Tasks were generated");
    }

    for (auto channel_mux : this->channelMuxMap)
    {
        jobs.push_back(
            new Task(TASK_MILLISECOND * 50, TASK_FOREVER, [channel_mux]()
                     { channel_mux.second->runtime(); }));
    }

    // Restore Loaded Config States
    if (this->appConfig->currentRestoreMode != RestoreMode::NONE) 
    {
        jobs.push_back(
            new Task(TASK_MILLISECOND * 500, TASK_ONCE, [this]()
                     { 
                        for (auto channel_mux : this->channelMuxMap) {
                            channel_mux.second->switchSource(this->appConfig->restoreState[channel_mux.first.c_str()]);
                        }
                     }));
    }

    return jobs;
}
