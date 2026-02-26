/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include <cassert>
#include <format>
#include <fstream>
#include <iostream>

#include "binio.h"
#include "DirNames.h"
#include "Fsb.h"

#include "Wav.h"


namespace MH2FSB {

Fsb::Fsb(const std::filesystem::path &input_path) {

  std::filesystem::path yaml_file = input_path / "fileinfo.yaml";
  YAML::Node node;
  try {
    node = YAML::LoadFile(yaml_file);
  } catch (std::exception &e) {
    std::cerr << std::format("Failed to load YAML-file: {}", e.what()) << std::endl;
    exit(-1);
  }

  m_header = node["fsb"]["header"].as<FsbHeader>();
  m_header.m_numsamples = node["fsb"]["samples"].size();

  if (m_header.m_mode & FSOUND_FSB_SOURCE_BASICHEADERS) {
    // Full first sample (80 bytes) + (n - 1) * short samples (8 bytes each)
    m_header.m_shdrsize = 80 + (m_header.m_numsamples - 1) * 8;
  } else {
    m_header.m_shdrsize = m_header.m_numsamples * 80;
  }

  auto header_size = m_header.m_shdrsize + m_header.m_version == FSOUND_FSB_VERSION_3_1 ? 24 : 48;
  WavHeader wav_header;

  if (m_header.m_mode & FSOUND_FSB_SOURCE_BASICHEADERS) {
    auto it = node["fsb"]["samples"].begin();

    auto sample_header = it.operator*().as<FsbSampleHeader>();
    sample_header.m_lengthcompressedbytes = GetWavSampleRawSize(input_path / sample_header.m_realname);
    // Actual data can be 16-bit aligned, rounding
    // TODO: Only ADPCM accounted
    sample_header.m_lengthsamples = sample_header.m_lengthcompressedbytes / 36 * 64;
    m_header.m_datasize += sample_header.m_lengthcompressedbytes;
    sample_header.m_offset = m_header.m_datasize + header_size;

    m_samples.push_back(sample_header);
    std::advance(it, 1);

    // Prepare sample sequences
    sample_header.m_is_basicheader = true;

    for ( ; it != node["fsb"]["samples"].end(); ++it) {
      sample_header.m_realname = it.operator*()["real_name"].as<std::string>();
      sample_header.m_lengthcompressedbytes = GetWavSampleRawSize(input_path / sample_header.m_realname);
      // TODO: Only ADPCM accounted
      sample_header.m_lengthsamples = sample_header.m_lengthcompressedbytes / 36 * 64;
      m_header.m_datasize += sample_header.m_lengthcompressedbytes;
      sample_header.m_offset = m_header.m_datasize + header_size;
      m_samples.push_back(sample_header);
    }
  } else {
    // TODO: uncovered and untested
    for (auto it : node["fsb"]["samples"]) {
      auto sample_header = it.as<FsbSampleHeader>();
      sample_header.m_lengthcompressedbytes = GetWavSampleRawSize(input_path / sample_header.m_realname);
      // TODO: Only ADPCM accounted
      sample_header.m_lengthsamples = sample_header.m_lengthcompressedbytes / 36 * 64;
      m_header.m_datasize += sample_header.m_lengthcompressedbytes;
      sample_header.m_offset = m_header.m_datasize + header_size;
      m_samples.push_back(sample_header);
    }
  }
}

void Fsb::ResolveScriptedNames(const Dir &dir) {
  for (size_t i = 0; i < m_samples.size(); i++) {
    uint32_t crc32 = dir.GetEntriesId(i);
    std::string name = Crc32ResolveNames::getInstance().getScriptedName(crc32);
    if (name.empty()) {
      // Failed to resolve, mark it as unknown
      name = std::format(R"(scripted\unknown\unknown_{:08x}\pc_stream.wav)", crc32);
      // std::cout << std::format("{:08x} has no resolved name, set to {}!", crc32, name) << std::endl;
    }
    std::ranges::replace(name, '\\', '/');
    // Remove parent path
    std::filesystem::path result = name;
    auto it = result.begin();
    if (it != result.end()) {
      ++it;
    }
    std::filesystem::path out;
    for (; it != result.end(); ++it) {
      out /= *it;
    }
    out = out.parent_path().replace_extension(".wav");
    result = std::format("{:04}_{}", i, out.filename().string());

    m_samples.at(i).SetRealName(out.parent_path() / result);
  }
}

void Fsb::ResolveExecutionsNames(const Dir &dir) {
  for (size_t i = 0; i < m_samples.size(); i++) {
    uint32_t crc32 = dir.GetEntriesId(i);
    std::string name = Crc32ResolveNames::getInstance().getExecutionsName(crc32);
    if (name.empty()) {
      // Failed to resolve, mark it as unknown
      name = std::format(R"(executions\unknown\unknown_{:08x}.wav)", crc32);
      // std::cout << std::format("{:08x} has no resolved name, set to {}!", crc32, name) << std::endl;
    }
    std::ranges::replace(name, '\\', '/');
    // Remove parent path
    std::filesystem::path result = name;
    auto it = result.begin();
    if (it != result.end()) {
      ++it;
    }
    std::filesystem::path out;
    for (; it != result.end(); ++it) {
      out /= *it;
    }
    result = std::format("{:04}_{}", i, out.filename().string());

    m_samples.at(i).SetRealName(out.parent_path() / result);
  }
}

uint32_t Fsb::GetWavSampleRawSize(const std::filesystem::path &real_name) {
  WavHeader wav_header;
  std::fstream wav_stream(real_name, std::ios::in | std::ios::binary);
  if (!wav_stream.is_open()) {
    throw std::runtime_error("Failed to open wav file");
  }
  wav_stream >> wav_header;
  wav_stream.close();

  return wav_header.GetSubchunk2Size();
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
  if (!c.m_is_basicheader) {
    UTILS::bin_write(out, c.m_size);
    UTILS::bin_write(out, c.m_name, 30);
  }

  UTILS::bin_write(out, c.m_lengthsamples);
  UTILS::bin_write(out, c.m_lengthcompressedbytes);

  if (!c.m_is_basicheader) {
    UTILS::bin_write(out, c.m_loopstart);
    UTILS::bin_write(out, c.m_loopend);
    UTILS::bin_write(out, c.m_mode);
    UTILS::bin_write(out, c.m_deffreq);
    UTILS::bin_write(out, c.m_defvol);
    UTILS::bin_write(out, c.m_defpan);
    UTILS::bin_write(out, c.m_defpri);
    UTILS::bin_write(out, c.m_numchannels);
    UTILS::bin_write(out, c.m_mindistance);
    UTILS::bin_write(out, c.m_maxdistance);
    UTILS::bin_write(out, c.m_varfreq);
    UTILS::bin_write(out, c.m_varvol);
    UTILS::bin_write(out, c.m_varpan);
  }

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
  out << c.m_header;
  for (auto sample_header : c.m_samples) {
    out << sample_header;
  }
  return out;
}

std::istream &operator>>(std::istream &in, Fsb &c) {
  in >> c.m_header;
  auto header_size = c.m_header.m_version == FSOUND_FSB_VERSION_3_1 ? 24 : 48;
  size_t offset = c.m_header.m_shdrsize + header_size;

  FsbSampleHeader sample_header(c.m_header.m_version, false);
  for (int32_t i = 0; i < c.m_header.m_numsamples; i++) {
    if (c.m_header.m_mode & FSOUND_FSB_SOURCE_BASICHEADERS && i) {
      sample_header.m_is_basicheader = true;
    }
    in >> sample_header;
    sample_header.m_offset = offset;
    offset += sample_header.m_lengthcompressedbytes;
    c.m_samples.push_back(sample_header);
  }

  return in;
}
} // namespace MH2FSB
