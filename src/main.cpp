#include <iostream>
#include "http_server.h"

const short PORT = 1234;


int main(int argc, char* argv[])
{
	google::InitGoogleLogging("Call center");
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
