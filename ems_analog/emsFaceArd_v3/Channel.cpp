#include "Channel.h"
#include <Arduino.h>

#define DELAYCHANNEL 400


Channel::Channel(int pulseTemp, int frequencyTemp, int voltageTemp, int stateTemp, int durationTemp){
  pulse = pulseTemp;
  frequency = frequencyTemp;
  voltage = voltageTemp;
  state = stateTemp;
  duration = durationTemp;
}

void Channel::initialize(){ 

}


void Channel::setPulse(int pulseTemp){
  pulse = pulseTemp;
}

void Channel::setFrequency(int frequencyTemp){
  frequency = frequencyTemp;
}

void Channel::setVoltage(int voltageTemp){
  voltage = voltageTemp;
}

void Channel::setState(int stateTemp){
  state = stateTemp;
}

void Channel::setDuration(int durationTemp){
  duration = durationTemp;
}

int Channel::getPulse(){
  return pulse;
}

int Channel::getFrequency(){
  return frequency;
}
  
int Channel::getVoltage(){
  return voltage;
}

int Channel::getState(){
  return state;
}

long Channel::getDuration(){
  return duration;
}


int Channel::channelOut(int channelNumber, long startSec){

    if(state == 1){   
      digitalWrite(channelNumber+2,HIGH);
      delayMicroseconds(pulse);
      digitalWrite(channelNumber+2,LOW);
      delayMicroseconds(DELAYCHANNEL-pulse);
    }else if(state == 0){
      digitalWrite(channelNumber+2,LOW);
      delayMicroseconds(DELAYCHANNEL);
    }
    
}
