#include "config.hpp"

Config::Config(int serialRxPin, int serialTxPin, double baud, HdmiChannelPinoutMap &pinouts, RestoreMode restoreMode)
{
    this->serialConfig.baudRate = baud;
    this->serialConfig.txPin = serialTxPin;
    this->serialConfig.rxPin = serialRxPin;
    this->currentRestoreMode = restoreMode;
    this->hdmiChannelPinouts = &pinouts;
}
void Config::init()
{
    this->load();

    if (this->currentRestoreMode == RestoreMode::NONE)
    {
        for (auto const &key_val : *hdmiChannelPinouts)
        {
            this->restoreState[key_val.first.c_str()] = HdmiSource::HDMI1;
        }
    }

    this->save();
}

void Config::save()
{
    if (!LittleFS.exists(this->flashConfigFilename.c_str()))
    {
        Serial.printf("%s not found \n", this->flashConfigFilename.c_str());
    }

    File configFilehandle = LittleFS.open(this->flashConfigFilename.c_str(), "w+");

    if (!configFilehandle)
    {
        Serial.printf("Config File Not Opened! Not Saving\n");
        return;
    }

    JsonDocument configJson;

    for (auto const &key_val : restoreState)
    {
        configJson["config"]["restoreState"][key_val.first.c_str()] = this->hdmiSourceStringMap[key_val.second];
    }

    Serial.printf("Saving Json!!\n");
    serializeJsonPretty(configJson, Serial);
    if (!serializeJson(configJson, configFilehandle))
    {
        Serial.printf("Failed to write to file");
    }
    configFilehandle.close();
}

void Config::load()
{
    LittleFSConfig cfg;
    cfg.setAutoFormat(false);
    LittleFS.setConfig(cfg);

    LittleFS.begin();
    if (!LittleFS.exists(this->flashConfigFilename.c_str()))
    {
        Serial.printf("%s not found \n", this->flashConfigFilename.c_str());
    }

    File configFilehandle = LittleFS.open(this->flashConfigFilename.c_str(), "r+");

    if (!configFilehandle)
    {
        Serial.printf("Config File Not Opened!\n");
    }

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, configFilehandle);
    Serial.printf("Reading Json: Err: %s \n", err ? "Yes" : "No");

    serializeJsonPretty(doc, Serial);

    configFilehandle.close();
    if (err)
    {
        Serial.printf("Failed to read file, using default config\n");
        return;
    }

    for (JsonPair key_value : doc["config"]["restoreState"].as<JsonObject>())
    {
        this->restoreState[key_value.key().c_str()] = this->hdmiStringSourceMap[key_value.value().as<String>().c_str()];
    }
}

void Config::addHdmiPinout(String channelId, int trigPin, int sensePin)
{
    this->hdmiChannelPinouts[0][channelId] = make_pair(trigPin, sensePin);

    if (this->currentRestoreMode = RestoreMode::NONE)
    {
        this->restoreState[channelId] = HdmiSource::HDMI1;
    }
}