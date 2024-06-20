#include <Arduino.h>
#include <TaskSchedulerDeclarations.h>

class Mux
{
public:
    enum Source
    {
        HDMI1,
        HDMI2
    };

    Source currentSource;
    Source requestedSource;
    void switchSource(Source source);
    Source getCurrentSource();
    void init();
    void runtime();
    bool errorFlag = false;
    Mux(int trig_pin, int sense_pin,long long retryTimeout);


private:
    int trigPin;
    int sensePin;
    long long retryTimeout;
    long long requestedTimestamp;
    Source getPinState();
    void setPinState(bool pinState);
};