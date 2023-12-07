#include "boost/asio.hpp"
#include <random>
#include <iostream>
#include <string>
#include <memory>
#include "glog/logging.h"


using namespace boost::asio;
using ip::tcp;



long long getRandomNumber(long long min, long long max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<long long> dis(min, max);
	return dis(gen);
}

std::string getRandomPhoneNumber() {

	auto phoneNumber = getRandomNumber(70000000000LL, 89999999999LL);

	std::ostringstream oss;
	oss << "+" << phoneNumber;

	return oss.str();
}


class AsyncClient
{
public:
	AsyncClient(io_context &io_context, std::string _number)
			: socket{io_context}, resolver{io_context}, number{std::move(_number)}
	{
		data.resize(max_length, '\0');
	}

	void get(const std::string &host, const std::string &service)
	{

		tcp::resolver::query query{host, service};
		resolver.async_resolve(
				query,
				[&](const boost::system::error_code &ec, const tcp::resolver::iterator &it) {
					this->resolveHandler(ec, it);
				}
		);
	}

private:
	void resolveHandler(const boost::system::error_code &ec, const ba::ip::tcp::resolver::iterator &it)
	{
		if (ec)
		{
			std::cout << "Resolve handler for user with phone number " << number << " - failed: " << ec.what()
					  << std::endl;
			return;
		}

		if (it == tcp::resolver::iterator{})
		{
			std::cout << "Resolve handler for user with phone number " << number
					  << " - no address: " << std::endl;
			return;
		}

		socket.async_connect(
				*it,
				[&](const boost::system::error_code &ec) {
					this->connectHandler(ec);
				}
		);
	}

	void connectHandler(const boost::system::error_code &ec)
	{
		if (ec)
		{
			std::cout << "Connect handler for user with phone number " << number
					  << " - failed: " << ec.what() << std::endl;
			return;
		}

		const std::string request = "GET /call-center?phone=" + number + " HTTP/1.1\r\n\r\n";
		write(socket, buffer(request));

		socket.async_read_some(
				buffer(data, max_length),
				[&](const boost::system::error_code &ec, std::size_t length) {
					this->readHandler(ec, length);
				}
		);
	}

	void readHandler(const boost::system::error_code &ec, std::size_t length)
	{
		if (ec)
		{
			std::cout << "Read handler for user with phone number " << number
					  << " - failed: " << ec.what() << std::endl;
			return;
		}

		if (!length)
		{
			std::cout << "Read handler for user with phone number " << number
					  << " - no more data: " << ec.what() << std::endl;
			return;
		}
		std::cout << data;
	}

	tcp::resolver resolver;
	tcp::socket socket;
	enum{
		max_length = 1024
	};
	std::string data;
	std::string number;
};

int main()
{
	std::locale::global(std::locale(""));

	io_context ioContext;

	AsyncClient client(ioContext, getRandomPhoneNumber());

	client.get("127.0.0.1", "1234");

	ioContext.run();

	return 0;
}