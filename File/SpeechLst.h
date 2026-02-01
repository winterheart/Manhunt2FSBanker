/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */
#pragma once

#include <string>
#include <vector>
#include "FileContainer.h"

namespace MH2FSB {

class SpeechLst : public FileContainer {
  public:
    std::vector<std::string> GetEntries() { return m_entries; }

    friend std::ostream& operator<<(std::ostream& out, const SpeechLst& c);
    friend std::istream& operator>>(std::istream& in, SpeechLst& c);

  private:
    std::vector<std::string> m_entries;
};

}
