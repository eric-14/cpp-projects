

#ifndef __SYSTEM_ERROR__
#define __SYSTEM_ERROR__

#include <string>
#include <variant>

namespace SystemError {

template <typename... Fs> struct match : Fs... {
  using Fs::operator()...;
};

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

using Error =
    std::variant<UndefinedState, ConstructionError, OrderFillLimitError,
                 OrderEntryError, SnapFailedToSave, OrderFailedToBuild,
                 OrderValidationError>;

void displayError(const Error &error) {
  std::visit(match{
                 [](const UndefinedState &error) {
                   // logger.print(Undefined state.error_message)
                 },
                 [](const ConstructionError &error) {
                   // logger.println(undefined state.error_messag)
                 },
                 [](const OrderFillLimitError &error) {
                   // logger.println(undefined state.error_messag)
                 },
                 [](const OrderEntryError &error) {
                   // logger.println(undefined state.error_messag)
                 },
                 [](const SnapFailedToSave &error) {
                   // logger.println(undefined state.error_messag)
                 },
                 [](const OrderFailedToBuild &error) {
                   // logger.println(undefined state.error_messag)
                 },

                 [](const OrderValidationError &error) {
                   // logger.println(undefined state.error_messag)
                 },

             },
             error);
}

} // namespace SystemError

#endif