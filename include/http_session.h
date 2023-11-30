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

	void processService(std::size_t length);

	static std::string determineService(const std::string& requestData);

	void processCallCenterService(std::string &request);

	void write(const std::string &message);

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