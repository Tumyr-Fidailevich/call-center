#include "call.h"
#include "logging_macros.h"


Call::Call(boost::asio::io_context &_ioContext, const std::string &number) : timer(_ioContext)
{
	cdr.phoneNumber = number;
	cdr.connectionTime = boost::posix_time::second_clock::local_time();
	cdr.callId = getUniqueId();
}

CDR &Call::getCDR()
{
	return cdr;
}

std::string Call::getUniqueId() const
{
	return {};
}
