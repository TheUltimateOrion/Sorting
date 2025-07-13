#pragma once
#include <iostream>

#include "core/app_ctx.h"

#define LOGINFO(str) std::cout << "[INFO]"  << '[' << AppCtx::getTimestamp() << "s]: " << str << std::endl;
#define LOGERR(str)  std::cerr << "[ERROR]" << '[' << AppCtx::getTimestamp() << "s]: " << str << std::endl;