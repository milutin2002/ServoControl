package com.example.controlservo

import android.os.Bundle
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.IntrinsicSize
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxHeight
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.ArrowBack
import androidx.compose.material.icons.filled.ArrowForward
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import com.example.controlservo.ui.theme.ControlServoTheme
import com.example.nottificationapp.MqttController
import io.reactivex.plugins.RxJavaPlugins
import kotlinx.coroutines.Dispatchers

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        RxJavaPlugins.setErrorHandler { e ->
            android.util.Log.e("RxJava", "Unhandled error", e)
        }
        MqttController.createClient("broker.emqx.io", brokerPort = 8883)
        MqttController.connect(onConnected = {
            Log.i("Success","You connect to broker")
        }, onError = {
            Log.e("Error",it.message.toString())
        })
        setContent {
            ControlServoTheme {
                Scaffold(modifier = Modifier.fillMaxSize()) { innerPadding ->
                    Column{
                        Row(
                            modifier = Modifier.fillMaxWidth().fillMaxHeight(),
                            horizontalArrangement = Arrangement.Center,
                            verticalAlignment = Alignment.CenterVertically
                        ) {
                            IconButton(onClick = {
                                MqttController.publishData("pico/servo/left", onError = {
                                    print(it)
                                })
                            }) {
                                Icon(Icons.Default.ArrowBack, contentDescription = null)
                            }
                            IconButton(onClick = {
                                MqttController.publishData("pico/servo/right", onError = {
                                    print(it)
                                })
                            }) {
                                Icon(Icons.Default.ArrowForward, contentDescription = null)
                            }
                        }
                    }
                }
            }
        }
    }
}

@Composable
fun Greeting(name: String, modifier: Modifier = Modifier) {
    Text(
        text = "Hello $name!",
        modifier = modifier
    )
}

@Preview(showBackground = true)
@Composable
fun GreetingPreview() {
    ControlServoTheme {
        Greeting("Android")
    }
}