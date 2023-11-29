#ifndef CALL_CENTER_RANDOM_H
#define CALL_CENTER_RANDOM_H

#include "pch.h"


std::chrono::seconds getRandomCallDuration(const std::chrono::seconds &min, const std::chrono::seconds &max);

#endif
