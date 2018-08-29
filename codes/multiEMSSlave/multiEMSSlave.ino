//2018/8/3
//multi-ch EMS
//Michi Kono, U-Tokyo


#include <Wire.h>
#include <Metro.h>

//500 Hz max for voltage, due to metro.h res

int channelNum = 4;
Metro metroVolt[4];  //count time for pulse 
Metro metroTime[4];  //count time for duration
int metroFlag[4];  //counting completed
int channelState[4]; //the state of the channel
int pinState[4]; //the pin state for pulse output


void setup()
{
  Wire.begin(1);                // join i2c bus with address i
  Wire.onReceive(receiveEvent); // register event
  
  for(int i=2;i<14;i++){
    pinMode(i,OUTPUT);
  }
  
  for(int i=0;i<channelNum;i++){
    metroVolt[i].interval(1);
    metroTime[i].interval(1);
    metroFlag[i] = 0;
    channelState[i] = 0;
    pinState[i] = 0;
  }
  
}

void loop(){

  voltOut();
  
}

void receiveEvent(int howMany)
{
  int voltage[channelNum];
  int duration[channelNum];
  
  if(Wire.available()> ( (3*channelNum -1) )){
  
    for(int i=0;i<channelNum;i++){
      voltage[i] = Wire.read()*10;
      if(voltage[i]>500)voltage[i] = 500;
      channelState[i] = Wire.read();
      duration[i] =  Wire.read()*100 + 2000; //500 is for stablization
      metroVolt[i].interval(1000 / (voltage[i]*2));
      metroTime[i].interval(duration[i]);
      metroVolt[i].reset();
      metroTime[i].reset();
      metroFlag[i] = 0;
      pinState[i] = 0;
    }

  }

}


void voltOut(){
  
  for(int i=0;i<channelNum;i++){
    
    //if the duration has ended, stop
    if(metroTime[i].check()==1){
      metroFlag[i] = 1;
      channelState[i] = 0;
      digitalWrite(i+2,LOW);      
    }
    
    else if(channelState[i] == 0){
      digitalWrite(i+2,LOW);
    }
    
    //write ON and OFF 
    else if(metroTime[i].check()==0 && metroFlag[i] == 0 && channelState[i] == 1){
      if(metroVolt[i].check()==1){
        if (pinState[i]==HIGH)  { 
          pinState[i]=LOW;
        } else {
          pinState[i]=HIGH;
        }
        digitalWrite(i+2,pinState[i]);
      
      }
    }
    
  }
  
  
}




