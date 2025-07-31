#pragma once
#include <iostream>

#include "core/timer.h"

#define LOGINFO(str) std::cout << "[INFO]"  << '[' << Core::Timer::getTimestamp() << "s]: " << str << std::endl;
#define LOGERR(str)  std::cerr << "[ERROR]" << '[' << Core::Timer::getTimestamp() << "s]: " << str << std::endl;
