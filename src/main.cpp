#include <TaskScheduler.h>

#include "serialHandler.hpp"

Scheduler *userScheduler = new Scheduler();

HdmiChannelPinoutMap hdmiPinout = {
    {"A", make_pair(9, 10)},
    {"B", make_pair(18, 19)},
    {"C", make_pair(8, 20)}};

Config *appConfig = new Config(13, 13, 115200, hdmiPinout, RestoreMode::NONE);



// Mux *hdmiMuxArray[3] = {
//     new Mux("A", 9, 10, 500),
//     new Mux("B", 18, 19, 500),
//     new Mux("C", 8, 20, 500)};

// SerialHandler *sh = new SerialHandler(13, 12, 115200, hdmiMuxArray, Serial1);

// Task *tasks[] = {
//     new Task(TASK_IMMEDIATE, TASK_FOREVER, []()
//              { hdmiMuxArray[0]->runtime(); }),
//     new Task(TASK_IMMEDIATE, TASK_FOREVER, []()
//              { hdmiMuxArray[1]->runtime(); }),
//     new Task(TASK_IMMEDIATE, TASK_FOREVER, []()
//              { hdmiMuxArray[2]->runtime(); })};

// Task *taskSerialHandlerRuntime = new Task(TASK_IMMEDIATE, TASK_FOREVER, []()
//                                           { sh->runtime(); });

Task *driver_task = new Task(TASK_SECOND * 1, TASK_ONCE, []() {
    Serial.printf("Hello, World!");
    appConfig->init();
});

void setup()
{
    Serial.begin(115200);
    // sh->init();

    // for (int i = 0; i < 3; i++)
    // {
    //     hdmiMuxArray[i]->init();
    //     userScheduler->addTask(*tasks[i]);
    //     tasks[i]->enable();
    // }

    userScheduler->addTask(*driver_task);
    driver_task->enableDelayed(5000);

    // userScheduler->addTask(*taskSerialHandlerRuntime);
    // taskSerialHandlerRuntime->enable();

}

void loop()
{
    userScheduler->execute();
}
