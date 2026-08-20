#include "snapStore.hpp"
#include <memory>
#include <variant>

namespace MCORE_S = Market::core::Snap;
namespace MCORE_O = Market::core::Order;
namespace MCORE = Market::core;

MCORE::system_time_t MCORE_S::snapStore::getTimeStamp() const noexcept {
  return m_timestamp;
}

std::string_view
MCORE_S::snapStore::snapObjName(const snapObject obj) noexcept {
  auto id = std::visit(
      [](auto &&ptr) {
        using T = std::decay_t<decltype(ptr)>;
        if constexpr (std::is_pointer_v<T>) {
          if (ptr) {
            std::string name = std::format(
                "{}:{}", ptr->getId(),
                "timestamp"); // ptr->getTimestamp().time_since_epoch()
            return name;
          }
        }
        return std::string("Undefined");
      },
      obj);
}
