#include "config.hpp"

Config::Config(int serialRxPin , int serialTxPin , double baud , HdmiChannelPinout &pinout , RestoreMode restoreMode)
{
    this->serialConfig.baudRate = baud;
    this->serialConfig.txPin = serialTxPin;
    this->serialConfig.rxPin = serialRxPin;
    this->currentRestoreMode = restoreMode;
    this->hdmiChannelPinouts = &pinout;
}

void Config::load()
{
    File configFilehandle = LittleFS.open(this->flashConfigFilename.c_str(),"r");

    if (!configFilehandle) {
        Serial.printf("Config File Not Opened!");
    }

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc,configFilehandle);

    if (err) {
        Serial.printf("Failed to read file, using default config");
    }

    for (JsonPair key_value: doc["config"]["restoreState"].as<JsonObject>()) {
        this->restoreState[key_value.key().c_str()] = this->hdmiStringSourceMap[key_value.value().as<String>().c_str()];
    }

}

void Config::addHdmiPinout(String channelId, int trigPin, int sensePin)
{
    this->hdmiChannelPinouts[0][channelId] =  make_pair(trigPin,sensePin);

    if (this->currentRestoreMode =  Config::NONE) {
        this->restoreState[channelId] = Config::HdmiSource::HDMI1;
    }
}