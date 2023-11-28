#include "config.h"

Config::Config(std::string &filename) : configFile(std::move(filename))
{
	update();
}


void Config::update()
{
	if (!boost::filesystem::exists(configFile))
	{
		//TODO здесь должно быть логирование - не получилось открыть конфигурационный файл
		setDefault();
	}
	boost::filesystem::ifstream config(configFile);
	if (!config.is_open())
	{
		//TODO здесь также должно быть логирование
		setDefault();
	}

	nlohmann::json jsonConfig;
	config >> jsonConfig;

	maxQueueSize = jsonConfig.value("max_queue_length", 100);
	maxQueueWaitingTime = jsonConfig.value("queue_waiting_time", 120);
	numOfOperators = jsonConfig.value("number_of_operators", 5);

}


void Config::setDefault()
{
	maxQueueSize = 5;
	maxQueueWaitingTime = 120;
	numOfOperators = 2;

}
