#include "mqtt.h"




static void onTopic(void *arg,const char *topic,u32_t len){
    printf("MQTT incoming topic %s\n and len is %d",topic,len);
}
static void onData(void *arg,const u8_t *data,u16_t len,u8_t flags){
    char buf[8];
}

static void onConnect(mqtt_client_t *client,void *arg,mqtt_connection_status_t st){
    if(st==MQTT_CONNECT_ACCEPTED){
        printf("MQTT connected\n");
        mqtt_set_inpub_callback(mq,onTopic,onData,NULL);
        mqtt_subscribe(mq,TOPIC_LEFT,0,NULL,NULL);
        mqtt_subscribe(mq,TOPIC_RIGHT,0,NULL,NULL);
    }
    else{
        printf("Mqtt connection failed\n");
    }
}

void mqttTask( void * _){
   // printf("%p\n",(void *)netEvents);
    //xEventGroupWaitBits(netEvents,NET_READY_BIT,pdFALSE,pdTRUE,portMAX_DELAY);
    printf("Entering mqtt task\n");
    ip_addr_t broker_ip;
    err_t de=netconn_gethostbyname(MQTT_HOST,&broker_ip);
    if(de!=ERR_OK){
        while(true){
            printf("DNS failed (%d)\n",(int)de);
        }
        vTaskDelete(NULL);
        return;
    }
    printf("Broker %s -> %s\n",MQTT_HOST,ipaddr_ntoa(&broker_ip));
    mq=mqtt_client_new();
    struct mqtt_connect_client_info_t ci;
    memset(&ci,0,sizeof(ci));
    ci.client_id="pico-led-1";
    ci.keep_alive=30;
    ci.will_topic=TOPIC_LEFT;
    ci.will_msg="ON";
    ci.will_qos=0;
    ci.will_retain=1;
    while(true){
        if(!mqtt_client_is_connected(mq)){
            printf("MQTT connecting\n");
           // cyw43_arch_lwip_begin();
            mqtt_client_connect(mq,&broker_ip,MQTT_PORT,onConnect,NULL,&ci);
            //cyw43_arch_lwip_end();
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}