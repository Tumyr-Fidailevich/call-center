#include "utils.h"

std::chrono::seconds getRandomCallDuration(const std::chrono::seconds &min, const std::chrono::seconds &max) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<std::chrono::seconds::rep> dist(min.count(), max.count());

	return std::chrono::seconds(dist(gen));
}