/*
 * bit_helper.h
 *
 *  Created on: Jul 16, 2022
 *      Author: matth
 */

#ifndef COMMON_UTILS_BIT_HELPER_H_
#define COMMON_UTILS_BIT_HELPER_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t set_bit(uint8_t value, uint8_t offset);

uint8_t clear_bit(uint8_t value, uint8_t offset);

uint8_t get_bit(uint8_t value, uint8_t place);

uint16_t combine_to_u16(uint8_t high, uint8_t low);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <bit>
#include <algorithm>
namespace std {
template<std::integral T>
constexpr T byteswap(T value) noexcept
{
    static_assert(std::has_unique_object_representations_v<T>,
                  "T may not have padding bits");
    auto value_representation = std::bit_cast<std::array<std::byte, sizeof(T)>>(value);
    std::ranges::reverse(value_representation);
    return std::bit_cast<T>(value_representation);
}
}
#endif

#endif  // COMMON_UTILS_BIT_HELPER_H_
