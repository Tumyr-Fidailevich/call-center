#ifndef CALL_CENTER_HTTP_SERVER_H
#define CALL_CENTER_HTTP_SERVER_H

#include "pch.h"
#include "http_session.h"
#include "call_center.h"


using namespace boost::asio;
using ip::tcp;

class HttpServer
{
public:
	HttpServer(io_context &_ioContext, short _port);

private:
	io_context &ioContext;

	tcp::acceptor acceptor;

	std::shared_ptr<CallCenter> callCenter{};

	void startAccept();

};

#endif
