#include <ArduinoJson.h>
#include <LittleFS.h>
#include <Arduino.h>
#include <vector>
#include <map>

using namespace std;

typedef struct
{
    int txPin;
    int rxPin;
    double baudRate;
} SerialConfig;

enum HdmiSource
{
    INVALID,
    HDMI1,
    HDMI2
};

enum RestoreMode
{
    NONE,
    RESTORE_PREVIOUS,
    CUSTOM
};

/**
 * @brief Map of Channel id and pinout pair (trig,sense)
 * 
 */
typedef std::map<String, pair<int, int>> HdmiChannelPinoutMap;

/**
 * @brief Map of Channel id and Hdmi Source enum
 * 
 */
typedef std::map<String, HdmiSource> HdmiChannelSourceMap;


/**
 * @brief Main Application Configuration Class 
 * 
 * This must contain all user configurable parameters ( Pinout, baud_rate, default mux behaviour, etc)
 * 
 */
class Config
{
public:
    /**
     * @brief Serial Configuration Parameters
    */
    SerialConfig serialConfig;

    /**
     * @brief Reference to a map of Channel id to pinout
     */
    HdmiChannelPinoutMap *hdmiChannelPinouts;

    /**
     * @brief State to set the Mux's to on boot. Loaded from flash on boot
     */
    HdmiChannelSourceMap restoreState;


    /**
     * @brief Current Restore Mode. 
     * NONE : Restored to HDMI2
     * CUSTOM : Custom Restore state from flash
     * RESTORE_PREVIOUS : Restored to the last known boot state
     */
    RestoreMode currentRestoreMode;

    /**
     * @brief Sets the on-boot RestoreMode & Source Map in flash.
     * 
     * @param mode RestoreMode ( NONE, CUSTOM, RESTORE_PREVIOUS )
     * @param map Source Map ( Only needed when RestoreMode::Custom )
     */
    void setRestoreState(RestoreMode mode, HdmiChannelSourceMap map);
    /** 
     * @brief Loads the Saved config from flash or loads default and saves it
    */
    void init();
    /**
     * @brief Construct a new Config object
     * 
     *  Contains all the configuration variables needed to construct all dependent classes.
     * 
     * @param serialRxPin Tx Pin of the Control Serial Interface
     * @param serialTxPin Tx Pin of the Control Serial Interface
     * @param baud Serial transmit Baud rate 
     * @param pinout HdmiChannelPinMap Map of the channel id and the pinout pair (trig, sense)
     * @param restoreMode Default Restore Mode if the config is not present
     */
    Config(int serialRxPin, int serialTxPin, double baud, HdmiChannelPinoutMap &pinout, RestoreMode restoreMode);
private:
    String flashConfigFilename = "/config.json";
    void save();
    void load();
};
