#ifndef CALL_CENTER_CALL_H
#define CALL_CENTER_CALL_H


#include "pch.h"
#include "operator.h"
#include "cdr.h"


class Call
{
public:

	explicit Call(boost::asio::io_context &_ioContext, const std::string &number);

	CDR& getCDR();

private:

	std::string setUniqueId();
	CDR cdr;
	ptime queueTime;
};

#endif
