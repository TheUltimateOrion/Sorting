#pragma once
#include "core/AppCtx.h"

#define LOGINFO(str) std::cout << "[INFO]"  << '[' << AppCtx::getTimestamp() << "s]: " << str << std::endl;
#define LOGERR(str)  std::cerr << "[ERROR]" << '[' << AppCtx::getTimestamp() << "s]: " << str << std::endl;