#include "http_session.h"
#include "logging_macros.h"


HttpSession::HttpSession(tcp::socket _socket, io_context &_ioContext, std::shared_ptr<CallCenter> &_callCenter)
		: socket(std::move(_socket)), ioContext(_ioContext), callCenter(_callCenter)
{}


void HttpSession::start()
{
	read();
}

void HttpSession::read()
{
	auto self(shared_from_this());
	socket.async_read_some(buffer(data, max_length),
						   [this, self](boost::system::error_code ec, std::size_t length) {
							   if (!ec)
							   {
								   std::string phoneNumber = processUserRequest();
								   std::shared_ptr<Call> newCall{std::make_shared<Call>(std::string(phoneNumber))};
								   write(newCall);
							   }
						   });
}

void HttpSession::write(const std::string &message)
{
	boost::asio::async_write(socket, buffer(message),
							 [&](boost::system::error_code ec, std::size_t /*length*/) {
								 if (!ec)
								 {

								 }else
								 {
									 LOG_TO_FILE(google::GLOG_ERROR, LOG_FILE) << "Error during send message to user";
								 }
							 });
}

std::string HttpSession::getMessageForUser(std::shared_ptr<Call> &call)
{
	std::string status{};
	switch (call->getCDR().status)
	{
		case CDR::Status::OK:
		{
			status = "OK";
			break;
		}
		case CDR::Status::Duplicate:
		{
			status = "Duplicate";
			break;
		}
		case CDR::Status::Overload:
		{
			status = "Overload";
			break;
		}
		case CDR::Status::Timeout:
		{
			status = "Timeout";
			break;
		}
		case CDR::Status::Interrupted:
		{
			status = "Interrupted";
			break;
		}
	}
	return std::string{status + " " + call->getCDR().callId};
}