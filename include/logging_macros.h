#ifndef CALL_CENTER_LOGGING_MACROS_H
#define CALL_CENTER_LOGGING_MACROS_H

#include "pch.h"
#define LOG_TO_FILE(level, filename) google::LogMessage("", 0, level).stream()

#endif
