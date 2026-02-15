/* SPDX-FileCopyrightText: Copyright 2025-2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include <array>
#include "CRC32.h"

namespace UTILS {

std::array<uint32_t, 256> generate_crc_table();

static std::array<uint32_t, 256> crc_table = generate_crc_table();

std::array<std::uint32_t, 256> generate_crc_table() {
  uint32_t polynomial = 0xEDB88320;

  for (uint32_t i = 0; i < 256; ++i) {
    uint32_t c = i;
    for (size_t j = 0; j < 8; ++j) {
      if (c & 1) {
        c = polynomial ^ (c >> 1);
      } else {
        c >>= 1;
      }
    }
    crc_table[i] = c;
  }
  return crc_table;
}

uint32_t crc32(const std::string& data) {
  uint32_t crc = 0xFFFFFFFF; // Initial value

  for (const unsigned char byte : data) {
    crc = crc_table[(crc ^ byte) & 0xFF] ^ (crc >> 8);
  }

  return crc ^ 0xFFFFFFFF; // Final XOR
}

}
