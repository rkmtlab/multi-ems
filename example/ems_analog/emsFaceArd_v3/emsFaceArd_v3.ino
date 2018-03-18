//2017/9/21
//multi-ch EMS
//Michi Kono, U-Tokyo

#include <MultiEMS.h>
#include <StandardCplusplus.h>
#include <vector>
using namespace std;

//500 Hz max for voltage

std::vector<rkmtlab::MultiEMS::Channel> channels;
int channelNum = 4;

void setup()
{
	Serial.begin(115200);

	for (int i = 0; i < channelNum; i++) {
		channels.push_back(rkmtlab::MultiEMS::Channel(i, 100, 100, 50, rkmtlab::MultiEMS::Channel::State::Disable, 1));
		pinMode(i + 2, OUTPUT);
	}

	for (int j = 4; j < 8; j++) {
		pinMode(j + 2, OUTPUT);
	}
}

void loop()
{
	//alwayas discharge
	for (int i = 0; i < channelNum; i++) {
		channels[i].discharge();
	}

	if (Serial.available() > 19) {
		for (int i = 0; i < channels.size(); i++) {
			channels[i].pulse = Serial.read();
			channels[i].frequency = Serial.read();
			channels[i].voltage = Serial.read();
			channels[i].state = Serial.read();
			channels[i].duration = Serial.read() * 100; // converted for msec
		}
		channelWrite();
	}
}

//write for each channel
void channelWrite()
{
	//duration * 10 for debug
	//this must be updated for individual controls of durations
	for (int t = 0; t < ((channels[0].duration)) / (1000 / channels[0].frequency); t++) {
		for (int i = 0; i < channels.size(); i++) {
			channels[i].drive();
		}
		//one pulse relevant for the freq
		//microsec calculation
		delayMicroseconds((1000000 / channels[0].frequency) - (rkmtlab::MultiEMS::Channel::Delay * channelNum));
	}
}
