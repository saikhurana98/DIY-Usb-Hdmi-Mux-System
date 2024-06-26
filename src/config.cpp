
#include "config.hpp"

std::map<HdmiSource, String> hdmiSourceStringMap = {
    {HdmiSource::INVALID, "INVALID"},
    {HdmiSource::HDMI1, "HDMI1"},
    {HdmiSource::HDMI2, "HDMI2"},
};

HdmiChannelSourceMap hdmiStringSourceMap = {
    {"INVALID", HdmiSource::INVALID},
    {"HDMI1", HdmiSource::HDMI1},
    {"HDMI2", HdmiSource::HDMI2},
};
std::map<RestoreMode, String> RestoreModeEnumToString = {
    {RestoreMode::NONE, "NONE"},
    {RestoreMode::RESTORE_PREVIOUS, "RESTORE_PREVIOUS"},
    {RestoreMode::CUSTOM, "CUSTOM"},
};
std::map<String, RestoreMode> RestoreModeStringToEnum = {
    {"NONE", RestoreMode::NONE},
    {"RESTORE_PREVIOUS", RestoreMode::RESTORE_PREVIOUS},
    {"CUSTOM", RestoreMode::CUSTOM},
};




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
    LittleFSConfig cfg;
    cfg.setAutoFormat(false);
    LittleFS.setConfig(cfg);
    LittleFS.begin();
    Serial1.println("Config init");
    this->load();
    this->save();
}

void Config::save()
{
    Serial1.printf("Saving the file at %s\n", this->flashConfigFilename.c_str());
    if (!LittleFS.exists(this->flashConfigFilename.c_str()))
    {
        Serial1.printf("%s not found \n", this->flashConfigFilename.c_str());
    }

    File configFilehandle = LittleFS.open(this->flashConfigFilename.c_str(), "w+");

    if (!configFilehandle)
    {
        Serial1.printf("Config File Not Opened! Not Saving\n");
        return;
    }

    JsonDocument configJson;
    configJson["config"]["restoreMode"] = RestoreModeEnumToString[this->currentRestoreMode];

    for (auto const &key_val : restoreState)
    {
        auto source = this->currentRestoreMode == RestoreMode::NONE ? HdmiSource::HDMI2 : key_val.second;

        configJson["config"]["restoreState"][key_val.first.c_str()] = hdmiSourceStringMap[source];
    }

    Serial1.printf("Saving Json!!\n");
    serializeJsonPretty(configJson, Serial1);
    if (!serializeJson(configJson, configFilehandle))
    {
        Serial1.printf("Failed to write to file");
    }
    configFilehandle.close();
}

void Config::load()
{
    
    Serial1.printf("Loading the file at %s", this->flashConfigFilename.c_str());
    if (!LittleFS.exists(this->flashConfigFilename.c_str()))
    {
        Serial1.printf("%s not found \n", this->flashConfigFilename.c_str());
    }

    File configFilehandle = LittleFS.open(this->flashConfigFilename.c_str(), "r+");

    if (!configFilehandle)
    {
        Serial1.printf("Config File Not Opened!\n");
    }

    JsonDocument configJson;
    DeserializationError err = deserializeJson(configJson, configFilehandle);
    Serial1.printf("Reading Json: Err: %s \n", err ? "Yes" : "No");

    serializeJsonPretty(configJson, Serial1);

    configFilehandle.close();
    if (err)
    {
        Serial1.printf("Failed to read file, using default config\n");
        return;
    }

    if (configJson["config"].containsKey("restoreMode"))
    {
        this->currentRestoreMode = RestoreModeStringToEnum[configJson["config"]["restoreMode"].as<String>().c_str()];
    }

    for (JsonPair key_value : configJson["config"]["restoreState"].as<JsonObject>())
    {
        this->restoreState[key_value.key().c_str()] = hdmiStringSourceMap[key_value.value().as<String>().c_str()];
    }
}

void Config::setRestoreState(RestoreMode mode, HdmiChannelSourceMap map)
{
    this->currentRestoreMode = mode;
    if (this->currentRestoreMode == RestoreMode::CUSTOM) {
        this->restoreState = map;
    }
    this->save();
}