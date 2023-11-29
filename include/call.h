#ifndef CALL_CENTER_CALL_H
#define CALL_CENTER_CALL_H


#include "pch.h"
#include "cdr.h"


class Call
{
public:

	explicit Call(const std::string &number);

	CDR& getCDR();

private:

	std::string setUniqueId();
	CDR cdr;
	ptime queueTime;
};

#endif