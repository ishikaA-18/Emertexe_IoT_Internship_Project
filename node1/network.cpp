#include "network.h"
#include "config.h"
static PubSubClient* mqtt                =NULL;
static byte        mac[6]                =NODE_MAC;
static unsigned long lastReconnectTime  =0;
static char thingsboardServer[] = "mqtt.thingsboard.cloud";
static void doConnect(void){
  Serial.print("Connecting to: ");
  Serial.println(TB_SERVER);
  Serial.print("Token: ");
  Serial.println(ACCESS_TOKEN);
  Serial.print("ClientID: ");
  Serial.println(CLIENT_ID);
  
  if(mqtt->connect(CLIENT_ID, ACCESS_TOKEN, NULL)){
    Serial.println("Connected!");
  }
  else{
    Serial.print("mqtt failed, rc=");
    Serial.println(mqtt->state());  // ← THIS IS KEY - tells exact error code
  }
}

void    network_begin(PubSubClient* mqttClient){
  mqtt     =mqttClient;

  //connect board to the internet
  Ethernet.begin(mac);
  delay(2000);

  //connect to things board
  mqtt->setServer(thingsboardServer, TB_PORT);
  doConnect();
}


void  network_maintain(void){
  Ethernet.maintain();

  //if board not connected try reconnection after every 5 seconds
  if(mqtt->connected()){
    mqtt->loop();
  }
  else{
    unsigned long now = millis();
    if( now - lastReconnectTime >= RECONNECT_INTERVAL){
      lastReconnectTime = now;
      doConnect();
    }
  }
}
//return the status of mqtt connection
uint8_t network_isConnected(void){
  return (uint8_t)mqtt->connected();
}
uint8_t network_publish(const char* topic, const char* payload, uint8_t retain){
  return (uint8_t) mqtt->publish(topic,payload,(bool)retain);
}

