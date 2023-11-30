#ifndef CALL_CENTER_CALL_CENTER_H
#define CALL_CENTER_CALL_CENTER_H

#include "pch.h"
#include "operator.h"
#include "config.h"
#include "call.h"

class Operator;

class CallCenter
{
public:
	CallCenter();

	void processCall(std::shared_ptr<Call> &call);

	static std::string getPhoneNumberFromRequest(const std::string &request);

	static std::string getMessageForUser(std::shared_ptr<Call> &call);

	static bool isNumberValid(const std::string &phoneNumber);

private:

	bool isQueueOverload();

	bool isQueueEmpty();

	void addCallToQueue(std::shared_ptr<Call> &call);

	void removeCall(std::shared_ptr<Call> &call);

	bool hasAvailableOperator();

	bool isDuplicated(std::shared_ptr<Call> &call);

	[[noreturn]] void processCallQueue();

	std::deque<std::shared_ptr<Call>> callQueue;
	std::deque<std::shared_ptr<Operator>> freeOperators;
	std::unique_ptr<Config> config;
	std::mutex callQueueMutex;
	std::mutex operatorsMutex;
	std::thread callQueueThread;
};

#endif
