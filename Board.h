#pragma once

#include "./Channel.h"
#include <StandardCplusplus.h>
#include <vector>

namespace rkmtlab
{
namespace MultiEMS
{
	class Board
	{
	protected:
		int _numberOfChannels;
		std::vector<MultiEMS::Channel *> _channels;

	public:
		int numberOfChannels()
		{
			return this->_numberOfChannels;
		}

		Board(int numberOfChannels)
		{
			this->_numberOfChannels = numberOfChannels;
			for (int i = 0; i < this->_numberOfChannels; i++) {
				_channels.push_back(new MultiEMS::Channel(i, 100, 100, 50));
			}
		}

		~Board()
		{
			for (int i = 0; i < this->_numberOfChannels; i++) {
				delete _channels[i];
			}
		}

		std::vector<MultiEMS::Channel *> channels()
		{
			return this->_channels;
		}

		MultiEMS::Channel *channelForIndex(int index)
		{
			return _channels[index];
		}

		void discharge()
		{
			for (MultiEMS::Channel *c : _channels) {
				c->discharge();
			}
		}

		void driveChannel(int index)
		{
			auto c = this->channelForIndex(index);
			for (int i = 0; i < ((c->duration)) / (1000 / c->frequency); i++) {
				c->drive();
				//one pulse relevant for the freq
				//microsec calculation
				delayMicroseconds((1000000 / c->frequency) - (MultiEMS::Channel::Delay * _channels.size()));
			}
		}

		void driveAll()
		{
			auto channel = _channels[0];
			for (int i = 0; i < ((channel->duration)) / (1000 / channel->frequency); i++) {
				for (MultiEMS::Channel *c : _channels) {
					c->drive();
				}
				//one pulse relevant for the freq
				//microsec calculation
				delayMicroseconds((1000000 / channel->frequency) - (MultiEMS::Channel::Delay * _channels.size()));
			}
		}
	};
}
}