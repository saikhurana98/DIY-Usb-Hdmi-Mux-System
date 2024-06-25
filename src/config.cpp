#include "config.hpp"

Config::Config(int serialRxPin = 2, int serialTxPin = 1, double baud = 115200, RestoreMode restoreMode = NONE)
{
    this->serialConfig.baudRate = baud;
    this->serialConfig.txPin = serialTxPin;
    this->serialConfig.rxPin = serialRxPin;
    this->currentRestoreMode = restoreMode;
}

void Config::addHdmiPinout(String channelId, int trigPin, int sensePin)
{
    this->hdmiChannelPinouts[channelId] = {trigPin, sensePin};

    if (this->currentRestoreMode =  Config::NONE) {
        this->restoreState[channelId] = Config::HdmiSource::HDMI1;
    }
}