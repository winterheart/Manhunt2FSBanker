/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "ContextMapBin.h"

#include "../binio.h"

namespace MH2FSB {

std::ostream& operator<<(std::ostream& out, const ContextMapBin& c) {
  UTILS::bin_write(out, c.m_bank_name, 32);
  for (auto i : c.m_speech_entries) {
    UTILS::bin_write(out, i);
  }
  return out;
}

std::istream& operator>>(std::istream& in, ContextMapBin& c) {
  UTILS::bin_read(in, c.m_bank_name, 32);
  for (auto &i : c.m_speech_entries) {
    UTILS::bin_read(in, i);
  }
  return in;
}

} // MH2FSB
