#include "actuator.h"
static uint8_t blinkstate=0;
static unsigned long lastBlinkTime=0;
static uint8_t relay_status;
void actuators_begin(void){
  pinMode(PIN_LED_GREEN,OUTPUT);//EVERYTHING IS NORMAL
  pinMode(PIN_LED_YELLOW,OUTPUT);//if blinking then something critical is happening
  pinMode(PIN_RELAY, OUTPUT);//to control ventilation if humidity is more
  //in the begining till network connection LED be on
  digitalWrite(PIN_LED_GREEN,LOW);
  digitalWrite(PIN_LED_YELLOW,LOW);
  digitalWrite(PIN_RELAY,LOW);
}

//from rpc request is to set relay then turn on the relay
void    actuators_setRelay(uint8_t on){
  relay_status=on;
  digitalWrite(PIN_RELAY,on ? HIGH:LOW);
}
//send back the status of relay then turn on the relay
uint8_t actuators_getRelayState(void){
  return relay_status;
}
void    actuators_updateStatusLEDs(uint8_t mqttConnected, uint8_t sensorError){
  //if board not connected to things board-red led on
  if(!mqttConnected){
    digitalWrite(PIN_LED_RED,HIGH);
    digitalWrite(PIN_LED_GREEN,LOW);
    digitalWrite(PIN_LED_YELLOW,LOW);
    return;
  }
  //if mqtt up-trurn off red led
    digitalWrite(PIN_LED_RED,LOW);
  //if sensor error-yellow led blinks
  if(sensorError){
    digitalWrite(PIN_LED_GREEN,HIGH);
    unsigned long now=millis();
    if(now-lastBlinkTime>500){
      lastBlinkTime=now;
      blinkstate=!blinkstate;
      digitalWrite(PIN_LED_YELLOW,blinkstate?HIGH:LOW);
    }
  }
   else{//if everything normal-green led on
      digitalWrite(PIN_LED_GREEN,HIGH);
      digitalWrite(PIN_LED_YELLOW,LOW);
  }
}