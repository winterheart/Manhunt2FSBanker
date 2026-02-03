/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */
#pragma once

#include <filesystem>
#include <fstream>

inline bool compare_files(const std::filesystem::path &a, const std::filesystem::path &b) {
  std::ifstream fa(a, std::ios::binary | std::ios::ate);
  std::ifstream fb(b, std::ios::binary | std::ios::ate);

  if (fa.fail() || fb.fail()) {
    return false;
  }

  if (fa.tellg() != fb.tellg()) {
    return false;
  }

  fa.seekg(0, std::ios::beg);
  fb.seekg(0, std::ios::beg);

  return std::equal(std::istreambuf_iterator<char>(fa.rdbuf()), std::istreambuf_iterator<char>(),
                    std::istreambuf_iterator<char>(fb.rdbuf()));
}
