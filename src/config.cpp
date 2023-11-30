#include "config.h"
#include "logging_macros.h"


Config::Config(std::string &filename) : configFile(std::move(filename))
{
	update();
}


void Config::update()
{
	if (!boost::filesystem::exists(configFile))
	{
		setDefault();
		LOG_TO_FILE(google::GLOG_WARNING, LOG_FILE) << "Could not find the current config";
		return;
	}
	boost::filesystem::ifstream config(configFile);
	if (!config.is_open())
	{
		LOG_TO_FILE(google::GLOG_WARNING, LOG_FILE) << "Could not open the current config";
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
	LOG_TO_FILE(google::GLOG_INFO, LOG_FILE) << "Config updated successfully";
}


void Config::setDefault()
{
	maxQueueSize = 5;
	maxQueueWaitingTime = std::chrono::seconds(60);
	minCallDuration = std::chrono::seconds(20);
	maxCallDuration = std::chrono::seconds(40);
	numOfOperators = 2;
	LOG_TO_FILE(google::GLOG_INFO, LOG_FILE) << "Default config installed successfully";

}
