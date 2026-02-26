/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */

#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace MH2FSB {

extern std::vector<std::string> LevelNames;
extern std::vector<std::string> SpeechNames;

class Crc32ResolveNames {
public:
  static Crc32ResolveNames& getInstance() {
    static Crc32ResolveNames instance;
    return instance;
  }

  std::string getScriptedName(uint32_t crc32) {
    if (m_crc32map_scripted.contains(crc32)) {
      return m_crc32map_scripted[crc32];
    }
    return "";
  }

  std::string getExecutionsName(uint32_t crc32) {
    if (m_crc32map_executions.contains(crc32)) {
      return m_crc32map_executions[crc32];
    }
    return "";
  }

private:
  Crc32ResolveNames();
  static std::map<uint32_t, std::string> m_crc32map_scripted;
  static std::map<uint32_t, std::string> m_crc32map_executions;
};

} // MH2FSB
