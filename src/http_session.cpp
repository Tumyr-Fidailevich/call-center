#include "http_session.h"


HttpSession::HttpSession(tcp::socket _socket, io_context &_ioContext, std::shared_ptr<CallCenter> &_callCenter)
		: socket(std::move(_socket)), ioContext(_ioContext), callCenter(_callCenter)
{
	data.resize(max_length);
}


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
								   processService(length);
							   } else
							   {
								   LOG(ERROR) << "Error during reading user message: " << ec.what();
							   }
						   });
}

void HttpSession::processService(std::size_t length)
{
	std::string requestData(data, length);
	std::string service = determineService(requestData);
	if (service == "call-center")
	{
		processCallCenterService(requestData);
	} else
	{
		LOG(WARNING) << "User with undefined service request " << service << " try to connect";
		writeError("Error: undefined service");
	}
}

std::string HttpSession::determineService(const std::string &requestData)
{
	if (requestData.find("/call-center") != std::string::npos)
	{
		return "call-center";
	}
	return "";
}

void HttpSession::processCallCenterService(std::string &request)
{
	std::string phoneNumber = CallCenter::getPhoneNumberFromRequest(request);
	if (CallCenter::isNumberValid(phoneNumber))
	{
		std::shared_ptr<Call> newCall{
				std::make_shared<Call>(ioContext, std::string(phoneNumber))};
		auto self(shared_from_this());
		newCall->setReleaseCallback([this, self](std::shared_ptr<Call> call) {
			writeCall(call);
		});
		callCenter->processCall(newCall);
		LOG(INFO) << "User with phone number " << phoneNumber << " connected to the server";
	} else
	{
		LOG(WARNING) << "User with invalid phone number " << phoneNumber << " try to connect";
		writeError("Error: invalid phone number");
	}

}

void HttpSession::writeCall(std::shared_ptr<Call> &call)
{
	std::string userMessage = CallCenter::getMessageForUser(call);
	data.assign(userMessage.substr(0, std::min(userMessage.size(), data.size())));
	LOG(INFO) << "User with phone number " << call->getCDR().phoneNumber << " has disconnected from the server";
	call->getCDR().write();
	write(userMessage.size());
}

void HttpSession::writeError(const std::string& body)
{
	data.assign(body.substr(0, std::min(body.size(), data.size())));
	write(body.size());
}

void HttpSession::write(std::size_t length)
{
	boost::asio::async_write(socket, buffer(data, length),
							 [this](boost::system::error_code ec, std::size_t /*length*/) {
								 if (ec)
								 {
									 LOG(ERROR) << "Error during send message to user: " << ec.what();
								 }
								 data.resize(max_length, '\0');
							 });
}
