#include "mux.hpp"
#include "LittleFS.h"

void pulse(int trig_pin, bool state)
{
    digitalWrite(trig_pin, state);
}



Mux::Mux(String channel, int trig_pin, int sense_pin, long long retryTimeout, Config &config)
{
    this->channel = channel;
    this->trigPin = trig_pin;
    this->sensePin = sense_pin;
    this->retryTimeout = retryTimeout;
    this->config = &config;
}

HdmiSource Mux::getPinState()
{
    return digitalRead(this->sensePin) == HIGH ? HdmiSource::HDMI1 : HdmiSource::HDMI2;
}

void Mux::switchSource(HdmiSource source)
{
    if (source == HdmiSource::INVALID) return;
    this->requestedSource = source;
    this->requestedTimestamp = millis();
}
void Mux::switchSource(String source)
{
    HdmiSource sourceEnum = this->config->hdmiStringSourceMap[source];
    if (sourceEnum == HdmiSource::INVALID) return;
    this->requestedSource = sourceEnum;
    this->requestedTimestamp = millis();
}
HdmiSource Mux::getCurrentSource()
{
    return this->currentSource;
}


void Mux::init()
{
    pinMode(this->trigPin, OUTPUT);
    pinMode(this->sensePin, INPUT);
    this->currentSource = this->getPinState();
    this->requestedSource = this->currentSource;
    this->errorFlag = false;
}


void Mux::runtime()
{
    // Don't Execute if we have an error
    if (this->errorFlag)
        return;

    // if the state is the same then just return 
    if (this->requestedSource == this->getPinState())
    {
        this->currentSource = this->requestedSource;
        this->requestedTimestamp = millis();
        this->setPinState(false);
        return;
    }
 
    if (!digitalRead(this->trigPin))
    {
        this->setPinState(true);
        return;
    }

    // if we haven't exceeded the timeout then continue processing
    if (millis() - this->requestedTimestamp > this->retryTimeout) {
        return;
    }


    this->errorFlag = true;
    this->currentSource = HdmiSource::INVALID;
}

void Mux::setPinState(bool pinState)
{
    digitalWrite(this->trigPin, pinState);
}