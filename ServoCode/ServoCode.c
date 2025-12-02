#include <stdio.h>
#include "pico/stdlib.h"
#include "servo.h"
#include "mqtt/mqtt.h"

bool direction=true;
int currentMills=400;
int pin=15;
void servoTask(void *){
    while(true){
        currentMills+=(direction)?5:-5;
        if(currentMills>=2400){
            direction=false;
        }
        else if(currentMills<=400){
            direction=true;
        }
        setMills(pin,currentMills);
        sleep_ms(100);
    }
}
int main()
{
    stdio_init_all();
    setServo(pin,currentMills);
    netEvents=xEventGroupCreate();
    xTaskCreate(wifiTask,"Wifi task",256,NULL,tskIDLE_PRIORITY+1,NULL);
    //xTaskCreate(mqttTask,"Mqtt task",256,NULL,tskIDLE_PRIORITY+1,NULL);
    xTaskCreate(servoTask,"Servo task",256,NULL,tskIDLE_PRIORITY+1,NULL);
    vTaskStartScheduler();
    while(true){}
}
