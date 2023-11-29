#ifndef CALL_CENTER_CDR_H
#define CALL_CENTER_CDR_H

#include "pch.h"

using namespace boost::posix_time;

struct CDR
{
	enum class Status
	{
		OK,
		Duplicate,
		Overload,
		Timeout,
		Interrupted
	};
	ptime connectionTime;
	ptime responseTime;
	ptime disconnectTime;
	int callDuration{0};
	int operatorId{0};
	std::string callId{};
	std::string phoneNumber{};
	std::string status{};

	void write();
};

void CDR::write()
{

}

#endif
