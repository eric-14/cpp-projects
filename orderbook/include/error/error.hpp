

#ifndef __SYSTEM_ERROR__
#define __SYSTEM_ERROR__

#include <variant>
#include <string>


namespace SystemError {

struct UndefinedState {
  std::string_view error_meesage;
};
struct ConstructionError {
  std::string_view error_message;
};
struct OrderFillLimitError {
  std::string_view error_message;
};
struct OrderEntryError {
  std::string_view error_message;
};
struct SnapFailedToSave {
  std::string_view error_message;
};
struct OrderFailedToBuild {
  std::string_view error_messaege;
};
struct OrderValidationError {
  std::string_view error_messaege;
};

using OError =
    std::variant<UndefinedState, ConstructionError, OrderFillLimitError,
                 OrderEntryError, SnapFailedToSave, OrderFailedToBuild,
                 OrderValidationError>;

void displayError(const SystemError::OError &error);

} // namespace SystemError

#endif