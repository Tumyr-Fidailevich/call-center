#include <iostream>
#include "http_server.h"

const short PORT = 1234;


int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
	google::InitGoogleLogging(argv[0]);
	FLAGS_log_dir = "../output";
	try
	{
		io_context ioContext;
		HttpServer server(ioContext, PORT);
		ioContext.run();

	} catch (const std::exception &e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	google::ShutdownGoogleLogging();
	return 0;
}
