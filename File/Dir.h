/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */

#pragma once

#include <cstdint>
#include <fstream>
#include <vector>

namespace MH2FSB {

class Dir {
public:
  [[nodiscard]] uint32_t GetEntriesId(uint32_t index) const { return m_entries.at(index); }
private:
  std::vector<uint32_t> m_entries;

  friend std::ostream &operator<<(std::ostream &out, Dir &c);
  friend std::istream &operator>>(std::istream &in, Dir &c);
};

} // MH2FSB
