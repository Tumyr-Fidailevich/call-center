#include "cdr.h"

std::string CDR::getFullRepresentation()
{
	std::ostringstream fullRepresentation;

	fullRepresentation << boost::posix_time::to_simple_string(connectionTime) << ';';

	fullRepresentation << callId << ';';

	fullRepresentation << phoneNumber << ';';

	fullRepresentation << boost::posix_time::to_simple_string(disconnectTime) << ';';

	switch (status) {
		case Status::OK:
			fullRepresentation << "OK";
			break;
		case Status::Duplicate:
			fullRepresentation << "Duplicate";
			break;
		case Status::Overload:
			fullRepresentation << "Overload";
			break;
		case Status::Timeout:
			fullRepresentation << "Timeout";
			break;
		case Status::Interrupted:
			fullRepresentation << "Interrupted";
			break;
	}
	fullRepresentation << ';';

	if (responseTime.is_not_a_date_time()) {
		fullRepresentation << ';';
	} else {
		fullRepresentation << boost::posix_time::to_simple_string(responseTime) << ';';
	}

	fullRepresentation << operatorId << ';';

	if (status == Status::OK) {
		fullRepresentation << callDuration.count();
	}

	return fullRepresentation.str();
}
