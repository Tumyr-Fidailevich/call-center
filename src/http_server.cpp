#include "http_server.h"

HttpServer::HttpServer(io_context &_ioContext, short _port) :
		acceptor(_ioContext, tcp::endpoint(tcp::v4(), _port)), ioContext(_ioContext)
{
	std::cout << "Server start working" << std::endl;
	startAccept();
}

void HttpServer::startAccept()
{
	acceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
		if (!ec)
		{
			auto session = std::make_shared<HttpSession>(std::move(socket), ioContext, callCenter);
			session->start();
		}
		startAccept();
	});
}
