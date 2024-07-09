#include "mux.hpp"

 
void AbstractMux<HdmiSource, HdmiPinout>::setPinout()
{
    // Inputs
    pinMode(this->pinout.sense,INPUT);

    // Outputs
    pinMode(this->pinout.trig,OUTPUT);
}

void AbstractMux<UsbSource, UsbPinout>::setPinout()
{
    // Inputs
    pinMode(this->pinout.sense1,INPUT);
    pinMode(this->pinout.sense2,INPUT);

    // Outputs
    pinMode(this->pinout.trig,OUTPUT);
}

int AbstractMux<HdmiSource, HdmiPinout>::getSensePinOutput()
{
    return digitalRead(this->pinout.sense) == HIGH ? 1 : 2;
}

int AbstractMux<UsbSource, UsbPinout>::getSensePinOutput()
{
    bool sense_1 = digitalRead(this->pinout.sense1);
    bool sense_2 = digitalRead(this->pinout.sense2);
    if (sense_1) return 1;
    if (sense_2) return 2;
    return 0;
}

bool AbstractMux<HdmiSource, HdmiPinout>::getTrig()
{
    return digitalRead(this->pinout.trig);
}
bool AbstractMux<UsbSource, UsbPinout>::getTrig()
{
    return digitalRead(this->pinout.trig);
}

void AbstractMux<HdmiSource, HdmiPinout>::setTrig(bool state)
{
    digitalWrite(this->pinout.trig,state);
}

void AbstractMux<UsbSource, UsbPinout>::setTrig(bool state)
{
    digitalWrite(this->pinout.trig,state);
}




void Mux::switchSource(String source)
{
    HdmiSource sourceEnum = hdmiStringSourceMap[source];
    if (sourceEnum == HdmiSource::INVALID)
        return;
    this->requestedSource = sourceEnum;
    this->requestedTimestamp = millis();
}





void Mux::setPinState(bool pinState)
{
    digitalWrite(this->trigPin, pinState);
}