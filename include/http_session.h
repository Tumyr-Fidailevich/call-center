#ifndef CALL_CENTER_HTTP_SESSION_H
#define CALL_CENTER_HTTP_SESSION_H

#include "pch.h"
#include "operator.h"
#include "call_center.h"


using namespace boost::asio;
using namespace boost::posix_time;
using ip::tcp;

class HttpSession : public std::enable_shared_from_this<HttpSession>
{
public:
	HttpSession(tcp::socket _socket, std::shared_ptr<CallCenter> &_callCenter);

	void start();

private:

	void read();

	void write(std::shared_ptr<Call> &call);

	std::string processUserData();

	std::string getMessageForUser(std::shared_ptr<Call> &call);

	enum
	{
		max_length = 1024
	};
	char data[max_length];
	tcp::socket socket;
	std::shared_ptr<CallCenter> callCenter;
};


#endif