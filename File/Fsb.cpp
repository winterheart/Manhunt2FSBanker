/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include <cassert>
#include <format>
#include <fstream>
#include <iostream>

#include "binio.h"
#include "DirNames.h"
#include "Fsb.h"


namespace MH2FSB {

void Fsb::ResolveRealNames(const Dir &dir) {
  for (size_t i = 0; i < m_samples.size(); i++) {
    std::string name = Crc32ResolveNames::getInstance().getName(dir.GetEntriesId(i));
    if (name.empty()) {
      name = std::format("unknown\\unknown_{:08x}\\pc_stream.wav", dir.GetEntriesId(i));
      std::cout << std::format("{:08x} has no resolved name, set to {}!", dir.GetEntriesId(i), name) << std::endl;
    }
    m_samples.at(i).SetRealName(name);
  }
}

/* FsbHeader IO */

std::ostream& operator<<(std::ostream& out, FsbHeader& c) {
  assert(c.m_version == FSOUND_FSB_VERSION_3_1 || c.m_version == FSOUND_FSB_VERSION_4_0);

  UTILS::bin_write(out, c.m_id, 4);
  UTILS::bin_write(out, c.m_numsamples);
  UTILS::bin_write(out, c.m_shdrsize);
  UTILS::bin_write(out, c.m_datasize);
  UTILS::bin_write(out, c.m_version);
  UTILS::bin_write(out, c.m_mode);

  if (c.m_version == FSOUND_FSB_VERSION_4_0) {
    out.write(reinterpret_cast<char*>(c.m_zeros.data()), c.m_zeros.size());
    out.write(reinterpret_cast<char*>(c.m_hash.data()), c.m_hash.size());
  }

  return out;
};

std::istream& operator>>(std::istream& in, FsbHeader& c) {
  UTILS::bin_read(in, c.m_id, 4);
  UTILS::bin_read(in, c.m_numsamples);
  UTILS::bin_read(in, c.m_shdrsize);
  UTILS::bin_read(in, c.m_datasize);
  UTILS::bin_read(in, c.m_version);
  UTILS::bin_read(in, c.m_mode);

  assert(c.m_version == FSOUND_FSB_VERSION_3_1 || c.m_version == FSOUND_FSB_VERSION_4_0);

  if (c.m_version == FSOUND_FSB_VERSION_4_0) {
    in.read(reinterpret_cast<char*>(c.m_zeros.data()), c.m_zeros.size());
    in.read(reinterpret_cast<char*>(c.m_hash.data()), c.m_hash.size());
  }

  return in;
};

/* FsbSampleHeader IO */

std::ostream& operator<<(std::ostream& out, FsbSampleHeader& c) {
  // TODO: implement
  assert(false);
  return out;
}

std::istream& operator>>(std::istream& in, FsbSampleHeader& c) {
  assert(c.m_headerversion == FSOUND_FSB_VERSION_3_1 || c.m_headerversion == FSOUND_FSB_VERSION_4_0);

  if (!c.m_is_basicheader) {
    UTILS::bin_read(in, c.m_size);
    UTILS::bin_read(in, c.m_name, 30);
  }

  UTILS::bin_read(in, c.m_lengthsamples);
  UTILS::bin_read(in, c.m_lengthcompressedbytes);

  if (!c.m_is_basicheader) {
    UTILS::bin_read(in, c.m_loopstart);
    UTILS::bin_read(in, c.m_loopend);
    UTILS::bin_read(in, c.m_mode);
    UTILS::bin_read(in, c.m_deffreq);
    UTILS::bin_read(in, c.m_defvol);
    UTILS::bin_read(in, c.m_defpan);
    UTILS::bin_read(in, c.m_defpri);
    UTILS::bin_read(in, c.m_numchannels);
    UTILS::bin_read(in, c.m_mindistance);
    UTILS::bin_read(in, c.m_maxdistance);
    UTILS::bin_read(in, c.m_varfreq);
    UTILS::bin_read(in, c.m_varvol);
    UTILS::bin_read(in, c.m_varpan);
  }
  return in;
}

/* Fsb IO */

std::ostream &operator<<(std::ostream &out, Fsb &c) {
  // TODO: implement
  assert(false);
  return out;
}

std::istream &operator>>(std::istream &in, Fsb &c) {
  in >> c.m_header;

  FsbSampleHeader sample_header(c.m_header.m_version, false);
  for (int32_t i = 0; i < c.m_header.m_numsamples; i++) {
    if (c.m_header.m_mode & FSOUND_FSB_SOURCE_BASICHEADERS && i) {
      sample_header.m_is_basicheader = true;
    }
    in >> sample_header;
    c.m_samples.push_back(sample_header);
  }

  return in;
}
} // namespace MH2FSB
