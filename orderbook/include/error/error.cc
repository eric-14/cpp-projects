#include "error.hpp"


template <typename... Fs> struct match : Fs... {
  using Fs::operator()...;
};

void displayError(const SystemError::OError &error) {
  std::visit(match{
                 [](const SystemError::UndefinedState &error) {
                   // logger.print(Undefined state.error_message)
                 },
                 [](const SystemError::ConstructionError &error) {
                   // logger.println(undefined state.error_messag)
                 },
                 [](const SystemError::OrderFillLimitError &error) {
                   // logger.println(undefined state.error_messag)
                 },
                 [](const SystemError::OrderEntryError &error) {
                   // logger.println(undefined state.error_messag)
                 },
                 [](const SystemError::SnapFailedToSave &error) {
                   // logger.println(undefined state.error_messag)
                 },
                 [](const SystemError::OrderFailedToBuild &error) {
                   // logger.println(undefined state.error_messag)
                 },

                 [](const SystemError::OrderValidationError &error) {
                   // logger.println(undefined state.error_messag)
                 },

             },
             error);
}