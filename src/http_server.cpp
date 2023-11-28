#include "http_server.h"

HttpServer::HttpServer(io_context &_io_context, short _port) :
		acceptor(_io_context, tcp::endpoint(tcp::v4(), _port))
{
	std::cout << "Server start working" << std::endl;
	startAccept();
}

void HttpServer::startAccept()
{
	acceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
		if (!ec)
		{
			auto session = std::make_shared<HttpSession>(std::move(socket), callCenter);
			session->start();
		}
		startAccept();
	});
}
