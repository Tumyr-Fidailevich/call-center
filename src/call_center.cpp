#include "call_center.h"

CallCenter::CallCenter()
{
	for (std::size_t i = 1; i <= config->numOfOperators; i++)
	{
		freeOperators.emplace_back(i, this);
	}
}

void CallCenter::processCall(std::shared_ptr<Call> &call)
{
	if (isQueueOverload())
	{
		call->getCDR().status = "Overload";
	} else if (isDuplicated(call))
	{
		call->getCDR().status = "Duplicate";
	} else
	{
		callQueue.push_back(call);
	}
}
