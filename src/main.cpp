#include <TaskScheduler.h>

#include "mux.hpp"
#include "serialHandler.hpp"
#include "config.hpp"

Scheduler *userScheduler = new Scheduler();


Config *appConfig = new Config(13,13,115200,Config::RestoreMode::NONE);


Mux *hdmiMuxArray[3] = {
    new Mux("A",9, 10, 500),
    new Mux("B",18, 19, 500),
    new Mux("C",8, 20, 500)};

SerialHandler *sh = new SerialHandler(13,12,115200,hdmiMuxArray,Serial1);

Task *tasks[] = {
    new Task(TASK_IMMEDIATE,TASK_FOREVER,[](){ hdmiMuxArray[0]->runtime(); }),
    new Task(TASK_IMMEDIATE,TASK_FOREVER,[](){ hdmiMuxArray[1]->runtime(); }),
    new Task(TASK_IMMEDIATE,TASK_FOREVER,[](){ hdmiMuxArray[2]->runtime(); })
};

Task *taskSerialHandlerRuntime = new Task(TASK_IMMEDIATE,TASK_FOREVER,[](){
    sh->runtime();
});


Task *driver_task = new Task(TASK_SECOND * 1, TASK_FOREVER, []()
                            {
                            });



void setup()
{
    sh->init();

    for (int i = 0; i < 3 ; i++)
    {
        hdmiMuxArray[i]->init();
        userScheduler->addTask(*tasks[i]);
        tasks[i]->enable();
    }
    
    // userScheduler->addTask(*driver_task);
    // driver_task->enableDelayed(2000);

    userScheduler->addTask(*taskSerialHandlerRuntime);
    taskSerialHandlerRuntime->enable();
    
    userScheduler->startNow();
}

void loop()
{
    userScheduler->execute();
}
