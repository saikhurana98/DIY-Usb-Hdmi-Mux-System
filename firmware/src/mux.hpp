
#include <Arduino.h>

#define _TASK_STD_FUNCTION

/*
 Even thought the code is written for Raspberry Pi RP2040
 We're setting this Flag for standard functions to be available as callbacks for TaskScheduler Tasks
 Ref: https://github.com/arkhipenko/TaskScheduler/issues/184
 */
#define ARDUINO_ARCH_STM32
#include <TaskSchedulerDeclarations.h>
#undef ARDUIONO_ARCH_STM32

#include <ArduinoJson.h>
#include "LittleFS.h"
#include "config.hpp"

extern std::map<HdmiSource, String> hdmiSourceStringMap;
extern HdmiChannelSourceMap hdmiStringSourceMap;

typedef struct
{
    int trig;
    int sense;
} HdmiPinout;
typedef struct
{
    int trig;
    int sense1;
    int sense2;
} UsbPinout;

class BaseMux{
    public:
    virtual void init();
    virtual void runtime();
};

/**
 * @brief Creates the Class for either an Hdmi Mux or a USB Mux. Since HDMI Mux uses a single sense pin and the USB Mux uses two sense pins
 *
 * @tparam GenericSourceType One of ["HdmiSource","UsbSource"]
 * @tparam GenericPinout One of ["HdmiPinout","UsbPinout"]
 */
template <typename GenericSourceType, typename GenericPinout>
/**
 * @brief  Abstract Mux Class.
 * Manages an individual mux switch. Requests for a state change and then validates it.
 * make sure runtime() is called repeatedly.
 */
class TemplateMux : public BaseMux
{
public:
    /**
     * @brief Channel id
     *
     */
    String channel;
    /**
     * @brief Current HdmiSource
     *
     */
    GenericSourceType currentSource;
    /**
     * @brief Requested HdmiSource
     *
     */
    GenericSourceType requestedSource;

    /**
     * @brief is set to true on timeout during runtime()
     *
     */
    bool errorFlag = false;

    /**
     * @brief GPIO Pins physically wired up
     *
     */
    GenericPinout pinout;

    /**
     * @brief Sets pinMode and Inits the class to the current state on hardware.
     *
     */
    void init() {
        this->setPinout();
        this->currentSource = this->getHWSource();
        this->requestedTimestamp = this->currentSource;
        this->errorFlag = false;
    }

    /**
     * @brief Main runtime of this class. Call this on repeat. either inside void loop() or using a TaskScheduler
     * Ref: https://github.com/arkhipenko/TaskScheduler
     *
     */
    void runtime() {
        if (this->errorFlag)
        {
            return;
        }

        if (this->requestedSource == this->getHWSource()) 
        {
            this->currentSource = this->requestedSource;
            this->requestedTimestamp = millis();
            this->setTrig(0);
            return;
        }


        if (!this->getTrig()) 
        {
            this->setTrig(1);
            return;
        }


        if (millis() - this->requestedTimestamp > this->retryTimeout)
        {
            return;
        }

        this->errorFlag = true;
        this->currentSource = static_cast<GenericSourceType>(0);
    }

    /**
     * @brief Get the Current Source
     *
     * @return GenericSourceType
     */
    GenericSourceType getCurrentSource() {
        return this->currentSource;
    }

    /**
     * @brief Requests for a Hdmi Source
     *
     * @param source requested Hdmi Source
     */
    void switchSource(GenericSourceType source) {
        if (source == 0)
        {
            return;
        }
        this->requestedSource = source;
        this->requestedTimestamp = millis();
    }

    /**
     * @brief Requests for a Hdmi Source
     * \overload  Uses the GenericSourceTypeEnumMap to get the enum val;
     * @param source String Hdmi Source (as String)
     */
    void switchSource(String source) {
        auto sourceEnum = this->getSourceEnum(source);
        this->switchSource(sourceEnum);
    }

    /**
     * @brief Get the Source Enum object
     *  To be Implemented for all source types
     * @param sourceString String Source as String
     * @return GenericSourceType 
     */
    GenericSourceType getSourceEnum(String sourceString);


    /**
     * @brief Construct a new Mux object
     *
     * @param channel String channel id. must be distinct across other instances
     * @param pinout GPIO Pins for the Mux
     * @param retryTimeout Retry runtime threshold
     */
    TemplateMux(String channel, GenericPinout pinout, long long retryTimeout)
    {
        this->channel = channel;
        this->pinout = pinout;
        this->retryTimeout = retryTimeout;
    };

private:
    long long retryTimeout;
    long long requestedTimestamp;

    /**
     * @brief Get's the Current Source According to the hardware 
     *
     * @return GenericSourceType
     */
    GenericSourceType getHWSource() {
        return static_cast<GenericSourceType>(this->getSensePinOutput());
    }

    /**
     * @brief Gets the Current Source On Hardware
     * Each Type Must implement it's own hardware check mechanism
     * @return int 
     */
    int getSensePinOutput();

    /**
     * @brief Set the Trig object
     *  To be implemented individually for respective types
     * @param state Digital HIGH or LOW ( 1 or 0 )
     */
    void setTrig(bool state);

    /**
     * @brief Get the Trig Pin State
     * 
     * @return true 
     * @return false 
     */
    bool getTrig();

    /**
     * @brief Calls PinMode for the respective GPIO
     * To be implemented individually for respective yypes
     */
    void setPinout();


};
