package com.example.nottificationapp

import android.annotation.SuppressLint
import android.util.Log
import com.hivemq.client.mqtt.MqttClient
import com.hivemq.client.mqtt.datatypes.MqttQos
import com.hivemq.client.mqtt.mqtt3.Mqtt3AsyncClient
import java.nio.charset.StandardCharsets


val topicSet=  "pico/alarm/set"
val topicStatus ="pico/alarm/status"

object MqttController {
    private var client: Mqtt3AsyncClient? =null

    fun createClient(brokerHost: String,brokerPort:Int = 1883,clientId: String="android-"+ System.currentTimeMillis()){
        if(client!=null){
            return
        }
        Log.i("Creating","Creating client")
        client= MqttClient.builder().useMqttVersion3().identifier(clientId).serverHost(brokerHost).serverPort(brokerPort).sslWithDefaultConfig().buildAsync()
    }

    fun connect(onConnected:()->Unit, onError:(Throwable)-> Unit){
        val c=client?: return onError(IllegalStateException("Client not created"))
        c.connect().whenComplete { _,t ->
            if(t!=null){
                onError(t)
            }
            else{
                onConnected()
            }
        }

    }


    fun publishData(topic:String,onError: (Throwable) -> Unit){
        val c=client?: return onError(IllegalStateException("Client not created"))
        c.publishWith().topic(topic).qos(MqttQos.AT_MOST_ONCE).payload("ON".toByteArray(
            StandardCharsets.UTF_8)).send().whenComplete { _,t->{
            if(t!=null){
                onError(t)
            }
        } }
    }
    fun disconnect(){
        client?.disconnect()
        client=null
    }
}