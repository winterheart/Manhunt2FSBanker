/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "YAMLHelpers.h"

#include "Fsb.h"

#include <cassert>

namespace YAML {
Node convert<MH2FSB::Fsb>::encode(const MH2FSB::Fsb &rhs) {
  Node ret;

  // TODO: Implement
  assert(false);

  return ret;
}

bool convert<MH2FSB::Fsb>::decode(const Node &node, MH2FSB::Fsb &rhs) {

  // TODO: Implement
  assert(false);

  return true;
}

Node convert<MH2FSB::FsbHeader>::encode(const MH2FSB::FsbHeader &rhs) {
  Node ret;

  ret["m_id"] = rhs.m_id;
  ret["m_shrsize"] = rhs.m_shdrsize;
  ret["m_version"] = rhs.m_version;
  ret["m_mode"] = rhs.m_mode;

  return ret;
}

bool convert<MH2FSB::FsbHeader>::decode(const Node &node, MH2FSB::FsbHeader &rhs) {
  if (!node.IsMap())
    return false;

  if (node["m_id"].as<std::string>() != "FSB3" || node["m_id"].as<std::string>() != "FSB4") {
    return false;
  }
  rhs.m_id = node["m_id"].as<std::string>();
  rhs.m_shdrsize = node["m_shrsize"].as<uint32_t>();
  rhs.m_version = node["m_version"].as<uint32_t>();
  rhs.m_mode = node["m_mode"].as<uint32_t>();

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
  // TODO: Implement
  assert(false);

  return true;
}

} // namespace YAML
