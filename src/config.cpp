#include "config.hpp"

Config::Config(int serialRxPin , int serialTxPin , double baud , HdmiChannelPinout &pinout , RestoreMode restoreMode)
{
    this->serialConfig.baudRate = baud;
    this->serialConfig.txPin = serialTxPin;
    this->serialConfig.rxPin = serialRxPin;
    this->currentRestoreMode = restoreMode;
}

void Config::addHdmiPinout(String channelId, int trigPin, int sensePin)
{
    this->hdmiChannelPinouts[0][channelId] =  make_pair(trigPin,sensePin);

    if (this->currentRestoreMode =  Config::NONE) {
        this->restoreState[channelId] = Config::HdmiSource::HDMI1;
    }
}