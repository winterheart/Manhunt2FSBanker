/* SPDX-FileCopyrightText: Copyright 2026 Azamat H. Hackimov <azamat.hackimov@gmail.com> */
/* SPDX-License-Identifier: LGPL-2.1-or-later */

#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <utility>
#include <vector>

#include "Dir.h"
#include "FileContainer.h"
#include "YAMLHelpers.h"

#pragma pack(2)

namespace MH2FSB {

// Forward declaration
class Fsb;
class FsbHeader;

/*
    Defines for FSOUND_FSB_HEADER->mode field
*/
enum FsbHeaderMode {
  FSOUND_FSB_SOURCE_FORMAT = 0x00000001,       /* all samples stored in their original compressed format */
  FSOUND_FSB_SOURCE_BASICHEADERS = 0x00000002, /* samples should use the basic header structure */
};

/*
    Defines for FSOUND_FSB_HEADER->version field
*/
enum FsbHeaderVersion {
  FSOUND_FSB_VERSION_3_1 = 0x00030001, /* FSB version 3.1 */
  FSOUND_FSB_VERSION_4_0 = 0x00040000, /* FSB version 4.0 */
};

enum FsbMode {
  FSOUND_LOOP_OFF = 0x00000001,    /* For non looping samples. */
  FSOUND_LOOP_NORMAL = 0x00000002, /* For forward looping samples. */
  FSOUND_LOOP_BIDI = 0x00000004,   /* For bidirectional looping samples. (no effect if in hardware). */
  FSOUND_8BITS = 0x00000008,       /* For 8 bit samples. */
  FSOUND_16BITS = 0x00000010,      /* For 16 bit samples. */
  FSOUND_MONO = 0x00000020,        /* For mono samples. */
  FSOUND_STEREO = 0x00000040,      /* For stereo samples. */
  FSOUND_UNSIGNED = 0x00000080,    /* For user created source data containing unsigned samples. */
  FSOUND_SIGNED = 0x00000100,      /* For user created source data containing signed data. */
  FSOUND_DELTA = 0x00000200,       /* For user created source data stored as delta values. */
  FSOUND_IT214 = 0x00000400,       /* For user created source data stored using IT214 compression. */
  FSOUND_IT215 = 0x00000800,       /* For user created source data stored using IT215 compression. */
  FSOUND_HW3D = 0x00001000,       /* Attempts to make samples use 3d hardware acceleration. (if the card supports it) */
  FSOUND_2D = 0x00002000,         /* Tells software (not hardware) based sample not to be included in 3d processing. */
  FSOUND_STREAMABLE = 0x00004000, /* For a streaming sound where you feed the data to it. */
  FSOUND_LOADMEMORY = 0x00008000, /* "name" will be interpreted as a pointer to data for streaming and samples. */
  FSOUND_LOADRAW = 0x00010000,    /* Will ignore file format and treat as raw pcm. */
  FSOUND_MPEGACCURATE =
      0x00020000, /* For FSOUND_Stream_Open - for accurate FSOUND_Stream_GetLengthMs/FSOUND_Stream_SetTime. */
  FSOUND_FORCEMONO = 0x00040000, /* For forcing stereo streams and samples to be mono - needed if using FSOUND_HW3D and
                                    stereo data - incurs a small speed hit for streams */
  FSOUND_HW2D = 0x00080000,      /* 2D hardware sounds. allows hardware specific effects */
  FSOUND_ENABLEFX =
      0x00100000, /* Allows DX8 FX to be played back on a sound. Requires DirectX 8 - Note these sounds cannot be played
                     more than once, be 8 bit, be less than a certain size, or have a changing frequency */
  FSOUND_MPEGHALFRATE =
      0x00200000, /* For FMODCE only - decodes mpeg streams using a lower quality decode, but faster execution */
  FSOUND_IMAADPCM = 0x00400000, /* Contents are stored compressed as IMA ADPCM */
  FSOUND_VAG = 0x00800000,      /* For PS2 only - Contents are compressed as Sony VAG format */
  FSOUND_XMA = 0x01000000,
  FSOUND_GCADPCM = 0x02000000,      /* For Gamecube only - Contents are compressed as Gamecube DSP-ADPCM format */
  FSOUND_MULTICHANNEL = 0x04000000, /* For PS2 and Gamecube only - Contents are interleaved into a multi-channel (more
                                       than stereo) format */
  FSOUND_USECORE0 = 0x08000000,     /* For PS2 only - Sample/Stream is forced to use hardware voices 00-23 */
  FSOUND_USECORE1 = 0x10000000,     /* For PS2 only - Sample/Stream is forced to use hardware voices 24-47 */
  SOUND_LOADMEMORYIOP = 0x20000000, /* For PS2 only - "name" will be interpreted as a pointer to data for streaming and
                                       samples. The address provided will be an IOP address */
  FSOUND_IGNORETAGS = 0x40000000, /* Skips id3v2 etc tag checks when opening a stream, to reduce seek/read overhead when
                                     opening files (helps with CD performance) */
  FSOUND_STREAM_NET = 0x80000000, /* Specifies an internet stream */
  FSOUND_NORMAL = FSOUND_16BITS | FSOUND_SIGNED | FSOUND_MONO
};

enum FsbSoundFormat {
  FMOD_SOUND_FORMAT_NONE,            /* Initialized / unknown. */
  FMOD_SOUND_FORMAT_PCM8,            /* 8bit integer PCM data. */
  FMOD_SOUND_FORMAT_PCM16,           /* 16bit integer PCM data. */
  FMOD_SOUND_FORMAT_PCM24,           /* 24bit integer PCM data. */
  FMOD_SOUND_FORMAT_PCM32,           /* 32bit integer PCM data. */
  FMOD_SOUND_FORMAT_PCMFLOAT,        /* 32bit floating point PCM data. */
  FMOD_SOUND_FORMAT_GCADPCM,         /* Compressed Nintendo 3DS/Wii DSP data. */
  FMOD_SOUND_FORMAT_IMAADPCM,        /* Compressed IMA ADPCM data. */
  FMOD_SOUND_FORMAT_VAG,             /* Compressed PlayStation Portable ADPCM data. */
  FMOD_SOUND_FORMAT_HEVAG,           /* Compressed PSVita ADPCM data. */
  FMOD_SOUND_FORMAT_XMA,             /* Compressed Xbox360 XMA data. */
  FMOD_SOUND_FORMAT_MPEG,            /* Compressed MPEG layer 2 or 3 data. */
  FMOD_SOUND_FORMAT_CELT,            /* Compressed CELT data. */
  FMOD_SOUND_FORMAT_AT9,             /* Compressed PSVita ATRAC9 data. */
  FMOD_SOUND_FORMAT_XWMA,            /* Compressed Xbox360 xWMA data. */
  FMOD_SOUND_FORMAT_VORBIS,          /* Compressed Vorbis data. */
  FMOD_SOUND_FORMAT_MAX,             /* Maximum number of sound formats supported. */
  FMOD_SOUND_FORMAT_FORCEINT = 65536 /* Makes sure this enum is signed 32bit. */
};

class FsbHeader {
  std::string m_id;         /* 'FSB3' or 'FSB4', 4 bytes length */
  uint32_t m_numsamples = 0; /* number of samples in the file */
  uint32_t m_shdrsize = 0;   /* size in bytes of all the sample headers including extended information */
  uint32_t m_datasize = 0;   /* size in bytes of compressed sample data */
  uint32_t m_version = 0;   /* extended fsb version */
  uint32_t m_mode = 0;      /* flags that apply to all samples in the fsb */
  /* FSB4 additional data */
  std::array<uint8_t, 8> m_zeros = {}; /* ? */
  std::array<uint8_t, 16> m_hash = {}; /* hash? */

