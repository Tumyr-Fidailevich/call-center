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
								   processService(length);
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
		LOG_TO_FILE(google::GLOG_ERROR, LOG_FILE) << "User with invalid service request " << service
												  << " try to connect";
		write("Cant find this service on the server");
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
		newCall->setReleaseCallback([this](std::shared_ptr<Call> &call) { writeCall(call); });
		callCenter->processCall(newCall);
		LOG_TO_FILE(google::GLOG_INFO, LOG_FILE) << "User with phone number " << phoneNumber
												 << " connected to the server";
	} else
	{
		LOG_TO_FILE(google::GLOG_WARNING, LOG_FILE) << "User with invalid phone number " << phoneNumber
													<< " try to connect";
		write("Invalid phone number");

	}

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
