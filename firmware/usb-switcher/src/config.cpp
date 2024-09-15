
#include "config.hpp"

std::map<UsbSource, String> usbSourceStringMap = {
    {UsbSource::INVALID, "INVALID"},
    {UsbSource::USB1, "USB1"},
    {UsbSource::USB2, "USB2"},
};

UsbChannelSourceMap usbStringSourceMap = {
    {"INVALID", UsbSource::INVALID},
    {"USB1", UsbSource::USB1},
    {"USB2", UsbSource::USB2},
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

Config::Config(int serialRxPin, int serialTxPin, double baud, UsbChannelPinoutMap &pinouts, RestoreMode restoreMode)
{
    this->serialConfig.baudRate = baud;
    this->serialConfig.txPin = serialTxPin;
    this->serialConfig.rxPin = serialRxPin;
    this->currentRestoreMode = restoreMode;
    this->usbChannelPinouts = &pinouts;
}
void Config::init()
{
    LittleFSConfig cfg;
    cfg.setAutoFormat(false);
    LittleFS.setConfig(cfg);
    LittleFS.begin();
    Serial.println("Config init");
    this->load();


    // if restored incorrectly
    if (this->restoreState.size() != this->usbChannelPinouts->size()) 
    {
        Serial.print("Config Incorrectly Loaded, Resetting... ");
        this->currentRestoreMode = RestoreMode::NONE;
        for (auto channel_pinout : *this->usbChannelPinouts)
        {
            String channel  = channel_pinout.first;
            this->restoreState[channel] = UsbSource::USB1;
        }
        this->save();
        Serial.print("\nDone!! Rebooting...\n");
        rp2040.reboot();
    }


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
    configJson["config"]["restoreMode"] = RestoreModeEnumToString[this->currentRestoreMode];

    for (auto const &key_val : restoreState)
    {
        auto source = this->currentRestoreMode == RestoreMode::NONE ? UsbSource::USB2 : key_val.second;
        configJson["config"]["restoreState"][key_val.first.c_str()] = usbSourceStringMap[source];
    }

    if (!serializeJson(configJson, configFilehandle))
    {
        Serial.printf("Failed to write to file");
    }
    Serial.printf("Config Saved!\n");
    configFilehandle.close();
}

void Config::load()
{

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
    Serial.printf("Config Loaded!\n");
    serializeJsonPretty(configJson, Serial);

    configFilehandle.close();
    if (err)
    {
        Serial.printf("Failed to read file, using default config\n");
        return;
    }
    if (configJson["config"].containsKey("restoreMode"))
    {
        this->currentRestoreMode = RestoreModeStringToEnum[configJson["config"]["restoreMode"].as<String>().c_str()];
    }
    for (JsonPair key_value : configJson["config"]["restoreState"].as<JsonObject>())
    {
        this->restoreState[key_value.key().c_str()] = usbStringSourceMap[key_value.value().as<String>().c_str()];
    }
}

void Config::setRestoreState(RestoreMode mode, UsbChannelSourceMap map)
{
    this->currentRestoreMode = mode;
    if (this->currentRestoreMode == RestoreMode::CUSTOM)
    {
        this->restoreState = map;
    }
    this->save();
}