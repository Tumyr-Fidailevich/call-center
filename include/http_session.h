#ifndef CALL_CENTER_HTTP_SESSION_H
#define CALL_CENTER_HTTP_SESSION_H

#include "pch.h"
#include "call_center.h"
#include "call.h"



using namespace boost::asio;
using namespace boost::posix_time;
using ip::tcp;

class HttpSession : public std::enable_shared_from_this<HttpSession>
{
public:
	HttpSession(tcp::socket _socket, io_context &_ioContext, std::shared_ptr<CallCenter> &_callCenter);

	void start();

private:

	void read();

	void write(const std::string &message);

	static std::string getMessageForUser(std::shared_ptr<Call> &call);

	enum
	{
		max_length = 1024
	};
	char data[max_length];
	io_context &ioContext;
	tcp::socket socket;
	std::shared_ptr<CallCenter> callCenter;
};


#endif