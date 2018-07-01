//multi EMS controller
//2017/9/21
//Michi Kono, U-Tokyo

//use serialPrinting() to output signals. do not use continuously.

import processing.serial.*;
import controlP5.*;


final boolean OFF = false;
final boolean ON = true;

//30 < pulsewidth < 240 us
final int MIN_PULSE = 40;
final int MAX_PULSE = 240;

//50 < f < 150 Hz
final int MIN_FREQ = 50; //f > 50
final int MAX_FREQ = 150;

//100 ms < t < 2000 ms
final int MIN_TIME = 500;
final int MAX_TIME = 2000;

//init values
final int PULSE_INIT = 70;
final int FREQ_INIT = 100;
final int VOLT_INIT = 5;
final int TIME_INIT = 1;


/*
//the number of channels
 */
final int channelNum = 4;  //n >= 10


PFont font_20;
ControlP5 cp5;
CheckBox checkbox[];
CheckBox checkBox;

Serial port;
Channel channels[];

int PulseWidth[];
int Frequency[];
int Time[];

ControlFont font;


void setup() {
  size(800, 400);
  frameRate(30);

  cp5 = new ControlP5(this);
  font_20 = loadFont("Helvetica-20.vlw");
  font = new ControlFont(font_20, 241);
  //font.setSize(20);
  
  //Serial settings
  printArray(Serial.list());
  String portName = Serial.list()[5];
  port = new Serial(this, portName, 115200);

  channels = new Channel[channelNum];
  checkbox = new CheckBox[channelNum];
  PulseWidth = new int[channelNum];
  Frequency = new int[channelNum];
  Time = new int[channelNum];

  for (int i=0; i<channelNum; i++) {
    channels[i] = new Channel(100, 100, 50, OFF, 1);
    PulseWidth[i] = 100;
    Frequency[i] = 100;
    Time[i] = 1;
  }

  controls();
}



void draw() {
  background(0);

  /*
 the frequency and duration cannot be adjusted individually
   */
  for (int i=1; i<channelNum; i++) {
    cp5.getController("Frequency["+i+"]").setValue(cp5.getController("Frequency[0]").getValue());
    cp5.getController("Time["+i+"]").setValue(cp5.getController("Time[0]").getValue());
  } 


  boxApply();

  updateChannels();

  /*
  send to arduino after a 500 msec delay
   */
  if (checkBox.getState(3)==true) {
    delay(500);

    serialPrinting();
    
    delay(1000);
    
    checkBox.deactivate(3);
  }
  
  if (port.available() > 0) {
    String str = port.readString();
    if (str != null) {
      println(str);
    }
  }
  
}

/*
update the channels from the sliders
 */
void updateChannels() {

  for (int i=0; i<channelNum; i++) {
    channels[i].setPulse((int)cp5.getController("PulseWidth["+i+"]").getValue());
    channels[i].setFrequency((int)cp5.getController("Frequency["+i+"]").getValue());
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
    port.write(0);  //dummy for voltage
    port.write(channels[i].getState());
    port.write(channels[i].getTime()/100); //msec converted for serial
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

//  if (checkBox.getState(1)==true) {
//    for (int i=1; i<channelNum; i++) {
//      cp5.getController("Time["+i+"]").setValue(cp5.getController("Time[0]").getValue());
//    }
//  }

  if (checkBox.getState(1)==true) {
    checkBox.deactivate(1);
    for (int i=0; i<channelNum; i++) {
      checkbox[i].deactivateAll();
    }
  }

  if (checkBox.getState(2)==true) {
    checkBox.deactivate(2);
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
    .setPosition(600, 40)
      .setColorForeground(color(120))
        .setColorActive(color(255))
          .setColorLabel(color(255))
            .setSize(40, 40)
              //.setItemsPerRow(3)
              //.setSpacingColumn(30)
              .setSpacingRow(20)
                .addItem("UnifyPulseWidth", 0)
                    //.addItem("UnifyTime", 0)
                      .addItem("AllOff", 0)
                        .addItem("AllOn", 0)
                          .addItem("Confirm", 0)
                            .setFont(font)
                              ;

  //the checkboxes for the channel ON/OFF
  for (int i=0; i<channelNum; i++) {

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
              .setHeight(20)
                .setNumberOfTickMarks(21);
    ; 

    cp5.addSlider("Frequency["+i+"]")
      .setPosition(140, 40+i*90)
        .setWidth(200)
          .setRange(MIN_FREQ, MAX_FREQ)
            .setValue(FREQ_INIT)
              .setHeight(20)
                .setNumberOfTickMarks(11);
    ; 

    cp5.addSlider("Time["+i+"]")
      .setPosition(140, 60+i*90)
        .setWidth(200)
          .setRange(MIN_TIME, MAX_TIME)
            .setValue(TIME_INIT)
              .setHeight(20)
                .setNumberOfTickMarks(16);
    ;
  }

}
