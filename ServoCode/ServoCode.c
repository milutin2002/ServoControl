#include <stdio.h>
#include "pico/stdlib.h"
#include "servo.h"
#include "mqtt/mqtt.h"

bool direction=true;
int currentMills1=400,currentMills2=3000;
#define SERVO_PIN1 12
#define SERVO_PIN2 10



void servoTask(void *){
    while(true){
        enum Action cmd=LEFT;
        BaseType_t res=xQueueReceive(queue,&cmd,0);
        if(res==pdTRUE){
            if(cmd==LEFT){
                if(currentMills1<2400){
                    currentMills1+=50;
                }
                setMills(SERVO_PIN1,currentMills1);
            }
            else if(cmd==RIGHT){
                if(currentMills1>400){
                    currentMills1-=50;
                }
                setMills(SERVO_PIN1,currentMills1);
            }
            else if(cmd==UP){
                printf("Increasing height\n");
                if(currentMills2<2400){
                    currentMills2+=50;
                }
                setMills(SERVO_PIN2,currentMills2);
            }
            else{
                if(currentMills2>400){
                    currentMills2-=50;
                }
                setMills(SERVO_PIN2,currentMills2);
            }
            
        }
        sleep_ms(100);
    }
}
EventGroupHandle_t netEvents;
int main()
{
    stdio_init_all();
    setServo(SERVO_PIN1,currentMills1);
    setServo(SERVO_PIN2,currentMills2);
    netEvents=xEventGroupCreate();
    queue=xQueueCreate(COUNT_QUEUE_LEN,sizeof(enum Action));
    xTaskCreate(wifiTask,"Wifi task",256,NULL,tskIDLE_PRIORITY+1,NULL);
    xTaskCreate(mqttTask,"Mqtt task",256,NULL,tskIDLE_PRIORITY+1,NULL);
    xTaskCreate(servoTask,"Servo task",256,NULL,tskIDLE_PRIORITY+1,NULL);
    vTaskStartScheduler();
    while(true){}
}
