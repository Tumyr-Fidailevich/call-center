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

private:

	std::deque<std::shared_ptr<Call>> callQueue;
	std::deque<Operator*> freeOperators;
	std::unique_ptr<Config> config = std::make_unique<Config>("../output/config.json");
	std::mutex mutex;
};

#endif
