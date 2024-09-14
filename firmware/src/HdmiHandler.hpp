
#include <Arduino.h>
#include "mux.hpp"

extern std::map<HdmiSource, String> hdmiSourceStringMap;
extern std::map<RestoreMode, String> RestoreModeEnumToString;
extern std::map<String, RestoreMode> RestoreModeStringToEnum;

template <typename GenericSource, typename GenericPinout>
class TemplateHandler
{
    typedef vector<Task *> TaskList;
    typedef std::map<String, BaseMux *> ChannelMuxMap;

private:
    String getStringFromEnum(GenericSource source);
    ChannelMuxMap channels;
    Config *appConfig;

public:
    void init()
    {
        for (auto mux : this->channels)
        {
            mux.second->init();
        }
    }
    TaskList getJobs()
    {
        TaskList jobs;

        if (this->channels.size() == 0)
        {
            Serial.printf("No Tasks were generated!\n");
            return jobs;
        }

        for (auto channel_mux : this->channels)
        {
            jobs.push_back(
                new Task(TASK_IMMEDIATE, TASK_FOREVER, [channel_mux]()
                         { channel_mux.second->runtime(); }));
        }

        // TODO:: Add Task to Restore State on Boot

        return jobs;
    }
    void setSource(String channel, GenericSource source)
    {
        auto *mux = this->channels[channel.c_str()];
        mux->switchSource(source);
    }
    GenericSource getSource(String channel)
    {
        auto *mux = this->channels[channel.c_str()];
    }
    String getSourceString(String channel);
};

/**
 * @brief Handles the HDMI Mux(s) and handles their states.
 *
 */
class HdmiHandler
{
private:
    /**
     * @brief Map of Channel Id and their Mux instances
     */
    std::map<String, TemplateMux<HdmiSource, HdmiPinout> *> channelMuxMap;

    /**
     * @brief Instance of the appConfig
     */
    Config *appConfig;

public:
    /**
     * @brief Get the Jobs to be Scheduled on the TaskScheduler
     *
     * @return std::vector<Task *>
     */
    std::vector<Task *> getJobs();

    /**
     * @brief Get the Mux By the channel Id
     *
     * @param channelId
     * @return Mux*
     */
    TemplateMux<HdmiSource, HdmiPinout> *getMuxById(String channelId);

    /**
     * @brief Set the Boot Restore Mode and State
     *
     * @param mode RestoreMode restore mode to set
     * @param map Restore State (Needed only when Mode == RestoreMode::CUSTOM)
     */
    void setBootRestoreMode(RestoreMode mode, HdmiChannelSourceMap map);

    /**
     * \overload
     * @brief Set the Boot Restore Mode and State
     * @param mode String RestoreMode
     * @param map  JsonDocument channel_state_map
     */
    void setBootRestoreMode(String mode, JsonDocument map);

    /**
     * @brief Set the Source object
     *
     * @param channel String ChannelID
     * @param source HdmiSource Enum source
     */
    void setSource(String channel, HdmiSource source);

    /**
     * \overload
     * @brief Set the Source object
     *
     * @param channel String ChannelID
     * @param source String source ( Uses the map to translate to Enum )
     */
    void setSource(String channel, String source);

    /**
     * @brief Init all the mux instance
     *
     */
    void init();

    /**
     * @brief Get the Current Source as String
     *
     * @param channel channel to get the source for
     * @return String
     */
    String getSourceString(String channel);

    /**
     * @brief Get the Current Source
     *
     * @param channel channel to get the source for
     * @return HdmiSource Current Source
     */
    HdmiSource getSource(String channel);

    /**
     * @brief Construct a new Hdmi Handler object
     *
     * @param config appConfig instance
     */
    HdmiHandler(Config &config);
};
