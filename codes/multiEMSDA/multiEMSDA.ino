//2018/8/10
//multi-ch EMS
//Michi Kono, U-Tokyo


#include "Channel.h"
#include <StandardCplusplus.h>
#include <vector>
using namespace std;

//500 Hz max for voltage in slave

#include <Wire.h>

#define ON 1
#define OFF 0

//the duration of each channel, pulse width
#define DELAYCHANNEL 400

vector<Channel> channels;


/////////////////

//this is the only part you should modify

int channelNum = 4; //how many channels your using

/////////////////


int chargePin = channelNum+2;
int dischargePin = channelNum+3;

void setup(){
  Serial.begin(115200);
  Wire.begin();
  
  for(int i=0; i<channelNum; i++){
    channels.push_back(Channel(100, 100, 50, OFF, 1));
    pinMode(i+2, OUTPUT); //do not use TX/RX pins
  }
  
  //charging pin
  pinMode(chargePin, OUTPUT);

  //discharging pin
  pinMode(dischargePin, OUTPUT);
  
}


void loop(){

  //if serial recieved, read and write to i2c
    if(Serial.available() > ((channelNum*5)-1)){

      //start charging
      digitalWrite(chargePin, HIGH);  
      //discharge
      digitalWrite(dischargePin, HIGH);
      delayMicroseconds(200);
      digitalWrite(dischargePin, LOW);
      
      //read from serial
      for(int i=0;i<channelNum;i++){
    
         channels[i].setPulse(Serial.read());
         channels[i].setFrequency(Serial.read());
         channels[i].setVoltage(Serial.read());
         channels[i].setState(Serial.read());
         channels[i].setDuration(Serial.read()*100);
  
      }
      
      //send to slave, voltage info. and duration
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
      channels[i].channelOut(i);
    }
    
  //one pulse relevant for the freq
  //microsec calculation
    delayMicroseconds( (1000000/channels[0].getFrequency()) - (DELAYCHANNEL*channelNum) );
  
  }
  
  //stop charging
  digitalWrite(chargePin, LOW);
  
}
