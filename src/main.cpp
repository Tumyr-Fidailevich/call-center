#include <iostream>
#include "http_server.h"

const int PORT = 1234;


int main(int argc, char* argv[])
{
	try
	{
		io_context ioContext;
		HttpServer server(ioContext, PORT);
		ioContext.run();

	} catch (const std::exception &e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	return 0;
}
