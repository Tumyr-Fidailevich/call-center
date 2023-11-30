#include "http_server.h"
#include "logging_macros.h"


HttpServer::HttpServer(io_context &_ioContext, short _port) :
		acceptor(_ioContext, tcp::endpoint(tcp::v4(), _port)), ioContext(_ioContext)
{
	LOG_TO_FILE(google::GLOG_INFO, LOG_FILE) << "Server is running";
	startAccept();
}

void HttpServer::startAccept()
{
	acceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
		if (!ec)
		{
			auto session = std::make_shared<HttpSession>(std::move(socket), ioContext, callCenter);
			session->start();
		}else
		{
			LOG_TO_FILE(google::GLOG_ERROR, LOG_FILE) << "Error during create new connection";
		}
		startAccept();
	});
}
