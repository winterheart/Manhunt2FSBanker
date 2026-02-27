/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */
#pragma once

#include <map>
#include <string>
#include <vector>

#include "FileContainer.h"

namespace MH2FSB {

/// Some speech.lst files has inconsistencies. This class provides corrections.
class SpeechLstQuirks {
public:
  static std::vector<std::string> Resolve(const std::string& level_name);
private:
  static std::map<std::string, std::vector<std::string>> m_quirks;
};

class SpeechLst : public FileContainer {
public:
  std::vector<std::string> GetEntries() { return m_entries; }
  void SetEntries(const std::vector<std::string> &entries) { m_entries = entries; }

  friend std::ostream &operator<<(std::ostream &out, const SpeechLst &c);
  friend std::istream &operator>>(std::istream &in, SpeechLst &c);

private:
  std::vector<std::string> m_entries;
};

} // namespace MH2FSB
