#include "asyncLogger.hpp"

namespace MLogger = Market::core::Logger;

std::expected<bool, SystemError::UndefinedState>
MLogger::asyncLogger::addLog(std::string_view log) {
  return true; // To be implemented later
}

std::expected<bool, SystemError::UndefinedState>
MLogger::asyncLogger::addError(SystemError::OError error) {
  return true; // to be implemented later
}