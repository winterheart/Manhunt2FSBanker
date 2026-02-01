/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */
#pragma once

#include <fstream>

namespace MH2FSB {

/// Abstract class for (de)serialization.
class FileContainer {
  public:
    virtual ~FileContainer() = default;
    virtual void Serialize(std::istream &output);
    virtual void Deserialize(const std::ostream &input);
};

} // MH2FSB
