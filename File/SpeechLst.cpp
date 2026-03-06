/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include <filesystem>
#include <fstream>

#include "SpeechLst.h"

namespace MH2FSB {

std::map<std::filesystem::path, std::vector<std::filesystem::path>> SpeechLstQuirks::m_quirks = //
    {
        {
            "A01_Escape_Asylum", // Add INMATE_1_1
            {
                "GENERIC",
                "PLAYER",
                "DIRECTOR_LEO",
                "ASYLUM_1_1",
                "ASYLUM_2_1",
                "ASYLUM_3_1",
                "INMATE_1_1",
                "INMATE_2_1",
                "LEGION_6_1",
                "LEGION_1_1",
                "LEGION_4_1",
                "LEO_HUNTER_1_1",
                "GASMAN_1",
                "DR_PICKMAN_1",
                "MALE_VICTIM_1",
            }},
        {"A04_SM_Nightclub", // Remove JUDY_1
         {
             "GENERIC",
             "PLAYER",
             "DIRECTOR_LEO",
             // "JUDY_1", // Exclude
             "GIMP_2_1",
             "FREAK_4_1",
             "FREAK_1_1",
             "FREAK_2_1",
             "GIMP_1_1",
             "DUNVIC_1_1",
         }},
        {"A06_CIA_Trap",  // Remove JOHN_2_1
         {
             "GENERIC",
             "PLAYER",
             "DIRECTOR_LEO",
             "BLOODHOUND_2_1",
             "BLOODHOUND_4_1",
             "BLOODHOUND_5_1",
             "ASYLUM_1_1",
             "LEO_HUNTER_1_1",
             "ASYLUM_3_1",
             // "JOHN_2_1",  // Exclude
             "WATCHDOG_4_1",
             "WATCHDOG_5_1",
             "WATCHDOG_1_1",
             "WATCHDOG_2_1",
         }},
        {"A10_Brothel", // Add WATCHDOG_3_1
         {
             "GENERIC",
             "PLAYER",
             "DIRECTOR_LEO",
             "JUDY_1",
             "DANCER_2_1",
             "RECEPTIONIST_1",
             "SLEEPER_2_1",
             "DJ_1_1",
             "JOHN_2_1",
             "WATCHDOG_1_1",
             "WATCHDOG_3_1", // Cut from release
             "WATCHDOG_4_1",
         }}};

std::vector<std::filesystem::path> SpeechLstQuirks::Resolve(const std::filesystem::path &level_name) {
  if (m_quirks.contains(level_name)) {
    return m_quirks[level_name];
  }
  return {};
}

std::ostream &operator<<(std::ostream &out, const SpeechLst &c) {
  for (auto const &entry : c.m_entries) {
    out << entry << "\r\n";
  }
  return out;
}

std::istream &operator>>(std::istream &in, SpeechLst &c) {
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

} // namespace MH2FSB
