/* SPDX-FileCopyrightText: Copyright 2025-2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */
#pragma once

#include <cstdint>
#include <istream>
#include <type_traits>
#include <vector>

#include "byteswap.h"

namespace UTILS {

template <class T, typename = std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>>>
/**
 * Reads from stream n bytes and interpreted them as LE integer
 * @tparam T
 * @param input Reads from stream n bytes and interpreted them as LE integer
 * @param value Little Endian Integer based on input T template
 * @param n count of bytes
 * @return pointer to updated stream position
 */

std::istream &bin_read(std::istream &input, T &value, size_t n) {
  input.read(reinterpret_cast<char *>(&value), n);
  switch (sizeof(T)) {
  case 2:
    value = convert_le<uint16_t>(value);
    break;
  case 4:
    value = convert_le<uint32_t>(value);
    break;
  case 8:
    value = convert_le<uint64_t>(value);
    break;
  default:
    break;
  }
  return input;
}

template <class T, typename = std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>>>
/**
 * Lightweight variant, read from stream sizeof(T) bytes and interpreted them as LE integer
 * @tparam T
 * @param input input stream
 * @param value Little Endian Integer based on input T template
 * @return pointer to updated stream position
 */
std::istream &bin_read(std::istream &input, T &value) {
  return bin_read(input, value, sizeof(T));
}

inline std::istream &bin_read(std::istream &input, std::string &value, size_t n) {
  std::vector<char> buffer;
  // add '\0' to the end
  buffer.resize(n + 1);
  input.read(buffer.data(), n);
  value = std::string(buffer.data());
  return input;
}

/**
 * Write to stream LE integer
 * @tparam T
 * @param output output stream
 * @param value Little Endian Integer based on input T template
 * @param n count of bytes
 * @return pointer to updated stream position
 */
template <class T, typename = std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>>>
std::ostream &bin_write(std::ostream &output, T value, size_t n) {
  T write_value;
  switch (sizeof(T)) {
  case 2:
    write_value = convert_le<uint16_t>(value);
    break;
  case 4:
    write_value = convert_le<uint32_t>(value);
    break;
  case 8:
    write_value = convert_le<uint64_t>(value);
    break;
  default:
    write_value = value;
    break;
  }
  output.write(reinterpret_cast<char *>(&write_value), n);
  return output;
}

/**
 * Lightweight variant, write to stream LE integer
 * @tparam T
 * @param output output stream
 * @param value Little Endian Integer based on input T template
 * @return pointer to updated stream position
 */
template <class T, typename = std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>>>
std::ostream &bin_write(std::ostream &output, T value) {
  return bin_write(output, value, sizeof(T));
}

inline std::ostream &bin_write(std::ostream &output, const std::string &value, const size_t n) {
  std::vector<char> buffer;
  std::copy(value.begin(), value.end(), std::back_inserter(buffer));
  buffer.resize(n);
  output.write(&buffer[0], n);
  return output;
}

}
