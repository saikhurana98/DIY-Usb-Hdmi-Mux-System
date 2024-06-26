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
    serialHandler->init();
    Serial1.printf("Booting");
    double start = millis();
    while ((millis() - start) < 5000) {

        delay(750);
        Serial1.printf(".");
    }
    appConfig->init();
    hdmiHandler->init();

    std::vector<Task *> jobs;

    for (auto job :hdmiHandler->getJobs()) {
        jobs.push_back(job);
    }
    jobs.push_back(serialHandler->getTask());

    for (auto job :jobs) {
        userScheduler->addTask(*job);
        job->enable();
    }
}

void loop()
{
    userScheduler->execute();
}
