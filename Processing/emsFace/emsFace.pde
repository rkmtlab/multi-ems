//multi EMS controller
//2017/7/8

import processing.serial.*;
import controlP5.*;


final boolean OFF = false;
final boolean ON = true;

//30 < pulsewidth < 240 us
final int MIN_PULSE = 30;
final int MAX_PULSE = 240;

//1 < f < 150 Hz
final int MIN_FREQ = 1;
final int MAX_FREQ = 150;

//0 < V < 50 ?
final int MIN_VOLT = 0;
final int MAX_VOLT = 50;

//1 < t < 10 sec
final int MIN_TIME = 1;
final int MAX_TIME = 10;

//init values
final int PULSE_INIT = 70;
final int FREQ_INIT = 100;
final int VOLT_INIT = 5;
final int TIME_INIT = 1;


/*
//the number of channels
 */
final int channelNum = 10;


PFont font_20;
ControlP5 cp5;
CheckBox checkbox[];
CheckBox checkBox;

Serial port;
Channel channels[];

int PulseWidth[];
int Frequency[];
int Voltage[];
int Time[];

ControlFont font;


void setup() {
  size(1500, 1000);
  frameRate(30);

  cp5 = new ControlP5(this);
  font_20 = loadFont("Helvetica-20.vlw");
  font = new ControlFont(font_20, 241);
  //font.setSize(20);
  printArray(Serial.list());
  String portName = Serial.list()[3];
  //port  =new Serial(this,"/dev/tty.usbmodem121",115200);  
  //port  =new Serial(this,"/dev/tty.usbmodem122",9600);  
  //port.bufferUntil('\n');
  port = new Serial(this, portName, 115200);

  channels = new Channel[channelNum];
  checkbox = new CheckBox[channelNum];
  PulseWidth = new int[channelNum];
  Frequency = new int[channelNum];
  Voltage = new int[channelNum];
  Time = new int[channelNum];

  for (int i=0; i<channelNum; i++) {
    channels[i] = new Channel(100, 100, 50, OFF, 1);
    PulseWidth[i] = 100;
    Frequency[i] = 100;
    Voltage[i] = 50;
    Time[i] = 200;
  }

  controls();
}



void draw() {
  background(0);

  /*
 the frequency cannot be adjusted individually
   */
  for (int i=1; i<channelNum; i++) {
    cp5.getController("Frequency["+i+"]").setValue(cp5.getController("Frequency[0]").getValue());
  } 


  boxApply();

  updateChannels();

  /*
  send to arduino after a 1 sec delay
   */
  if (checkBox.getState(5)==true) {
    delay(500);

    serialPrinting();
    delay(1000);
    checkBox.deactivate(5);
  }
}

/*
update the channels from the sliders
 */
void updateChannels() {

  for (int i=0; i<channelNum; i++) {
    channels[i].setPulse((int)cp5.getController("PulseWidth["+i+"]").getValue());
    channels[i].setFrequency((int)cp5.getController("Frequency["+i+"]").getValue());
    channels[i].setVoltage((int)cp5.getController("Voltage["+i+"]").getValue());
    channels[i].setState(checkbox[i].getState(0));
    channels[i].setTime((int)cp5.getController("Time["+i+"]").getValue());
  }
}

/*
serial write to arduino
 */
void serialPrinting() {

  for (int i=0; i<channelNum; i++) {
    port.write(channels[i].getPulse());
    port.write(channels[i].getFrequency());
    port.write(channels[i].getVoltage());
    port.write(channels[i].getState());
    port.write(channels[i].getTime());
  }
}


/*
apply the checkboxs to the sliders
 */
void boxApply() {

  if (checkBox.getState(0)==true) {
    for (int i=1; i<channelNum; i++) {
      cp5.getController("PulseWidth["+i+"]").setValue(cp5.getController("PulseWidth[0]").getValue());
    }
  }

  if (checkBox.getState(1)==true) {
    for (int i=1; i<channelNum; i++) {
      cp5.getController("Voltage["+i+"]").setValue(cp5.getController("Voltage[0]").getValue());
    }
  }

  if (checkBox.getState(2)==true) {
    for (int i=1; i<channelNum; i++) {
      cp5.getController("Time["+i+"]").setValue(cp5.getController("Time[0]").getValue());
    }
  }

  if (checkBox.getState(3)==true) {
    checkBox.deactivate(3);
    for (int i=0; i<channelNum; i++) {
      checkbox[i].deactivateAll();
    }
  }

  if (checkBox.getState(4)==true) {
    checkBox.deactivate(4);
    for (int i=0; i<channelNum; i++) {
      checkbox[i].activateAll();
    }
  }
}


/*
the guis
 */
