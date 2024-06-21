#include "serialHandler.hpp"



void SerialHandler::runtime()
{
    auto serial = this->serial;
    if (serial->available())
    {
        String serialJsonStringified = serial->readString();
        JsonDocument serialJson;
        deserializeJson(serialJson,serialJsonStringified);
        String testBuffer;
        serializeJsonPretty(serialJson,testBuffer);
        serial->print(testBuffer);
    }
}

void SerialHandler::init()
{
    auto serial = this->serial;
    serial->setRX(this->rxPin);
    serial->setTX(this->txPin);
    serial->begin(this->baud);
}

SerialHandler::SerialHandler(int rxPin, int txPin, double baud, Mux *muxArr[],SerialUART &serial)
{
    this->serial = &serial;
}