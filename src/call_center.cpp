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

	callQueueThread = std::thread(&CallCenter::processCallQueue, this);
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

void CallCenter::addCallToQueue(std::shared_ptr<Call> &call)
{
	{
		std::lock_guard lock(callQueueMutex);
		callQueue.push_back(call);
		LOG_TO_FILE(google::GLOG_INFO, LOG_FILE) << "Call with phone number " << call->getCDR().phoneNumber
												 << " added to queue";
	}
	call->startTimer(config->maxQueueWaitingTime, [this](std::shared_ptr<Call> call) {
		this->callTimeoutHandler(call);
	});
}

void CallCenter::removeCall(std::shared_ptr<Call> &call)
{
	std::lock_guard lock(callQueueMutex);
	auto it = std::find(callQueue.begin(), callQueue.end(), call);
	if (it != callQueue.end())
	{
		callQueue.erase(it);
		LOG_TO_FILE(google::GLOG_INFO, LOG_FILE) << "Call with phone number " << call->getCDR().phoneNumber
												 << " removed from the queue";
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

bool CallCenter::hasAvailableOperator()
{
	return !freeOperators.empty();
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

[[noreturn]] void CallCenter::processCallQueue()
{
	while (true)
	{
		if (hasAvailableOperator() && !isQueueEmpty())
		{
			std::lock_guard lock(callQueueMutex);
			auto call = callQueue.front();
			callQueue.pop_front();
			call->stopTimer();
			auto curOperator = freeOperators.front();
			freeOperators.pop_front();
			call->setOperator(curOperator);
			call->getCDR().responseTime = boost::posix_time::second_clock::local_time();
			call->getCDR().callDuration = getRandomCallDuration(config->minCallDuration, config->maxCallDuration);
			call->startTimer(
					call->getCDR().callDuration,
					[this](std::shared_ptr<Call> call) {
						this->callReleaseHandler(call);
					});
		}
	}
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

bool CallCenter::isNumberValid(const std::string &phoneNumber)
{
	boost::regex phoneRegex("\\+\\d{11}");

	return boost::regex_match(phoneNumber, phoneRegex);
}

std::string CallCenter::getMessageForUser(std::shared_ptr<Call> &call)
{
	std::string status{};
	switch (call->getCDR().status)
	{
		case CDR::Status::OK:
		{
			status = "OK";
			break;
		}
		case CDR::Status::Duplicate:
		{
			status = "Duplicate";
			break;
		}
		case CDR::Status::Overload:
		{
			status = "Overload";
			break;
		}
		case CDR::Status::Timeout:
		{
			status = "Timeout";
			break;
		}
		case CDR::Status::Interrupted:
		{
			status = "Interrupted";
			break;
		}
	}
	return std::string{status + " " + call->getCDR().callId};
}