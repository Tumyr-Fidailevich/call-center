#ifndef CALL_CENTER_OPERATOR_H
#define CALL_CENTER_OPERATOR_H

#include "pch.h"


class Operator
{
public:
	explicit Operator(int _id);

	void setCall(std::shared_ptr<Call> &_call);


private:

	int id{0};
	std::shared_ptr<Call> call = nullptr;

};

#endif
