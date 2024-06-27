
#include <Arduino.h>
#include <ArduinoJson.h>
#include "HdmiHandler.hpp"

/**
 * @brief SerialHandler class manages the serial communication with the Host MCU
 */
class SerialHandler
{
private:
    /**
     * @brief UART Rx Pin Number 
     */
    int rxPin;
    /**
     * @brief UART Tx Pin Number 
     */
    int txPin;
    /**
     * @brief UART baud rate 
     */
    double baud;

    /**
     * @brief Reference to the hardware serial interface
     */
    SerialUART *serial;

    /**
     * @brief Reference to the HdmiHandler Class Instance
     */
    HdmiHandler *hdmiHandler;

    
    /**
     * @brief Handle setSource Command 
     * 
     * @param payload JsonDocument Parsed json payload received over the serial interface
     */
    void setSource(JsonDocument &payload);

    /**
     * @brief Get the GET_SOURCE Payload to return
     * 
     * @param payload JsonDocument Parsed json payload received over the serial interface
     * @return JsonDocument Return Payload
     */
    JsonDocument getSource(JsonDocument &payload);

    /**
     * @brief Get the Source for Multiple Channels
     * 
     * @param payload JsonDocument Parsed json payload received over the serial interface
     * @return JsonDocument Return Paylod
     */
    JsonDocument getSourceMulti(JsonDocument &payload);

    /**
     * @brief Handles SetRestore Mode
     * 
     * @param payload JsonDocument Parsed json payload received over the serial interface
     */
    void setRestoreMode(JsonDocument &payload);
public:

    /**
     * @brief Main Runtime Routing should be called repeatedly in loop or with a task scheduler 
     * 
     *  Listens for a set of valid communication interface and hands off to the respective handler.
     */
    void runtime();

    /**
     * @brief Sets up the serial interface
     * 
     */
    void init();

    /**
     * @brief Get the Task object to be added to TaskScheduler
     * 
     * @return Task* 
     */
    Task * getTask();

    /**
     * @brief Construct a new Serial Handler object
     * 
     * @param config global config instance
     * @param hdmiHandler HdmiHandler class instance
     * @param serial Hardware Serial Controller Interface
     */
    SerialHandler(Config &config ,HdmiHandler &hdmiHandler ,SerialUART &serial);
};
