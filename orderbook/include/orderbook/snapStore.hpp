#ifndef __SNAP__
#define __SNAP__
#include "../logger/applogger/asyncLogger.hpp"
#include "../order/order.hpp"
#include "../users/users.hpp"
#include "orderbook.hpp"
#include <expected>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>

namespace Market::core {

namespace Snap {
/// Taking a snapshop of system objects including the orderbook, order,
/// validation engine, matching engine
//  to save data to disk
/// This is core to the State machine reply architecture.
/// It means that the state of the object can be replayed until the object is
/// fully recovered

using snapObject =
    std::variant<std::monostate, Trading::Orderbook *, Order::Order *,
                 Validation::validationEngine *, Users::User *>;
class snapStore {
private:
  std::string m_name;
  std::string m_Id;
  system_time_t m_timestamp;
  std::shared_ptr<Logger::asyncLogger> m_asyncLogger;

  /// Map with file paths of different snaps for different parts of the system
  /// Note: this is not in the hot path therefore map is used. Speed is not
  /// critical
  std::unordered_map<std::string, std::string> m_storage;

  // std::expected<>

  /// Objects that can be stored in the snap system
  /// Compatability is not enforced by inheritance or abstract class rather
  /// Using variants and trusting member classes implement the required methods
  void snapObjectParser(const snapObject &obj) {
    // 2. Use std::visit to pass the active inner object to a generic lambda
    // auto id = std::visit(
    //     [](const auto &active_item) -> int {
    //       using T = std::decay_t<decltype(active_item)>;

    //       // Handle std::monostate (which represents an empty/uninitialized
    //       // state)
    //       if constexpr (std::is_same_v<T, std::monostate>) {
    //         return -1; // Or handle an empty state appropriately
    //       }
    //       // Ensure the remaining types actually have a getId() method
    //       else {
    //         return active_item->getId();
    //       }
    //     },
    //     obj);
  }

public:
  snapStore(std::string name, system_time_t timestamp, std::string Id)
      : m_name{name}, m_Id{Id}, m_timestamp{timestamp} {

    auto state = m_asyncLogger->addLog(
        std::format("[{}] Created Snapstore with name {}",
                    m_timestamp.time_since_epoch(), m_name));
  }

  bool addSystem(snapObject obj) {
    /// move tracked bids and asks to snap store
    /// trigger storage event
    // create a date string
    std::string_view objName = snapObjName(obj);
    std::string objName_ = static_cast<std::string>(objName);
    if (m_storage.find(objName_) != m_storage.end()) {
      // has been snapped
      // m_storage[objName_] =
    }
  }

  std::string_view snapObjName(const snapObject obj) noexcept;

  system_time_t getTimeStamp() const noexcept;

  [[nodiscard]] constexpr std::string_view getName() noexcept;
  [[nodiscard]] constexpr std::string_view getID() noexcept;
  [[nodiscard]] constexpr system_time_t getTimeStamp() noexcept;
};

} // namespace Snap
} // namespace Market::core

#endif