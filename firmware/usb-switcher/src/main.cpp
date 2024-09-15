#define _TASK_STD_FUNCTION

#define ARDUINO_ARCH_STM32 
#include <TaskScheduler.h>
#undef ARDUIONO_ARCH_STM32

#include "serialHandler.hpp"

Scheduler *userScheduler = new Scheduler();

UsbChannelPinoutMap usbPinout = {
    {"A", make_pair(7, make_pair(27,28))}};

Config *appConfig = new Config(9, 8, 115200, usbPinout, RestoreMode::NONE);
UsbHandler *usbHandler = new  UsbHandler(*appConfig);
SerialHandler *serialHandler = new SerialHandler(*appConfig, *usbHandler, Serial2);

void setup()
{
    serialHandler->init();
    Serial.printf("Booting");
    double start = millis();
    while ((millis() - start) < 2000) {

        delay(750);
        Serial.printf(".");
    }
    appConfig->init();
    usbHandler->init();

    std::vector<Task *> jobs;

    for (auto job : usbHandler->getJobs()) {
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
