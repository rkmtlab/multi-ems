class Channel{
  
  final int OFF = 0;
  final int ON = 1;
  
  int pulseWidth;
  int frequency;
  int voltage;
  boolean state;
  int time;
  
  Channel(int pulseWidthTemp, int frequencyTemp, int voltageTemp, boolean stateTemp, int timeTemp){
    pulseWidth = pulseWidthTemp;
    frequency = frequencyTemp;
    voltage = voltageTemp;
    state = stateTemp;
    time = timeTemp;
  }
  
  void setPulse(int pulseWidthTemp){
    pulseWidth = pulseWidthTemp;
  }
  
  void setFrequency(int frequencyTemp){
    frequency = frequencyTemp;
  }
  
  void setVoltage(int voltageTemp){
    voltage = voltageTemp; 
  }
  
  void setState(boolean stateTemp){
    state = stateTemp; 
  }
  
  void setTime(int timeTemp){
    time = timeTemp;
  }
  
  int getPulse(){
    return pulseWidth;
  }
  
  int getFrequency(){
    return frequency;
  }
  
  int getVoltage(){
    return voltage; 
  }
  
  int getState(){
    int stateInt = 0;
    if(state==true)stateInt = 1;
    return stateInt;
  }
  
  int getTime(){
    return time;
  }
  
}
