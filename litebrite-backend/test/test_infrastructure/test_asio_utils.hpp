//
// Created by broosegoose on 7/19/23.
//

#ifndef TEST_INFRASTRUCTURE_ASIO_UTILS_HPP
#define TEST_INFRASTRUCTURE_ASIO_UTILS_HPP

#include <optional>

#include "infrastructure/asio/context.hpp"

[[nodiscard]] inline infrastructure::AsioContextPtr GetAsioTestContext(
    std::optional<int> pool_size
) {
    return std::move(
        infrastructure::AsioContext::Create(
            infrastructure::AsioContextConfig{
                .asio_pool_size = pool_size.has_value() ? pool_size.value() : 1
            }
        )
    );
}

#endif //TEST_INFRASTRUCTURE_ASIO_UTILS_HPP
