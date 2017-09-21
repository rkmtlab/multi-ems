//2017/9/21
//multi-ch EMS
//Michi Kono, U-Tokyo


#include "Channel.h"
#include <StandardCplusplus.h>
#include <vector>
using namespace std;

//500 Hz max for voltage

#define ON 1
#define OFF 0

//the duration of each channel, pulse width
#define DELAYCHANNEL 400

vector<Channel> channels;

int channelNum = 4;


void setup(){
  Serial.begin(115200);
  //Serial.begin(9600);
  
  for(int i=0; i<channelNum; i++){
    channels.push_back(Channel(100, 100, 50, OFF, 1));
    pinMode(i+2,OUTPUT);
  }
  
  for(int j=4; j<8; j++){
    pinMode(j+2, OUTPUT);
  }
  
}


void loop(){

  //if serial recieved, read and write to i2c
    if(Serial.available() > 19){

    
    for(int i=0;i<channelNum;i++){
  
       channels[i].setPulse(Serial.read());
       channels[i].setFrequency(Serial.read());
       channels[i].setVoltage(Serial.read());
       channels[i].setState(Serial.read());
       channels[i].setDuration(Serial.read());

    }
    
    //discharge
    for(int i=0;i<channelNum;i++){ 
      if(channels[i].getState() == 1){    
        digitalWrite(i+2+channelNum,HIGH); 
        delayMicroseconds(100);
        digitalWrite(i+2+channelNum,LOW); 
      }
    }
    
     channelWrite();

    

  }  

}


//write for each channel
void channelWrite(){
  
  //duration*10 for debug
  //multiplied by 1000 for millis sec conversion
  //this must be updated for individual controls of durations
  //500 is for discharge
  for(int t=0; t< ( ((channels[0].getDuration()*1000)+500) / (1000/channels[0].getFrequency()) ) ;t++){

    
    for(int i=0;i<channelNum;i++){            
      channels[i].channelOut(i,0);
    }
    
  //one pulse relevant for the freq
  //microsec calculation
    delayMicroseconds( (1000000/channels[0].getFrequency()) - (DELAYCHANNEL*channelNum) );
  
  }
  
}
