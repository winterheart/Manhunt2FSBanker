/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */

#pragma once

#include <yaml-cpp/yaml.h>

namespace MH2FSB {

// Forward declaration
class Fsb;
class FsbHeader;
class FsbSampleHeader;

}


namespace YAML {

template <>
struct convert<MH2FSB::Fsb> {
  /// Encode Fsb class to YAML
  static Node encode(const MH2FSB::Fsb &rhs);

  /// Decode YAML to Fsb class
  static bool decode(const Node& node, MH2FSB::Fsb& rhs);
};

template <>
struct convert<MH2FSB::FsbHeader> {
  /// Encode FsbHeader class to YAML
  static Node encode(const MH2FSB::FsbHeader &rhs);

  /// Decode YAML to FsbHeader class
  static bool decode(const Node& node, MH2FSB::FsbHeader& rhs);
};

template <>
struct convert<MH2FSB::FsbSampleHeader> {
  /// Encode FsbSampleHeader class to YAML
  static Node encode(const MH2FSB::FsbSampleHeader &rhs);

  /// Decode YAML to FsbSampleHeader class
  static bool decode(const Node& node, MH2FSB::FsbSampleHeader& rhs);
};

}  // namespace YAML
