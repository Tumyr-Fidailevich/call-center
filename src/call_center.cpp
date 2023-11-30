#include "call_center.h"
#include "logging_macros.h"

std::string CONFIG_PATH = "../output/config.json";


CallCenter::CallCenter() : config(std::make_unique<Config>(CONFIG_PATH))
{
	for (std::size_t i = 1; i <= config->numOfOperators; i++)
	{
		auto newOperator = std::make_shared<Operator>(i);
		LOG_TO_FILE(google::GLOG_INFO, LOG_FILE) << "Operator with id " << newOperator->getId() << " initialized";
		freeOperators.emplace_back(newOperator);
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
