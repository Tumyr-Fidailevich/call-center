#ifndef CALL_CENTER_CONFIG_H
#define CALL_CENTER_CONFIG_H

#include "pch.h"

struct Config
{
	int maxQueueSize;
	int maxQueueWaitingTime;
	int numOfOperators;

	std::string configFile;

	explicit Config(std::string &filename);

	inline void update();

private:
	void setDefault();
};

#endif