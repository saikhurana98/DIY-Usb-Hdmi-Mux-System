#include "mux.hpp"

void TemplateMux<HdmiSource, HdmiPinout>::setPinout()
{
    // Inputs
    pinMode(this->pinout.sense, INPUT);

    // Outputs
    pinMode(this->pinout.trig, OUTPUT);
}

void TemplateMux<UsbSource, UsbPinout>::setPinout()
{
    // Inputs
    pinMode(this->pinout.sense1, INPUT);
    pinMode(this->pinout.sense2, INPUT);

    // Outputs
    pinMode(this->pinout.trig, OUTPUT);
}

int TemplateMux<HdmiSource, HdmiPinout>::getSensePinOutput()
{
    return digitalRead(this->pinout.sense) == HIGH ? 1 : 2;
}

int TemplateMux<UsbSource, UsbPinout>::getSensePinOutput()
{
    bool sense_1 = digitalRead(this->pinout.sense1);
    bool sense_2 = digitalRead(this->pinout.sense2);
    if (sense_1)
        return 1;
    if (sense_2)
        return 2;
    return 0;
}

bool TemplateMux<HdmiSource, HdmiPinout>::getTrig()
{
    return digitalRead(this->pinout.trig);
}
bool TemplateMux<UsbSource, UsbPinout>::getTrig()
{
    return digitalRead(this->pinout.trig);
}

void TemplateMux<HdmiSource, HdmiPinout>::setTrig(bool state)
{
    digitalWrite(this->pinout.trig, state);
}

void TemplateMux<UsbSource, UsbPinout>::setTrig(bool state)
{
    digitalWrite(this->pinout.trig, state);
}

HdmiSource TemplateMux<HdmiSource, HdmiPinout>::getSourceEnum(String sourceString)
{
    std::map<String, HdmiSource> map = {
        {"INVALID", HdmiSource::INVALID},
        {"HDMI1", HdmiSource::HDMI1},
        {"HDMI2", HdmiSource::HDMI2},
    };

    return map[sourceString];
}

UsbSource TemplateMux<UsbSource, UsbPinout>::getSourceEnum(String sourceString)
{
    std::map<String, UsbSource> map = {
        {"INVALID", UsbSource::INVALID},
        {"USB1", UsbSource::USB1},
        {"USB2", UsbSource::USB2},
    };

    return map[sourceString];
}