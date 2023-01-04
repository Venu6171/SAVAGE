#pragma once

#include "SteeringBehavior.h"

namespace SAVAGE::AI
{
	class SeekBehavior : public SteeringBehavior
	{
	public:
		SAVAGE::Math::Vector2 Calculate(Agent& agent);

		void EnableDebug(bool debug) { mShowSeekLines = debug; }
		bool ShowDebug() const { return mShowSeekLines; }

	private:
		bool mShowSeekLines = false;;
	};
}