#include "config.h"


Config::Config(std::string &filename) : configFile(std::move(filename))
{
	update();
}


void Config::update()
{
	if (!boost::filesystem::exists(configFile))
	{
		setDefault();
		LOG(WARNING) << "Could not find current config path";
		return;
	}
	boost::filesystem::ifstream config(configFile);
	if (!config.is_open())
	{
		LOG(WARNING) << "Could not open current config";
		setDefault();
		return;
	}

	nlohmann::json jsonConfig;
	config >> jsonConfig;

	maxQueueSize = jsonConfig.value("max_queue_length", 100);
	maxQueueWaitingTime = std::chrono::seconds(jsonConfig.value("queue_waiting_time", 120));
	minCallDuration = std::chrono::seconds(jsonConfig.value("min_call_duration", 20));
	maxCallDuration = std::chrono::seconds(jsonConfig.value("max_call_duration", 40));
	numOfOperators = jsonConfig.value("number_of_operators", 5);
	LOG(INFO) << "Config updated successfully";
}


void Config::setDefault()
{
	maxQueueSize = 5;
	maxQueueWaitingTime = std::chrono::seconds(60);
	minCallDuration = std::chrono::seconds(20);
	maxCallDuration = std::chrono::seconds(40);
	numOfOperators = 2;
	LOG(INFO) << "Default config installed successfully";
}
