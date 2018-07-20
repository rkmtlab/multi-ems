//2017/9/21
//multi-ch EMS
//Michi Kono, U-Tokyo

#include <MultiEMS.h>
#include <StandardCplusplus.h>
#include <vector>
using namespace std;

std::vector<rkmtlab::MultiEMS::Channel *> channels;
int channelNum = 4;

void setup()
{
	Serial.begin(115200);

	for (int i = 0; i < channelNum; i++) {
		channels.push_back(new rkmtlab::MultiEMS::Channel(i, 100, 100, 50));
	}
}

void loop()
{
	//always discharge
	for (rkmtlab::MultiEMS::Channel *c : channels) {
		c->discharge();
	}

	if (Serial.available() > 19) {
		for (rkmtlab::MultiEMS::Channel *c : channels) {
			c->pulse = Serial.read();
			c->frequency = Serial.read();
			c->voltage = Serial.read();
			c->state = Serial.read();
			c->duration = Serial.read() * 100; // converted for msec
		}
		channelWrite();
	}
}

//write for each channel
void channelWrite()
{
	//duration * 10 for debug
	//this must be updated for individual controls of durations
	for (int i = 0; i < ((channels[0]->duration)) / (1000 / channels[0]->frequency); i++) {
		for (rkmtlab::MultiEMS::Channel *c : channels) {
			c->drive();
		}
		//one pulse relevant for the freq
		//microsec calculation
		delayMicroseconds((1000000 / channels[0]->frequency) - (rkmtlab::MultiEMS::Channel::Delay * channels.size()));
	}
}