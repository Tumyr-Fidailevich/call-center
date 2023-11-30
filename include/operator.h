#ifndef CALL_CENTER_OPERATOR_H
#define CALL_CENTER_OPERATOR_H

#include "pch.h"


class Operator
{
public:
	explicit Operator(int _id);

	[[nodiscard]] int getId() const;

private:

	int id{0};

};

#endif
