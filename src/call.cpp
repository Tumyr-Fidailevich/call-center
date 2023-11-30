#include "call.h"
#include "logging_macros.h"


Call::Call(const std::string &number)
{
	cdr.phoneNumber = number;
	cdr.connectionTime = boost::posix_time::second_clock::local_time();
	cdr.callId = getUniqueId();
}

CDR &Call::getCDR()
{
	return cdr;
}

std::string Call::setUniqueId()
{
	return {};
}
