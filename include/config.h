#ifndef CALL_CENTER_CONFIG_H
#define CALL_CENTER_CONFIG_H

#include "pch.h"


struct Config
{
	int maxQueueSize{};
	std::chrono::seconds maxQueueWaitingTime{0};
	std::chrono::seconds minCallDuration{0};
	std::chrono::seconds maxCallDuration{0};
	int numOfOperators{};

	std::string configFile;

	explicit Config(std::string &filename);

	inline void update();

private:
	void setDefault();
};

#endif