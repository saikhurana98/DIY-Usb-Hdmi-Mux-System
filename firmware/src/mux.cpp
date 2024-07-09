#include "mux.hpp"

template<>
void TemplateMux<HdmiSource, HdmiPinout>::setPinout()
{
    // Inputs
    pinMode(this->pinout.sense, INPUT);

    // Outputs
    pinMode(this->pinout.trig, OUTPUT);
}

template<>
void TemplateMux<UsbSource, UsbPinout>::setPinout()
{
    // Inputs
    pinMode(this->pinout.sense1, INPUT);
    pinMode(this->pinout.sense2, INPUT);

    // Outputs
    pinMode(this->pinout.trig, OUTPUT);
}

template<>
int TemplateMux<HdmiSource, HdmiPinout>::getSensePinOutput()
{
    return digitalRead(this->pinout.sense) == HIGH ? 1 : 2;
}

template<>
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

template<>
bool TemplateMux<HdmiSource, HdmiPinout>::getTrig()
{
    return digitalRead(this->pinout.trig);
}

template<>
bool TemplateMux<UsbSource, UsbPinout>::getTrig()
{
    return digitalRead(this->pinout.trig);
}

template<>
void TemplateMux<HdmiSource, HdmiPinout>::setTrig(bool state)
{
    digitalWrite(this->pinout.trig, state);
}

template<>
void TemplateMux<UsbSource, UsbPinout>::setTrig(bool state)
{
    digitalWrite(this->pinout.trig, state);
}

template<>
HdmiSource TemplateMux<HdmiSource, HdmiPinout>::getSourceEnum(String sourceString)
{
    std::map<String, HdmiSource> map = {
        {"INVALID", HdmiSource::HDMI_INVALID},
        {"HDMI1", HdmiSource::HDMI1},
        {"HDMI2", HdmiSource::HDMI2},
    };

    return map[sourceString];
}

template<>
UsbSource TemplateMux<UsbSource, UsbPinout>::getSourceEnum(String sourceString)
{
    std::map<String, UsbSource> map = {
        {"INVALID", UsbSource::USB_INVALID},
        {"USB1", UsbSource::USB1},
        {"USB2", UsbSource::USB2},
    };

    return map[sourceString];
}