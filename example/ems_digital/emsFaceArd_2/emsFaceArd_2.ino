//2017/9/7
//multi-ch EMS
//Michi Kono, U-Tokyo

#include <MultiEMS.h>
#include <StandardCplusplus.h>
#include <vector>
using namespace std;

//500 Hz max for voltage

#include <Wire.h>

#define ON 1
#define OFF 0

std::vector<rkmtlab::MultiEMS::Channel *> channels;

int channelNum = 10;

void setup()
{
	Serial.begin(115200);
	Wire.begin();
	//Serial.begin(9600);

	for (int i = 0; i < channelNum; i++) {
		channels.push_back(new rkmtlab::MultiEMS::Channel(i, 100, 100, 50));
		pinMode(i + 2, OUTPUT);
	}
}

void loop()
{
	//if serial recieved, read and write to i2c
	if (Serial.available() > 49) {
		for (int i = 0; i < channelNum; i++) {
			channels[i]->pulse = Serial.read();
			channels[i]->frequency = Serial.read();
			channels[i]->voltage = Serial.read();
			channels[i]->state = Serial.read();
			channels[i]->duration = Serial.read() * 100; // converted for msec
		}

		Wire.beginTransmission(1);
		for (int i = 0; i < channelNum; i++) {
			Wire.write(channels[i]->voltage);
			Wire.write(channels[i]->state);
			Wire.write((int)channels[i]->duration);
		}

		Wire.endTransmission();

		channelWrite();
	}
}

//write for each channel
void channelWrite()
{
	//duration*10 for debug
	//this must be updated for individual controls of durations
	//2500 is for discharge
	for (int t = 0; t < (((channels[0]->duration * 1000) + 2500) / (1000 / channels[0]->frequency)); t++) {
		for (int i = 0; i < channelNum; i++) {
			channels[i]->drive();
		}

		//one pulse relevant for the freq
		delayMicroseconds((1000000 / channels[0]->frequency) - (rkmtlab::MultiEMS::Channel::Delay * channelNum));
	}
}
