#include <TaskScheduler.h>

class Mux
{
private:
    Scheduler *scheduler;
    int trig_pin;
    int sense_pin;
    int retries;
    void pulse();
    Task *task_pulse_on;
    Task *task_pulse_off;
public:
    enum State
    {
        HDMI1,
        HDMI2
    };

    State getState();
    bool setState(State state);
    Mux( int trig_pin, int sense_pin, Scheduler &scheduler, int retries);
};