/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "YAMLHelpers.h"

#include "Fsb.h"

#include <cassert>

namespace YAML {
Node convert<MH2FSB::Fsb>::encode(const MH2FSB::Fsb &rhs) {
  Node ret;

  ret["header"] = rhs.m_header;
  ret["samples"] = rhs.m_samples;

  return ret;
}

bool convert<MH2FSB::Fsb>::decode(const Node &node, MH2FSB::Fsb &rhs) {

  // TODO: Implement
  assert(false);

  return true;
}

Node convert<MH2FSB::FsbHeader>::encode(const MH2FSB::FsbHeader &rhs) {
  Node ret;

  ret["id"] = rhs.m_id;
  ret["version"] = rhs.m_version;
  ret["mode"] = rhs.m_mode;

  if (rhs.m_version == MH2FSB::FSOUND_FSB_VERSION_4_0) {
    ret["zeros"] = rhs.m_zeros;
    ret["hash"] = rhs.m_hash;
  }

  return ret;
}

bool convert<MH2FSB::FsbHeader>::decode(const Node &node, MH2FSB::FsbHeader &rhs) {
  if (!node.IsMap())
    return false;

  if (node["id"].as<std::string>() != "FSB3" && node["id"].as<std::string>() != "FSB4") {
    return false;
  }
  rhs.m_id = node["id"].as<std::string>();
  rhs.m_version = node["version"].as<uint32_t>();
  rhs.m_mode = node["mode"].as<uint32_t>();

  if (rhs.m_version == MH2FSB::FSOUND_FSB_VERSION_4_0) {
    rhs.m_zeros = node["zeros"].as<std::array<uint8_t, 8>>();
    rhs.m_hash = node["hash"].as<std::array<uint8_t, 16>>();
  }

  return true;
}

Node convert<MH2FSB::FsbSampleHeader>::encode(const MH2FSB::FsbSampleHeader &rhs) {
  Node ret;

  ret["real_name"] = rhs.m_realname.string();

  if (!rhs.m_is_basicheader) {
    ret["name"] = rhs.m_name;
    ret["loopstart"] = rhs.m_loopstart;
    ret["loopend"] = rhs.m_loopend;
    ret["mode"] = rhs.m_mode;
    ret["deffreq"] = rhs.m_deffreq;
    ret["defvol"] = rhs.m_defvol;
    ret["defpan"] = rhs.m_defpan;
    ret["defpri"] = rhs.m_defpri;
    ret["numchannels"] = rhs.m_numchannels;
    ret["mindistance"] = rhs.m_mindistance;
    ret["maxdistance"] = rhs.m_maxdistance;
    ret["varfreq"] = rhs.m_varfreq;
    ret["varvol"] = rhs.m_varvol;
    ret["varpan"] = rhs.m_varpan;
  }

  return ret;
}

bool convert<MH2FSB::FsbSampleHeader>::decode(const Node &node, MH2FSB::FsbSampleHeader &rhs) {
  rhs.m_realname = node["real_name"].as<std::string>();

  if (!rhs.m_is_basicheader) {
    rhs.m_name = node["name"].as<std::string>();
    rhs.m_loopstart = node["loopstart"].as<uint32_t>();
    rhs.m_loopend = node["loopend"].as<uint32_t>();
    rhs.m_mode = node["mode"].as<uint32_t>();
    rhs.m_deffreq = node["deffreq"].as<int32_t>();
    rhs.m_defvol = node["defvol"].as<uint16_t>();
    rhs.m_defpan = node["defpan"].as<int16_t>();
    rhs.m_defpri = node["defpri"].as<uint16_t>();
    rhs.m_numchannels = node["numchannels"].as<uint16_t>();
    rhs.m_mindistance = node["mindistance"].as<float>();
    rhs.m_maxdistance = node["maxdistance"].as<float>();
    rhs.m_varfreq = node["varfreq"].as<int16_t>();
    rhs.m_varvol = node["varvol"].as<uint16_t>();
    rhs.m_varpan = node["varpan"].as<int16_t>();
  }

  return true;
}

} // namespace YAML
