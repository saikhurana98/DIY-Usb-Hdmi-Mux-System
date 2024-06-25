#include "mux.hpp"
#include "LittleFS.h"

void pulse(int trig_pin, bool state)
{
    digitalWrite(trig_pin, state ? HIGH : LOW);
}



Mux::Mux(String channel, int trig_pin, int sense_pin, long long retryTimeout)
{
    this->channel = channel;
    this->trigPin = trig_pin;
    this->sensePin = sense_pin;
    this->retryTimeout = retryTimeout;
}

HdmiSource Mux::getPinState()
{
    return digitalRead(this->sensePin) == HIGH ? HDMI1 : HDMI2;
}

HdmiSource Mux::getDefaultSourceConfig()
{
    return this->defaultSource;
}
void Mux::setDefaultSourceConfig(HdmiSource source)
{
    if(source == INVALID) return;
    this->defaultSource = source;
    // TODO: Save current source to flash;
}

void Mux::switchSource(HdmiSource source)
{
    if (source == INVALID) return;
    this->requestedSource = source;
    this->requestedTimestamp = millis();
}
void Mux::switchSource(String source)
{
    HdmiSource sourceEnum = this->getSourceEnumfromString(source);
    if (sourceEnum == INVALID) return;
    this->requestedSource = sourceEnum;
    this->requestedTimestamp = millis();
}
HdmiSource Mux::getCurrentSource()
{
    return this->currentSource;
}

String Mux::getSourceStringFromEnum(HdmiSource source)
{
    if (source == HdmiSource::HDMI1) return "HDMI1";
    else if (source == HdmiSource::HDMI2) return "HDMI2";
    else return "INVALID";
}

HdmiSource Mux::getSourceEnumfromString(String source)
{
    if (source == "HDMI1") return HdmiSource::HDMI1;
    else if (source == "HDMI2") return HdmiSource::HDMI2;
    else return HdmiSource::INVALID;
}

void Mux::init()
{
    pinMode(this->trigPin, OUTPUT);
    pinMode(this->sensePin, INPUT);
    this->currentSource = this->getPinState();
    this->requestedSource = this->currentSource;
    this->errorFlag = false;

    // TODO: Load config from flash else load default;
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
    this->currentSource = INVALID;
}

void Mux::setPinState(bool pinState)
{
    digitalWrite(this->trigPin, pinState);
}