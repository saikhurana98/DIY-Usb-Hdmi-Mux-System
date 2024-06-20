#include "mux.hpp"

void pulse(int trig_pin, bool state)
{
    digitalWrite(trig_pin, state ? HIGH : LOW);
}

Mux::Mux(int trig_pin, int sense_pin, Scheduler &scheduler, int retries)
{
    this->trigPin = trig_pin;
    this->sensePin = sense_pin;
    this->retries = retries;
    this->scheduler = &scheduler;
    pinMode(trig_pin, OUTPUT);
    pinMode(sense_pin, INPUT);

    this->pulsePinLow = false;
    this->errorFlag = false;
}

Mux::State Mux::getPinState()
{
    return digitalRead(this->sensePin) == HIGH ? HDMI1 : HDMI2;
}

void Mux::setState(State state)
{
    this->currentState = state;
    this->requestedTimestamp = millis();
}

void Mux::runtime()
{

    if (this->errorFlag)
        return;

    // Set Low
    if (this->pulsePinLow && (millis() - this->lastMillis) > 150)
    {
        this->pulse(false);
        this->lastMillis = millis();
        this->pulsePinLow = false;
    }

    if (this->currentState != this->getPinState())
    {
        if (this->pulsePinLow)
        {
            this->pulse(true);
            this->lastMillis = millis();
            this->pulsePinLow = true;
        }
    }
    else
    {
        if (millis() - this->requestedTimestamp > (this->retries * 150))
    }

    

    // Validate
    if (millis() - this->requestedTimestamp > (this->retries * 150))
    {
        this->errorFlag = true;
    }
}

void Mux::pulse(bool pinState)
{
    digitalWrite(this->trigPin, pinState);
}