#ifndef CALL_CENTER_CDR_H
#define CALL_CENTER_CDR_H

#include "pch.h"


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

	boost::posix_time::ptime connectionTime;
	boost::posix_time::ptime responseTime;
	boost::posix_time::ptime disconnectTime;
	std::chrono::seconds callDuration;
	int operatorId{0};
	std::string callId{};
	std::string phoneNumber{};
	Status status;

	std::string getFullRepresentation();

	void write();
};

#endif
