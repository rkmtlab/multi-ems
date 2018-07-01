#include "Channel.h"

rkmtlab::MultiEMS::Channel::Channel(int channelIdentifier, int pulse, int frequency, int voltage)
{
	this->_channelIdentifier = channelIdentifier;
	this->pulse = pulse;
	this->frequency = frequency;
	this->voltage = voltage;
	this->state = Enable;
	this->duration = 0;
	pinMode(this->_channelIdentifier + 2, OUTPUT);
	pinMode(this->_channelIdentifier + 2 + 4, OUTPUT); //2 is to avoid using RX/TX pins, 4 is the number of channels
}

int rkmtlab::MultiEMS::Channel::channelIdentifier()
{
	return this->_channelIdentifier;
}

int rkmtlab::MultiEMS::Channel::drive()
{
	if (this->state == Channel::State::Enable) {
		digitalWrite(this->channelIdentifier() + 2, HIGH);
		delayMicroseconds(this->pulse);
		digitalWrite(this->channelIdentifier() + 2, LOW);
		delayMicroseconds(Channel::Delay - this->pulse);
	}
	else if (this->state == Channel::State::Disable) {
		digitalWrite(this->channelIdentifier() + 2, LOW);
		delayMicroseconds(Channel::Delay);
	}
}

void rkmtlab::MultiEMS::Channel::discharge()
{
	//4 is the number of channels
	digitalWrite(this->channelIdentifier() + 2 + 4, HIGH);
	delayMicroseconds(100);
	digitalWrite(this->channelIdentifier() + 2 + 4, LOW);
}
