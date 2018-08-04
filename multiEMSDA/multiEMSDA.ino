//2018/8/3
//multi-ch EMS
//Michi Kono, U-Tokyo


#include "Channel.h"
#include <StandardCplusplus.h>
#include <vector>
using namespace std;

//500 Hz max for voltage

#include <Wire.h>

#define ON 1
#define OFF 0

//the duration of each channel, pulse width
#define DELAYCHANNEL 400

vector<Channel> channels;

int channelNum = 4;


void setup(){
  Serial.begin(115200);
  Wire.begin();
  //Serial.begin(9600);
  
  for(int i=0; i<channelNum; i++){
    channels.push_back(Channel(100, 100, 50, OFF, 1));
    pinMode(i+2,OUTPUT);
  }
  
  //charging pin
  pinMode(channelNum+3,OUTPUT);

  
}


void loop(){

  //if serial recieved, read and write to i2c
    if(Serial.available() > ((channelNum*5)-1)){

      //start charging
      digitalWrite(channelNum+3, HIGH);  
     
      //read from serial
      for(int i=0;i<channelNum;i++){
    
         channels[i].setPulse(Serial.read());
         channels[i].setFrequency(Serial.read());
         channels[i].setVoltage(Serial.read());
         channels[i].setState(Serial.read());
         channels[i].setDuration(Serial.read()*100);
  
      }
      
      //send to slave
      Wire.beginTransmission(1);
  
      for(int i=0;i<channelNum;i++){
        Wire.write(channels[i].getVoltage());
        Wire.write(channels[i].getState());
        Wire.write((int)channels[i].getDuration()/100);
      }
      
      Wire.endTransmission();     
      
      //confirm channel output
      channelWrite();

  }  

}


//write for each channel
void channelWrite(){
  
  //duration*10 for debug
  
  //this must be updated for individual controls of durations
  for(int t=0; t< ( (channels[0].getDuration()) / (1000/channels[0].getFrequency()) ) ;t++){
    
    for(int i=0;i<channelNum;i++){            
      channels[i].channelOut(i,0);
    }
    
  //one pulse relevant for the freq
  //microsec calculation
    delayMicroseconds( (1000000/channels[0].getFrequency()) - (DELAYCHANNEL*channelNum) );
  
  }
  
  //stop charging
  digitalWrite(channelNum+3, LOW);
  
}
