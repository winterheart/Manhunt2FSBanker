/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include <cassert>
#include "Dir.h"

#include "binio.h"

namespace MH2FSB {

std::ostream &operator<<(std::ostream &out, Dir &c) {
  // TODO: Implement
  assert(false);
  return out;
}

std::istream &operator>>(std::istream &in, Dir &c) {
  const auto cur_position = in.tellg();
  in.seekg(0, std::ios::end);
  const auto length = in.tellg() - cur_position;
  in.seekg(cur_position, std::ios::beg);
  for (int i = 0; i < length; i += 4) {
    uint32_t crc;
    UTILS::bin_read(in, crc);
    c.m_entries.push_back(crc);
  }

  return in;
}

} // MH2FSB
