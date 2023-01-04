#pragma once

#include "Common.h"

namespace SAVAGE::AI
{
	template <class AgentType>
	class State
	{
	public:
		virtual ~State() = default;

		virtual void Enter(AgentType& agent) = 0;
		virtual void Update(AgentType& agent, float deltaTime) = 0;
		virtual void SAVAGE::it(AgentType& agent) = 0;
	};
}
