#pragma once
#include "Agent.h"
class Prey: public Agent
{
	
	Action act(unsigned long long perception_hash) override;
};
