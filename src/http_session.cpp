#include <http_session.h>


HttpSession::HttpSession(tcp::socket _socket, std::shared_ptr<CallCenter> &_callCenter)
		: socket(std::move(_socket)), callCenter(_callCenter)
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
								   std::string phoneNumber = processUserData();
								   std::shared_ptr<Call> newCall{std::make_shared<Call>(std::string(phoneNumber))};
								   write(newCall);
							   }
						   });
}

void HttpSession::write(std::shared_ptr<Call> &call)
{
	callCenter->processCall(call);
	std::string userMassage = getMessageForUser(call);

	boost::asio::async_write(socket, buffer(userMassage),
							 [&](boost::system::error_code ec, std::size_t /*length*/) {
								 if (!ec)
								 {
									 // TODO Здесь пользователю приходит ответ в зависимости от статуса
									 // TODO Нужно писать в логер информацию о вызове
								 }
							 });
}

std::string HttpSession::processUserData()
{
	//TODO Реализовать функцию, которая преобразовывает пользовательский запрос в телефонный номер
	return {};
}


std::string HttpSession::getMessageForUser(std::shared_ptr<Call> &call)
{
	//TODO Реализовать функцию, которая формирует запрос для пользователя
	return {};
}