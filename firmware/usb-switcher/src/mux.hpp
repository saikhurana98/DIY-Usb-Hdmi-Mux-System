
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


extern std::map<UsbSource, String> usbSourceStringMap;
extern UsbChannelSourceMap usbStringSourceMap;

/**
 * @brief Mux Class.
 * Manages an individual mux switch. Requests for a state change and then validates it. 
 * make sure runtime() is called repeatedly. 
 */
class Mux
{
public:
    /**
     * @brief Channel id of this mux object 
     * 
     */
    String channel;
    /**
     * @brief Current UsbSource 
     * 
    */
    UsbSource currentSource;
    /**
     * @brief Requested UsbSource 
     * 
    */
    UsbSource requestedSource;

    /**
     * @brief is set to true on timeout during runtime()
     * 
     */
    bool errorFlag = false;

    /**
     * @brief Trigger Pin 
     * This pin is pulsed when a different state is requested 
     */
    
    int trigPin;

    /**
     * @brief Sense Pin(s)
     *  Checks for valid state. 
     */
    int senseA;
    int senseB;


    /**
     * @brief Sets pinMode and Inits the class to the current state on hardware.
     * 
    */
    void init();

    /**
     * @brief Main runtime of this class. Call this on repeat. either inside void loop() or using a TaskScheduler
     * Ref: https://github.com/arkhipenko/TaskScheduler
     * 
     */
    void runtime();

    /**
     * @brief Get the Current Hdmi Source
     * 
     * @return UsbSource 
     */
    UsbSource getCurrentSource();

    /**
     * @brief Requests for a Hdmi Source
     * 
     * @param source requested Hdmi Source
     */
    void switchSource(UsbSource source);

    /**
     * @brief Requests for a Hdmi Source
     * \overload  Uses the UsbSourceEnumMap to get the enum val;
     * @param source String Hdmi Source (as String)
     */
    void switchSource(String source);

    /**
     * @brief Construct a new Mux object
     * 
     * @param channel String channel id. must be distinct across other instances 
     * @param trig_pin Trigger digital pin
     * @param sense_a Sense digital pin A
     * @param sense_b Sense digital pin B
     * @param retryTimeout Retry runtime threshold
     */
    Mux(String channel, int trig_pin, int sense_a, int sense_b, long long retryTimeout);

private:
    long long retryTimeout;
    long long requestedTimestamp;
    long long pulseHighTimestamp;
    bool pulsedHigh;
    /**
     * @brief Get the Digital Pin State and return the associated hdmi source
     * 
     * @return HdmiSource 
     */
    UsbSource getPinState();

    /**
     * @brief Set the Digital Pin State
     * 
     * @param pinState 
     */
    void setPinState(bool pinState);
};