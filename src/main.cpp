#define _TASK_STD_FUNCTION

#define ARDUINO_ARCH_STM32 
#include <TaskScheduler.h>
#undef ARDUIONO_ARCH_STM32


#include "serialHandler.hpp"

Scheduler *userScheduler = new Scheduler();

HdmiChannelPinoutMap hdmiPinout = {
    {"A", make_pair(9, 10)},
    {"B", make_pair(18, 19)},
    {"C", make_pair(8, 20)}};

Config *appConfig = new Config(13, 12, 115200, hdmiPinout, RestoreMode::NONE);
HdmiHandler *hdmiHandler = new  HdmiHandler(*appConfig);
SerialHandler *serialHandler = new SerialHandler(*appConfig, *hdmiHandler, Serial1);

void setup()
{
    Serial.begin(115200);
    appConfig->init();
    serialHandler->init();

    std::vector<Task *> jobs;

    for (auto job :hdmiHandler->getJobs()) {
        jobs.push_back(job);
    }
    jobs.push_back(serialHandler->getTask());

    for (auto job :jobs) {
        userScheduler->addTask(*job);
    }
}

void loop()
{
    userScheduler->execute();
}