void controls() {

  //the checkboxes on the right
  checkBox = cp5.addCheckBox("checkBoxes")
    .setPosition(1240, 40)
      .setColorForeground(color(120))
        .setColorActive(color(255))
          .setColorLabel(color(255))
            .setSize(40, 40)
              //.setItemsPerRow(3)
              //.setSpacingColumn(30)
              .setSpacingRow(20)
                .addItem("UnifyPulseWidth", 0)
                  .addItem("UnifyVoltage", 0)
                    .addItem("UnifyTime", 0)
                      .addItem("AllOff", 0)
                        .addItem("AllOn", 0)
                          .addItem("Confirm", 0)
                            .setFont(font)
                              ;


  //the checkboxes for the channel ON/OFF
  for (int i=0; i<10; i++) {

    checkbox[i] = cp5.addCheckBox("checkbox[" + i +"]")
      .setPosition(50, 40+i*90)
        .setColorForeground(color(120))
          .setColorActive(color(255))
            .setColorLabel(color(255))
              .setSize(20, 20)
                //.setItemsPerRow(3)
                //.setSpacingColumn(30)
                //.setSpacingRow(20)
                .addItem("Ch" + i, 0)
                  ;

    cp5.addSlider("PulseWidth["+i+"]")
      .setPosition(140, 20+i*90)
        .setWidth(200)
          .setRange(MIN_PULSE, MAX_PULSE)
            .setValue(PULSE_INIT)
              .setHeight(20);
    ; 

    cp5.addSlider("Frequency["+i+"]")
      .setPosition(140, 40+i*90)
        .setWidth(200)
          .setRange(MIN_FREQ, MAX_FREQ)
            .setValue(FREQ_INIT)
              .setHeight(20);
    ; 

    cp5.addSlider("Voltage["+i+"]")
      .setPosition(140, 60+i*90)
        .setWidth(200)
          .setRange(MIN_VOLT, MAX_VOLT)
            .setValue(VOLT_INIT)
              .setHeight(20);
    ; 

    cp5.addSlider("Time["+i+"]")
      .setPosition(140, 80+i*90)
        .setWidth(200)
          .setRange(MIN_TIME, MAX_TIME)
            .setValue(TIME_INIT)
              .setHeight(20);
    ;
  }

  if (channelNum>10) {

    for (int i=10; i<20; i++) {

      checkbox[i] = cp5.addCheckBox("checkbox[" + i +"]")
        .setPosition(450, 40+(i-10)*90)
          .setColorForeground(color(120))
            .setColorActive(color(255))
              .setColorLabel(color(255))
                .setSize(20, 20)
                  //.setItemsPerRow(3)
                  //.setSpacingColumn(30)
                  //.setSpacingRow(20)
                  .addItem("Ch" + i, 0)
                    ;
      cp5.addSlider("PulseWidth["+i+"]")
        .setPosition(540, 20+(i-10)*90)
          .setWidth(200)
            .setRange(MIN_PULSE, MAX_PULSE)
              .setValue(PULSE_INIT)
                .setHeight(20);
      ; 

      cp5.addSlider("Frequency["+i+"]")
        .setPosition(540, 40+(i-10)*90)
          .setWidth(200)
            .setRange(MIN_FREQ, MAX_FREQ)
              .setValue(FREQ_INIT)
                .setHeight(20);
      ; 

      cp5.addSlider("Voltage["+i+"]")
        .setPosition(540, 60+(i-10)*90)
          .setWidth(200)
            .setRange(MIN_VOLT, MAX_VOLT)
              .setValue(VOLT_INIT)
                .setHeight(20);
      ; 

      cp5.addSlider("Time["+i+"]")
        .setPosition(540, 80+(i-10)*90)
          .setWidth(200)
            .setRange(MIN_TIME, MAX_TIME)
              .setValue(TIME_INIT)
                .setHeight(20);
      ;
    }

    if (channelNum>20) {

      for (int i=20; i<channelNum; i++) {

        checkbox[i] = cp5.addCheckBox("checkbox[" + i +"]")
          .setPosition(850, 40+(i-20)*90)
            .setColorForeground(color(120))
              .setColorActive(color(255))
                .setColorLabel(color(255))
                  .setSize(20, 20)
                    //.setItemsPerRow(3)
                    //.setSpacingColumn(30)
                    //.setSpacingRow(20)
                    .addItem("Ch" + i, 0)
                      ;

        cp5.addSlider("PulseWidth["+i+"]")
          .setPosition(940, 20+(i-20)*90)
            .setWidth(200)
              .setRange(MIN_PULSE, MAX_PULSE)
                .setValue(PULSE_INIT)
                  .setHeight(20);
        ; 

        cp5.addSlider("Frequency["+i+"]")
          .setPosition(940, 40+(i-20)*90)
            .setWidth(200)
              .setRange(MIN_FREQ, MAX_FREQ)
                .setValue(FREQ_INIT)
                  .setHeight(20);
        ; 

        cp5.addSlider("Voltage["+i+"]")
          .setPosition(940, 60+(i-20)*90)
            .setWidth(200)
              .setRange(MIN_VOLT, MAX_VOLT)
                .setValue(VOLT_INIT)
                  .setHeight(20);
        ; 

        cp5.addSlider("Time["+i+"]")
          .setPosition(940, 80+(i-20)*90)
            .setWidth(200)
              .setRange(MIN_TIME, MAX_TIME)
                .setValue(TIME_INIT)
                  .setHeight(20);
        ;
      }
    }
  }
  
}

