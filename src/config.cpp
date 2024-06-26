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
    configJson["restoreMode"] = this->RestoreModeEnumToString[this->currentRestoreMode];

    for (auto const &key_val : restoreState)
    {
        auto source = this->currentRestoreMode == RestoreMode::NONE ? HdmiSource::HDMI2 : key_val.second;

        configJson["config"]["restoreState"][key_val.first.c_str()] = this->hdmiSourceStringMap[source];
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

    JsonDocument configJson;
    DeserializationError err = deserializeJson(configJson, configFilehandle);
    Serial.printf("Reading Json: Err: %s \n", err ? "Yes" : "No");

    serializeJsonPretty(configJson, Serial);

    configFilehandle.close();
    if (err)
    {
        Serial.printf("Failed to read file, using default config\n");
        return;
    }

    if (configJson["config"].containsKey("restoreMode"))
    {
        this->currentRestoreMode = this->RestoreModeStringToEnum[configJson["config"]["restoreMode"].as<String>().c_str()];
    }

    for (JsonPair key_value : configJson["config"]["restoreState"].as<JsonObject>())
    {
        this->restoreState[key_value.key().c_str()] = this->hdmiStringSourceMap[key_value.value().as<String>().c_str()];
    }
}

void Config::setRestoreState(RestoreMode mode, HdmiChannelSourceMap map)
{
    this->currentRestoreMode = mode;
    if (currentRestoreMode == RestoreMode::CUSTOM) {
        this->restoreState = map;
    }
    this->save();
}