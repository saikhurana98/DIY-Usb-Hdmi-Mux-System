#include "serialHandler.hpp"

JsonDocument SerialHandler::getSource(JsonDocument &payload)
{
    String channel = payload["payload"]["channel"];
    Mux *foundMux = this->findMuxByChannel(channel);
    JsonDocument returnPayload;
    if (foundMux == NULL) {
        returnPayload["res"] = false;
        return returnPayload;
    }
    returnPayload["channel"] = channel;
    returnPayload["source"] = foundMux->getSourceStringFromEnum(foundMux->getCurrentSource());
    return returnPayload;
}
JsonDocument SerialHandler::getSourceMulti(JsonDocument &payload)
{
    JsonArray channelsArray = payload["payload"]["channels"].as<JsonArray>();
    JsonDocument returnPaylod;
    JsonDocument buffer;
    JsonArray results = buffer.to<JsonArray>();

    for (JsonVariant channel : channelsArray)
    {
        String channelString = channel.as<String>();
        channelString.replace("\"", "");
        JsonDocument reqPayload;
        reqPayload["payload"]["channel"] = channelString;
        results.add(this->getSource(reqPayload));
    }

    returnPaylod["payload"] = results;
    return returnPaylod;
}

void SerialHandler::setSource(JsonDocument &payload)
{
    String channel = payload["payload"]["channel"];
    Mux *foundMux = this->findMuxByChannel(channel);
    if (foundMux == NULL)
        return;
    foundMux->switchSource(foundMux->getSourceEnumfromString(payload["payload"]["source"]));
    return;
}

void SerialHandler::setSourceMulti(JsonDocument &payload)
{
    JsonArray channelsArray = payload["payload"]["channels"].as<JsonArray>();
    for (JsonDocument channel : channelsArray)
    {
        JsonDocument doc;
        doc["payload"] = channel;
        this->setSource(doc);
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
            }
            else if (serialJson["cmd"] == "SET_SOURCE_MULTI")
            {
                this->setSourceMulti(serialJson);
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

Mux *SerialHandler::findMuxByChannel(String channel)
{
    for (int i = 0; this->mux_arr[i] != NULL; i++)
    {
        if (this->mux_arr[i]->channel == channel)
        {
            return this->mux_arr[i];
        }
    }
    return NULL;
}

void SerialHandler::init()
{
    auto serial = this->serial;
    serial->setRX(this->rxPin);
    serial->setTX(this->txPin);
    serial->begin(this->baud);
    serial->print("Serial Ready");
}

SerialHandler::SerialHandler(int rxPin, int txPin, double baud, Mux *muxArr[], SerialUART &serial)
{
    this->serial = &serial;
    this->mux_arr = muxArr;
    this->rxPin = rxPin;
    this->txPin = txPin;
    this->baud = baud;
}