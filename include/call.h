#ifndef CALL_CENTER_CALL_H
#define CALL_CENTER_CALL_H


#include "pch.h"
#include "operator.h"
#include "cdr.h"


class Call
{
public:

	using Callback = std::function<void(std::shared_ptr<Call> &)>;

	explicit Call(boost::asio::io_context &_ioContext, const std::string &number);

	void startTimer(const std::chrono::seconds &timeout, const Callback &callback);

	CDR& getCDR();

private:

	Callback releaseCallback;

	boost::asio::steady_timer timer;

	std::string getUniqueId() const;

	CDR cdr;

};

#endif
