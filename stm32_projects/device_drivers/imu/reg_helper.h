#pragma once

#include <stdint.h>

#include <cassert>
#include <iostream>
#include <type_traits>

// from https://stackoverflow.com/a/75844949

template <uint8_t size>
requires(size <= sizeof(uint64_t)) struct size_to_type;

template <>
struct size_to_type<sizeof(uint64_t)> {
  using result = uint64_t;
};

template <>
struct size_to_type<sizeof(uint32_t)> {
  using result = uint32_t;
};

template <>
struct size_to_type<sizeof(uint16_t)> {
  using result = uint16_t;
};

template <>
struct size_to_type<sizeof(uint8_t)> {
  using result = uint8_t;
};

template <typename T>
requires(sizeof(T) <=
         sizeof(uint64_t)) using size_type_of = size_to_type<sizeof(T)>::result;

template <typename T>
requires(sizeof(T) <=
         sizeof(uint64_t)) constexpr auto to_size_type(T x) noexcept
    -> size_type_of<T> {
  return *reinterpret_cast<size_type_of<T>*>(&x);
}