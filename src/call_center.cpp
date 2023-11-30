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
	LOG_TO_FILE(google::GLOG_INFO, LOG_FILE) << "Call center processing call with number "
											 << call->getCDR().phoneNumber;
	if (isQueueOverload())
	{
		call->getCDR().status = CDR::Status::Overload;
		call->end();
	} else if (isDuplicated(call))
	{
		call->getCDR().status = CDR::Status::Duplicate;
		call->end();
	} else
	{
		addCallToQueue(call);
	}
}

bool CallCenter::isQueueOverload()
{
	std::lock_guard lock(callQueueMutex);
	if (callQueue.size() >= config->maxQueueSize)
	{
		LOG_TO_FILE(google::GLOG_WARNING, LOG_FILE) << "Queue is overloaded";
		return true;
	}
	return false;
}

bool CallCenter::isDuplicated(std::shared_ptr<Call> &targetCall)
{
	std::lock_guard lock(callQueueMutex);
	auto matchIt = std::find_if(callQueue.begin(), callQueue.end(),
								[&targetCall](auto const call) {
									return call == targetCall;
								});
	if (matchIt == callQueue.end())
	{
		LOG_TO_FILE(google::GLOG_WARNING,) << "Phone number " << targetCall->getCDR().phoneNumber
										   << " is duplicated in the queue";
		return true;
	}
	return false;
}

bool CallCenter::isQueueEmpty()
{
	return callQueue.empty();
}

std::string CallCenter::getPhoneNumberFromRequest(const std::string &request)
{
	std::string phoneNumber;

	size_t phoneStartPos = request.find("phone=");
	if (phoneStartPos != std::string::npos)
	{
		size_t phoneEndPos = request.find('&', phoneStartPos);
		if (phoneEndPos == std::string::npos)
			phoneEndPos = request.find(" HTTP", phoneStartPos);

		if (phoneEndPos != std::string::npos)
		{
			phoneStartPos += 6;
			phoneNumber = request.substr(phoneStartPos, phoneEndPos - phoneStartPos);
		}
	}

	return phoneNumber;
}