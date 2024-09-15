#include "mux.hpp"


Mux::Mux(String channel, int trig_pin, int sense_a, int sense_b, long long retryTimeout)
{
    this->channel = channel;
    this->trigPin = trig_pin;
    this->senseA = sense_a;
    this->senseB = sense_b;
    this->retryTimeout = retryTimeout;
    this->pulsedHigh = false;
}

UsbSource Mux::getPinState()
{
    bool val_sense_a = digitalRead(this->senseA);
    bool val_sense_b = digitalRead(this->senseB);

    if (val_sense_a != val_sense_b)
    {
       return val_sense_a == true ? UsbSource::USB1 : UsbSource::USB2;
    } 
    return UsbSource::INVALID;
}

void Mux::switchSource(UsbSource source)
{
    if (source == UsbSource::INVALID) return;
    this->requestedSource = source;
    this->requestedTimestamp = millis();
}
void Mux::switchSource(String source)
{
    UsbSource sourceEnum = usbStringSourceMap[source];
    if (sourceEnum == UsbSource::INVALID) return;
    this->requestedSource = sourceEnum;
    this->requestedTimestamp = millis();
}
UsbSource Mux::getCurrentSource()
{
    return this->currentSource;
}


void Mux::init()
{
    pinMode(this->trigPin, OUTPUT);
    pinMode(this->senseA, INPUT);
    pinMode(this->senseB, INPUT);
    this->currentSource = this->getPinState();
    this->requestedSource = this->currentSource;
    this->errorFlag = false;
}


void Mux::runtime() 
{
    if (this->currentSource != this->requestedSource) 
    {
        if (this->pulsedHigh) 
        {
            if ( (millis() - this->pulseHighTimestamp) > 180) 
            {
                this->setPinState(false);
                this->pulsedHigh = false;
            }   

        } else 
        {
            this->setPinState(true);
            this->pulsedHigh = true;
            this->pulseHighTimestamp = millis();
        }
    }

    this->currentSource = this->getPinState();
}

void Mux::setPinState(bool pinState)
{
    digitalWrite(this->trigPin, pinState);
}