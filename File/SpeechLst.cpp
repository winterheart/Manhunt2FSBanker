/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include <fstream>
#include "SpeechLst.h"

namespace MH2FSB {

std::ostream & operator<<(std::ostream &out, const SpeechLst &c){
  for (auto const &entry : c.m_entries) {
    out << entry << "\r\n";
  }
  return out;
}

std::istream & operator>>(std::istream &in, SpeechLst &c){
  std::string line;
  while (std::getline(in, line)) {
    if (!line.empty()) {
      if (line.back() == '\r') {
        line.pop_back();
      }
      c.m_entries.push_back(line);
    }
  }
  return in;
}

}
