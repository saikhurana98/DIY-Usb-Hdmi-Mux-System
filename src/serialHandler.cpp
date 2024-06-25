#include "serialHandler.hpp"

JsonDocument SerialHandler::getSource(JsonDocument &payload)
{
    String channel = payload["payload"]["channel"];
    JsonDocument returnPayload;
    returnPayload[channel] = this->hdmiHandler->getSourceString(channel);
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
    String channel = payload["payload"].as<JsonPair>() .key().c_str();
    String source = payload["payload"].as<JsonPair>().value().as<const char*>();
    this->hdmiHandler->setSourceByString(channel,source);
}

void SerialHandler::setSourceMulti(JsonDocument &payload)
{
    JsonObject channels = payload["payload"]["channels"].as<JsonObject>();
    
    for (JsonPair channel_source : channels) {
        String channel = channel_source.key().c_str();
        String source = channel_source.value().as<const char*>();

        this->hdmiHandler->setSourceByString(channel,source);
    }
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
            }
            else if (serialJson["cmd"] == "SET_SOURCE_MULTI")
            {
                this->setSourceMulti(serialJson);
                // wait here and get updated state
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


void SerialHandler::init()
{
    auto serial = this->serial;
    serial->setRX(this->rxPin);
    serial->setTX(this->txPin);
    serial->begin(this->baud);
    serial->print("Serial Ready");
}

SerialHandler::SerialHandler(int rxPin, int txPin, double baud, HdmiHandler &hdmiHandler ,SerialUART &serial)
{
    this->serial = &serial;
    this->hdmiHandler = &hdmiHandler;
    this->rxPin = rxPin;
    this->txPin = txPin;
    this->baud = baud;
}