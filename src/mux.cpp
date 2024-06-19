#include "mux.hpp"

using namespace std;

void pulse(int trig_pin,bool state)
{
    digitalWrite(trig_pin, state ? HIGH : LOW);
}


Mux::Mux(int trig_pin, int sense_pin, Scheduler &scheduler,int retries)
{
    this->trig_pin = trig_pin;
    this->sense_pin = sense_pin;
    this->retries = retries;
    this->scheduler = &scheduler;
    pinMode(trig_pin, OUTPUT);
    pinMode(sense_pin, INPUT);

    auto pulse_on = std::bind(pulse,trig_pin,true);
    this->task_pulse_on = new Task(0, TASK_ONCE, );


}

Mux::State Mux::getState()
{
    return digitalRead(sense_pin) == HIGH ? HDMI1 : HDMI2;
}

bool Mux::setState(State state)
{
    if (state == getState())
    {
        return false;
    }

    // Pulse the trigger pin
    // Validate the state
    // try 3 times with 100ms of wait time, use the scheduler to do this
    return true;
}