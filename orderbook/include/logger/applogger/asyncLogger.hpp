#ifndef __ASYNC__
#define __ASYNC__

#include "../../error/error.hpp"
#include <expected>
#include <string>
namespace Market::core {
namespace Logger {
class asyncLogger {
public:
  std::expected<bool, SystemError::UndefinedState> addLog(std::string_view log);

  std::expected<bool, SystemError::UndefinedState>
  addError(SystemError::OError error);
};
} // namespace Logger
} // namespace Market::core

#endif