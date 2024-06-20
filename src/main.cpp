#include "mux.hpp"
#include <TaskScheduler.h>

Scheduler *userScheduler = new Scheduler();
Mux *hdmiMux1 = new Mux(9,14,*userScheduler,3);

Task *t1 = new Task(TASK_IMMEDIATE,TASK_FOREVER,[](){
    hdmiMux1->runtime();
});


void setup() {
    Serial.begin(115200);
    userScheduler->addTask(*t1);
    userScheduler->enable();
    userScheduler->startNow();
}

void loop() {
    userScheduler->execute();
}
