#include "actuator.h"
static unsigned long lastBlinkTime=0;
static uint8_t blinkstate=0;
void actuators_begin(void){
  pinMode(PIN_LED_GREEN,OUTPUT);
  pinMode(PIN_LED_YELLOW,OUTPUT);
  pinMode(PIN_LED_RED,OUTPUT);
  pinMode(PIN_RELAY, OUTPUT);
  //in the begining till network connection LED be on
  digitalWrite(PIN_LED_RED,HIGH);
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