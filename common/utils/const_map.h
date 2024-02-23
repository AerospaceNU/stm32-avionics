#pragma once

#include <array>

// Compile-time map class
template <typename Key, typename Value, size_t N>
class ConstMap {
 public:
  constexpr explicit ConstMap(std::array<std::pair<Key, Value>, N> arr)
      : m_data{arr} {}

  constexpr const Value operator[](const Key&& key) const {
    for (const auto& pair : m_data) {
      if (pair.first == key) {
        return pair.second;
      }
    }
    // Return a default-constructed value if the key is not found
    return Value{};
  }

  const Value operator[](const Key key) const {
    for (const auto& pair : m_data) {
      if (pair.first == key) {
        return pair.second;
      }
    }
    // Return a default-constructed value if the key is not found
    return Value{};
  }

 private:
  std::array<std::pair<Key, Value>, N> m_data;
};
