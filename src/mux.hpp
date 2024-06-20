#include <Arduino.h>
#include <TaskSchedulerDeclarations.h>

class Mux
{
public:
    enum State
    {
        HDMI1,
        HDMI2
    };

    State currentState;
    void setState(State state);
    void runtime();
    bool errorFlag = false;
    Mux(int trig_pin, int sense_pin, Scheduler &scheduler, int retries);

private:
    Scheduler *scheduler;
    int trigPin;
    int sensePin;
    int retries;
    bool pulsePinLow;
    long long lastMillis;
    long long requestedTimestamp;
    State getPinState();
    void pulse(bool pinState);
};