  friend std::ostream &operator<<(std::ostream &out, Fsb &c);
  friend std::istream &operator>>(std::istream &in, Fsb &c);
  friend std::ostream &operator<<(std::ostream &out, FsbHeader &c);
  friend std::istream &operator>>(std::istream &in, FsbHeader &c);

  friend struct YAML::convert<FsbHeader>;

  friend class Fsb;
};

class FsbSampleHeader {
public:
  FsbSampleHeader() {
    m_headerversion = FSOUND_FSB_VERSION_3_1;
    m_is_basicheader = false;
  }
  FsbSampleHeader(uint32_t headerversion, bool is_basicheader)
      : m_headerversion(headerversion), m_is_basicheader(is_basicheader) {}

  void SetRealName(const std::string &realname) { m_realname = realname; }

  [[nodiscard]] std::filesystem::path GetRealName() const { return m_realname; }

  [[nodiscard]] uint32_t GetOffset() const { return m_offset; }

  [[nodiscard]] uint32_t GetSize() const { return m_lengthcompressedbytes; }

  [[nodiscard]] uint32_t GetMode() const { return m_mode; }

  [[nodiscard]] uint32_t GetChannels() const { return m_numchannels; }

  [[nodiscard]] uint32_t GetFrequency() const { return m_deffreq; }

private:
  /// Generated attributes

  /// Version of FSB file
  uint32_t m_headerversion;
  /// Is basic header? (use only m_lengthsamples and m_lengthcompressedbytes)
  bool m_is_basicheader;
  /// Real name of sample
  std::filesystem::path m_realname;
  /// Offset to begin of sample
  uint32_t m_offset = 0;

  // Size of sample header (version 3.1)
  uint16_t m_size = 80;
  std::string m_name;

  uint32_t m_lengthsamples = 0;
  uint32_t m_lengthcompressedbytes = 0;
  uint32_t m_loopstart = 0;
  uint32_t m_loopend = 0;

  uint32_t m_mode = 0;
  int32_t m_deffreq = 0;
  uint16_t m_defvol = 0;
  int16_t m_defpan = 0;
  uint16_t m_defpri = 0;
  uint16_t m_numchannels = 0;

  float m_mindistance = 0.0f;
  float m_maxdistance = 0.0f;
  int32_t m_varfreq = 0;
  uint16_t m_varvol = 0;
  int16_t m_varpan = 0;

  friend std::ostream &operator<<(std::ostream &out, Fsb &c);
  friend std::istream &operator>>(std::istream &in, Fsb &c);
  friend std::ostream &operator<<(std::ostream &out, FsbSampleHeader &c);
  friend std::istream &operator>>(std::istream &in, FsbSampleHeader &c);

  friend struct YAML::convert<FsbSampleHeader>;
  friend class Fsb;
};

/// FSB file class
/// Based on fsbext 0.3.8 (https://aluigi.altervista.org/search.php?src=fsbext)
/// Only FSB3 and FSB4 are implemented, since Manhunt 2 only uses these versions.
class Fsb : public FileContainer {
public:
  Fsb() = default;
  explicit Fsb(const std::filesystem::path &input_path);

  FsbHeader GetHeader() { return m_header; };
  std::vector<FsbSampleHeader> GetSamples() { return m_samples; }
  void ResolveRealNames(const Dir &dir);
private:
  FsbHeader m_header;
  std::vector<FsbSampleHeader> m_samples;

  static uint32_t GetWavSampleRawSize(const std::filesystem::path &real_name);

  friend std::ostream &operator<<(std::ostream &out, Fsb &c);
  friend std::istream &operator>>(std::istream &in, Fsb &c);
  friend struct YAML::convert<Fsb>;
};

} // namespace MH2FSB

#pragma pack()
