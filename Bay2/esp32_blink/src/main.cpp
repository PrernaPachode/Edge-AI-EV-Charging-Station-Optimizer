#include <Arduino.h>
#include<WiFi.h>
#include "State.h"
#include "config.h"
#include "Peripherals.h"
#include"Network.h"
#include "Telemetry.h"
#include "model.h"
#include "edge_ai.h"
#include "optimization.h"
#include "rpc.h"
#include "attributes.h"


void setup()
{
    // initialise serial monitor
    Serial.begin(115200);
    // initialise sensor
    dht.begin();  
    // config esp32 with real time 
    configTime(0,0,"pool.ntp.org", "time.nist.gov");
    // config peripheral pins 
    pinMode(BTN_PLUGIN,INPUT_PULLUP);
    pinMode(BTN_PLUGOUT,INPUT_PULLUP);
    pinMode(RELAY_PIN,OUTPUT);
    pinMode(LED_GREEN,OUTPUT);
    pinMode(LED_YELLOW,OUTPUT);
    pinMode(LED_RED,OUTPUT);

    // connect board to wifi
    connectWiFi();
  


    //Configure MQTT server
    mqtt.setServer(MQTT_SERVER, MQTT_PORT);
    //set call back function upon receiving data from the cloud
    mqtt.setCallback(mqttCallback);
    mqtt.setBufferSize(512);
    
    //Connect board to the cloud
    connectMQTT();

}

unsigned long now;
unsigned long last_print;

void loop()
{

    // listen to incoming request
    mqtt.loop();

    //push data every 5 sec
    now = millis();
    if((now - last_print) > 5000)
    {
        last_print = now;
        // read data from sensor    // voltage,current,temperature,power,bay status
        sample_sensor();
        // run AI to get prediction
        runEdgeAIInference();
        // decide load based on the predictions
        if(manualOverrideActive == 0)
        {

        runOptimization();
        }
        // publish the data
        publishTelemetry();


    }

    plug_status();
    update_led_status();
}

