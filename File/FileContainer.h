/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */
#pragma once

#include <fstream>

namespace MH2FSB {

/// Abstract class for (de)serialization.
class FileContainer {
  public:
    virtual ~FileContainer() = default;
};

} // MH2FSB
