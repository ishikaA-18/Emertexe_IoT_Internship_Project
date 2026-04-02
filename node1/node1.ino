//include required libraries
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <DHT.h>
#include "config.h"
#include "SensorManager.h"
#include "actuator.h"
#include "network.h"
#include "telemetry.h"
static char sharedBuf[200];
//create object of type ethernet pubsubclient
static EthernetClient ethClient;
static PubSubClient mqttClient(ethClient);
unsigned long lastTelemetry=0;
void setup() 
{
  Serial.begin(9600);
  Serial.print("Node1 is booting....");
  // put your setup code here, to run once:
   // initialise sensor 
   sensor_begin();
   //initialise actutors
   actuators_begin();
  telemetry_init(sharedBuf,sizeof(sharedBuf));
   // coonect board to internet and mqtt
  network_begin(&mqttClient);
}
void loop() 
{
  // put your main code here, to run repeatedly:
  //keep checking if board is connected to thingsBoard
  network_maintain();
  //read data from the sensors 
  SensorData data;
  sensors_read(&data);
  //publish to the cloud 
  //check id data available then push to thingsBoard
  if(network_isConnected()){
    unsigned long now=millis();
    //publish data to things every 5 seconds
    if(now-lastTelemetry>=TELEMETRY_INTERVAL){
      lastTelemetry=now;
      telemetry_publishTelemetry(&data,1);//converting sensor into json and publish
    }
  }
  actuators_updateStatusLEDs(network_isConnected(),data.sensorError);
}
