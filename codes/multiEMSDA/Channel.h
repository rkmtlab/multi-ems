


class Channel{
  public:
  
  int pulse;
  int frequency;
  int voltage;
  int state;
  int duration;
  
  Channel();
  Channel(int pulseTemp, int frequencyTemp, int voltageTemp, int stateTemp, int durationTemp);
 
  void initialize();

  void setPulse(int pulseTemp);
  void setFrequency(int frequencyTemp);
  void setVoltage(int voltageTemp);
  void setState(int stateTemp);
  void setDuration(int durationTemp);
  
  int getPulse();
  int getFrequency();
  int getVoltage();
  int getState();
  long getDuration();
  
  int channelOut(int channelNumber);  
  
};
