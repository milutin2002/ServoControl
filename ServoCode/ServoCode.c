#include <stdio.h>
#include "pico/stdlib.h"
#include "servo.h"
#include "mqtt/mqtt.h"

bool direction=true;
int currentMills=400;
int pin=15;



void servoTask(void *){
    while(true){
        enum Action cmd=LEFT;
        BaseType_t res=xQueueReceive(queue,&cmd,0);
        if(res==pdTRUE){
            if(cmd==LEFT){
                if(currentMills<2400){
                    currentMills+=50;
                }
            }
            else if(cmd==RIGHT){
                if(currentMills>400){
                    currentMills-=50;
                }
            }
            setMills(pin,currentMills);
        }
        sleep_ms(100);
    }
}
EventGroupHandle_t netEvents;
int main()
{
    stdio_init_all();
    setServo(pin,currentMills);
    netEvents=xEventGroupCreate();
    queue=xQueueCreate(COUNT_QUEUE_LEN,sizeof(enum Action));
    xTaskCreate(wifiTask,"Wifi task",256,NULL,tskIDLE_PRIORITY+1,NULL);
    xTaskCreate(mqttTask,"Mqtt task",256,NULL,tskIDLE_PRIORITY+1,NULL);
    xTaskCreate(servoTask,"Servo task",256,NULL,tskIDLE_PRIORITY+1,NULL);
    vTaskStartScheduler();
    while(true){}
}
