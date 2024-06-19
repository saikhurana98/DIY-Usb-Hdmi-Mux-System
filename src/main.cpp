#include <Arduino.h>
#include "mux.hpp"

Scheduler *userScheduler = new Scheduler();

Task task_pulse_on(TASK_IMMEDIATE, TASK_ONCE, []() {
    Serial.print("...ON");
    digitalWrite(8, HIGH);
    digitalWrite(18, HIGH);
    digitalWrite(9, HIGH);
});

Task task_pulse_off(TASK_IMMEDIATE, TASK_ONCE, []() {
    Serial.print("...OFF\n");
    digitalWrite(8, LOW);
    digitalWrite(18, LOW);
    digitalWrite(9, LOW);
});

Task task_runtime(TASK_SECOND * 2, TASK_FOREVER, []() {
    Serial.print("Pulsing...");
    digitalWrite(25, !digitalRead(25));
    task_pulse_on.restart();
    task_pulse_off.restartDelayed(150);
});



void setup() {
    Serial.begin(115200);

    pinMode(8, OUTPUT);
    pinMode(18, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(25, OUTPUT);

    userScheduler->addTask(task_pulse_on);
    userScheduler->addTask(task_pulse_off);
   
    userScheduler->addTask(task_runtime);
    task_runtime.enable();
    
    userScheduler->startNow();
}

void loop() {
    userScheduler->execute();
}
