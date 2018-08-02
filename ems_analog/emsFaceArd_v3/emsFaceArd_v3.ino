//2017/9/21
//multi-ch EMS
//Michi Kono, U-Tokyo

//2018/8/2
//modified


#include "Channel.h"
#include <StandardCplusplus.h>
#include <vector>
using namespace std;
  
  
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
  
  for(int j=channelNum; j<channelNum+4; j++){
    pinMode(j+2, OUTPUT);
  }
  
}


void loop(){

  
 //alwayas discharge
   for(int i=0;i<channelNum;i++){ 
       digitalWrite(i+2+channelNum,HIGH); 
       delayMicroseconds(200);
       digitalWrite(i+2+channelNum,LOW); 
   }
  
  //if serial recieved, read and write to i2c
    if(Serial.available() > 19){

    
    for(int i=0;i<channelNum;i++){
  
       channels[i].setPulse(Serial.read());
       channels[i].setFrequency(Serial.read());
       channels[i].setVoltage(Serial.read());
       channels[i].setState(Serial.read());
       channels[i].setDuration(Serial.read()*100); //converted for msec

    }
        
     channelWrite();

  }  

}


//write for each channel
void channelWrite(){
  
  //duration*10 for debug
  
  //this must be updated for individual controls of durations
  for(int t=0; t< ( ((channels[0].getDuration())) / (1000/channels[0].getFrequency()) ) ;t++){
    
    for(int i=0;i<channelNum;i++){            
      channels[i].channelOut(i,0);
    }
    
  //one pulse relevant for the freq
  //microsec calculation
    delayMicroseconds( (1000000/channels[0].getFrequency()) - (DELAYCHANNEL*channelNum) );
  
  }
  
}
