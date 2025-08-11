#pragma once

#include "core/timer.h"

#include <iostream>
#include <thread>

#define LOGINFO(str)                                                        \
    std::cout << "[INFO][Thread ID: " << std::this_thread::get_id() << "][" \
              << Core::Timer::GetTimestamp() << "s]: " << str << std::endl;
#define LOGERR(str)                                                          \
    std::cerr << "[ERROR][Thread ID: " << std::this_thread::get_id() << "][" \
              << Core::Timer::GetTimestamp() << "s]: " << str << std::endl;
