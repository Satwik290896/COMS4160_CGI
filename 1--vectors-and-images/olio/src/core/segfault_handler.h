//! \file       segfault_handler.h
//! \brief      Misc functions for catching and handling segfaults
//! \author     Hadi Fadaifard, 2022

#pragma once

#include <memory>
#include <string>

namespace olio {

void InstallSegfaultHandler();
std::string Backtrace(int skip);

}  // namespace olio
