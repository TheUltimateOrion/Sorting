#pragma once

#include <iostream>
#include <thread>

#include "core/timer.h"

#define LOGINFO(str)                                                        \
    std::cout << "[INFO][Thread ID: " << std::this_thread::get_id() << "][" \
              << Core::Timer::getTimestamp() << "s]: " << str << std::endl;
#define LOGERR(str)                                                          \
    std::cerr << "[ERROR][Thread ID: " << std::this_thread::get_id() << "][" \
              << Core::Timer::getTimestamp() << "s]: " << str << std::endl;
