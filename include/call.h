#ifndef CALL_CENTER_CALL_H
#define CALL_CENTER_CALL_H


#include "pch.h"
#include "operator.h"
#include "cdr.h"


class Call : public std::enable_shared_from_this<Call>
{
public:

	using Callback = std::function<void(std::shared_ptr<Call> &)>;

	explicit Call(boost::asio::io_context &_ioContext, const std::string &number);

	CDR& getCDR();

	void startTimer(const std::chrono::seconds &timeout, const Callback &callback);

	void stopTimer();

	void setReleaseCallback(const Callback &callback);

	void setOperator(std::shared_ptr<Operator> &_op);

private:

	Callback releaseCallback;

	boost::asio::steady_timer timer;

	std::string getUniqueId() const;

	CDR cdr;

};

#endif
