#include "mux.hpp"
#include <TaskScheduler.h>

Scheduler *userScheduler = new Scheduler();



Mux *hdmiMuxArray[3] = {
    new Mux("A",8, 20, 500),
    new Mux("B",18, 19, 500),
    new Mux("C",9, 10, 500)};

Task *tasks[] = {
    new Task(TASK_IMMEDIATE,TASK_FOREVER,[](){ hdmiMuxArray[0]->runtime(); }),
    new Task(TASK_IMMEDIATE,TASK_FOREVER,[](){ hdmiMuxArray[1]->runtime(); }),
    new Task(TASK_IMMEDIATE,TASK_FOREVER,[](){ hdmiMuxArray[2]->runtime(); })
};

Task *driver_task = new Task(TASK_SECOND * 1, TASK_FOREVER, []()
                             {
                                for (auto hdmiMux: hdmiMuxArray)
                                {
                                 auto currentSource = hdmiMux->getCurrentSource();
                                 hdmiMux->switchSource(currentSource == Mux::Source::HDMI1 ? Mux::Source::HDMI2 : Mux::Source::HDMI1);
                                }
                            });






void setup()
{
    Serial.begin(115200);

    for (int i = 0; i < 3 ; i++)
    {
        hdmiMuxArray[i]->init();
        userScheduler->addTask(*tasks[i]);
        tasks[i]->enable();
    }
    userScheduler->addTask(*driver_task);
    driver_task->enableDelayed(3000);

    userScheduler->startNow();
}

void loop()
{
    userScheduler->execute();
}
