#include <stdio.h>
#include "pico/stdlib.h"
#include "servo.h"

bool direction=true;
int currentMills=400;
int pin=15;
int main()
{
    stdio_init_all();
    setServo(pin,currentMills);
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
