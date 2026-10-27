#pragma once
#include <array>
#include <cstdint>
#include <memory>

namespace cds::bench {

/// A queue element that owns a shared allocation, so every move carries a reference count.
struct SharedPayload {
    std::uint32_t                                 kind{};
    std::shared_ptr<const std::array<double, 40>> bands;
};

inline SharedPayload make_shared_payload() {
    return {.kind = 1, .bands = std::make_shared<const std::array<double, 40>>()};
}

}  // namespace cds::bench
