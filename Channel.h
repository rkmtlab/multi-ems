#pragma once

#include <Arduino.h>

namespace rkmtlab
{
namespace MultiEMS
{
	class Channel
	{
	private:
		int _channelIdentifier;

	public:
		static const int Delay = 400;
		enum State
		{
			Disable = 0,
			Enable,
		};

		int pulse;
		int frequency;
		int voltage;
		int state;
		int duration;

		Channel(int channelIdentifier, int pulse, int frequency, int voltage);
		int channelIdentifier();
		int drive();
		void discharge();
	};
}
}
