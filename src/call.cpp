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
	size_t hashValue = 0;
	boost::hash_combine(hashValue, cdr.phoneNumber);
	boost::hash_combine(hashValue, boost::posix_time::to_iso_string(cdr.connectionTime));
	return std::to_string(hashValue);
}

void Call::startTimer(const std::chrono::seconds &timeout, const Callback &callback)
{
	auto self(shared_from_this());
	timer.expires_after(timeout);
	timer.async_wait([&](const boost::system::error_code &e) { callback(self); });
}