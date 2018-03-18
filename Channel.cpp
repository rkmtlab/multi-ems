#include "Channel.h"

rkmtlab::MultiEMS::Channel::Channel(int channelIdentifier, int pulse, int frequency, int voltage, Channel::State state, int duration)
{
	this->_channelIdentifier = channelIdentifier;
	this->pulse = pulse;
	this->frequency = frequency;
	this->voltage = voltage;
	this->state = state;
	this->duration = duration;
}

int rkmtlab::MultiEMS::Channel::channelIdentifier()
{
	return this->_channelIdentifier;
}

int rkmtlab::MultiEMS::Channel::drive()
{
	if (this->state == Channel::State::Enable) {
		digitalWrite(this->channelIdentifier() + 2, HIGH);
		delayMicroseconds(pulse);
		digitalWrite(this->channelIdentifier() + 2, LOW);
		delayMicroseconds(Channel::Delay - pulse);
	}
	else if (this->state == Channel::State::Enable) {
		digitalWrite(this->channelIdentifier() + 2, LOW);
		delayMicroseconds(Channel::Delay);
	}
}

void rkmtlab::MultiEMS::Channel::discharge()
{
	digitalWrite(this->channelIdentifier() + 2, HIGH);
	delayMicroseconds(100);
	digitalWrite(this->channelIdentifier() + 2, LOW);
}
