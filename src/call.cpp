#include "call.h"

Call::Call(const std::string &number)
{
	cdr.callId = setUniqueId();
	cdr.phoneNumber = number;
	cdr.connectionTime = boost::posix_time::second_clock::local_time();
}

CDR &Call::getCDR()
{
	return cdr;
}

std::string Call::setUniqueId()
{
	return {};
}
