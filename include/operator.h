#ifndef CALL_CENTER_OPERATOR_H
#define CALL_CENTER_OPERATOR_H

#include "pch.h"
#include "call.h"
#include "call_center.h"


class Operator : std::enable_shared_from_this<Operator>
{
public:
	Operator(int _id, CallCenter* _callCenter);

	void setCall(std::shared_ptr<Call> &_call);


private:

	int id{0};
	std::shared_ptr<Call> call = nullptr;
	CallCenter* callCenter;

};

#endif
