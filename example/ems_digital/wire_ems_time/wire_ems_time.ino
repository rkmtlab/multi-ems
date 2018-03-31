//2017/9/7
//Michi Kono, U-Tokyo

#include <Metro.h>
#include <Wire.h>

//500 Hz max for voltage, due to metro.h res

int channelNum = 10;
Metro metroVolt[10]; //count time for pulse
Metro metroTime[10]; //count time for duration
int metroFlag[10]; //counting completed
int channelState[10]; //the state of the channel
int pinState[10]; //the pin state for pulse output
int metVoltHolder[10]; //store the voltage
int voltage[10];
Metro metroInit[10]; //considering the delay
int initFlag[10];

void setup()
{
	Wire.begin(1); // join i2c bus with address i
	Wire.onReceive(receiveEvent); // register event

	for (int i = 2; i < 14; i++) {
		pinMode(i, OUTPUT);
	}

	for (int i = 0; i < channelNum; i++) {
		metroVolt[i].interval(1);
		metroTime[i].interval(1);
		metroFlag[i] = 0;
		channelState[i] = 0;
		pinState[i] = 0;
		metVoltHolder[i] = 0;
		voltage[i] = 0;
		metroInit[i].interval(1000);
		initFlag[i] = 0;
	}
}

void loop()
{
	voltOut();
}

void receiveEvent(int howMany)
{
	int duration[channelNum];

	if (Wire.available() > 29) {
		for (int i = 0; i < channelNum; i++) {
			voltage[i] = Wire.read() * 10;
			if (voltage[i] > 500) {
				voltage[i] = 500;
			}
			channelState[i] = Wire.read();
			duration[i] = Wire.read() * 1000 + 1000; //1000 is for stablization
			metroVolt[i].interval(1000 / (voltage[i] * 2));
			metroTime[i].interval(duration[i]);
			metroVolt[i].reset();
			metroTime[i].reset();
			metroFlag[i] = 0;
			pinState[i] = 0;
			if (voltage[i] > 60) {
				metVoltHolder[i] = voltage[i] / 4;
				metroVolt[i].interval(1000 / (metVoltHolder[i] * 2));
			}
			else {
				metVoltHolder[i] = voltage[i];
			}
			metroInit[i].reset();
			initFlag[i] = 0;
		}
	}
}

void voltOut()
{
	for (int i = 0; i < channelNum; i++) {
		if (metroInit[i].check() == 1) {
			initFlag[i] = 1;
		}

		//if the duration has ended, stop
		if (metroTime[i].check() == 1) {
			metroFlag[i] = 1;
			channelState[i] = 0;
			digitalWrite(i + 2, LOW);
		}

		else if (channelState[i] == 0) {
			digitalWrite(i + 2, LOW);
		}

		//write ON and OFF
		else if (metroTime[i].check() == 0 && metroFlag[i] == 0 && channelState[i] == 1) {
			if (metroVolt[i].check() == 1) {
				if (pinState[i] == HIGH) {
					pinState[i] = LOW;
					if (metVoltHolder[i] < voltage[i] && initFlag[i] == 1) {
						metroVolt[i].interval(1000 / (metVoltHolder[i] * 2));
						metVoltHolder[i] = metVoltHolder[i] + 1;
					}
				}
				else {
					pinState[i] = HIGH;
				}
				digitalWrite(i + 2, pinState[i]);
			}
		}
	}
}
