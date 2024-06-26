#include "serialHandler.hpp"
JsonDocument SerialHandler::getSource(JsonDocument &payload)
{
    String channel = payload["payload"]["channel"];
    JsonDocument returnPayload;
    returnPayload[channel] = this->hdmiHandler->getSourceString(channel);
    serializeJsonPretty(returnPayload, *(this->serial));
    return returnPayload;
}
JsonDocument SerialHandler::getSourceMulti(JsonDocument &payload)
{
    JsonDocument returnPaylod;

    for (JsonVariant channel : payload["payload"]["channels"].as<JsonArray>())
    {
        String channelString = channel.as<String>();
        channelString.replace("\"", "");
        returnPaylod["payload"][channelString] = this->hdmiHandler->getSourceString(channel);
    }

    return returnPaylod;
}

void SerialHandler::setSource(JsonDocument &payload)
{
    JsonObject channels = payload["payload"].as<JsonObject>();

    for (JsonPair channel_source : channels)
    {
        String channel = channel_source.key().c_str();
        String source = channel_source.value().as<const char *>();

        this->hdmiHandler->setSource(channel, source);
    }
}

void SerialHandler::setRestoreMode(JsonDocument &payload)
{
    String mode = payload["mode"].as<String>();
    JsonObject states = payload["payload"];
    this->hdmiHandler->setBootRestoreMode(mode,states);
}
void SerialHandler::runtime()
{
    auto serial = this->serial;
    if (serial->available())
    {
        String serialJsonStringified = serial->readStringUntil('\n');
        serialJsonStringified.trim();
        JsonDocument serialJson;
        JsonDocument outputJson;
        deserializeJson(serialJson, serialJsonStringified);
        serializeJsonPretty(serialJson,*(this->serial));
        this->hdmiHandler->printConfig(*(this->serial));
        JsonDocument returnPayload;
        returnPayload["res"] = true;
        if (serialJson.containsKey("cmd"))
        {

            if (serialJson["cmd"] == "GET_SOURCE")
            {
                returnPayload["payload"] = this->getSource(serialJson);
            }
            else if (serialJson["cmd"] == "GET_SOURCE_MULTI")
            {
                returnPayload["payload"] = this->getSourceMulti(serialJson);
            }
            else if (serialJson["cmd"] == "SET_SOURCE")
            {
                this->setSource(serialJson);
                // wait here and get updated state
            } else if (serialJson["cmd"] == "SET_RESTORE_MODE")
            {
                this->setRestoreMode(serialJson);
            }
            else
            {
                returnPayload["res"] = false;
                returnPayload["payload"]["error"] = "Invalid Key: 'cmd' ";
                returnPayload["payload"]["desc"] = " Valid values for key: 'cmd' are ['GET_SOURCE','GET_SOURCE_MULTI','SET_SOURCE','SET_SOURCE_MULTI']";
            }
        }
        else
        {
            returnPayload["res"] = false;
            returnPayload["payload"]["error"] = "Key not found: 'cmd'";
            returnPayload["payload"]["desc"] = "key:'cmd' must be present";
        }

        String stringifiedReturnPayload;
        serializeJson(returnPayload, stringifiedReturnPayload);
        this->serial->print(stringifiedReturnPayload);
        return;
    }
}

Task *SerialHandler::getTask()
{
    return new Task(TASK_IMMEDIATE, TASK_FOREVER, [this]()
                    { this->runtime(); });
}

void SerialHandler::init()
{
    auto serial = this->serial;
    serial->setRX(this->rxPin);
    serial->setTX(this->txPin);
    serial->begin(this->baud);
    serial->print("Serial Ready");
}

SerialHandler::SerialHandler(Config &config, HdmiHandler &hdmiHandler, SerialUART &serial)
{
    this->serial = &serial;
    this->hdmiHandler = &hdmiHandler;
    this->rxPin = config.serialConfig.rxPin;
    this->txPin = config.serialConfig.txPin;
    this->baud = config.serialConfig.baudRate;
}