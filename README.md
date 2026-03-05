# Manhunt2FSBanker - a tool converting FSB files from Rockstar's Manhunt 2 game

## Usage

```shell
# Unpacking
manhunt2fsbanker unpack Scripted.fsb extracted_directory
# Packing
manhunt2fsbanker pack extracted_directory Scripted.fsb
```

For more help, use `manhunt2fsbanker unpack -h` and `manhunt2fsbanker pack -h`.

A WAV-file should be pcm_s16le or adpcm_ima_xbox encoded file in order to be
encoded to internal FSB format.

For convenience, FSB-file can be extracted to pcm_s16le WAV-files (`unpack -r`
option), which can be played by common audio player. On packing
`manhunt2fsbanker` automatically converts them back.

## Compilation

Project requires any modern C++20 compiler (gcc 10, clang 10, msvc 2019),
cmake and [yaml-cpp library](https://github.com/jbeder/yaml-cpp) as
dependency.

```shell
cmake -B build
cmake --build build
```

## License

Project licensed under LGPL-2.1 or later license. See LICENSE file for more info.

ManhuntRIBber

Copyright (C) 2026 Azamat H. Hackimov

This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation; either version 2.1 of the License, or (at your option)
any later version.

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
details.

You should have received a copy of the GNU Lesser General Public License along
with this library; if not, write to the Free Software Foundation, Inc., 51
Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

----

Project uses CLI11 library under a 3-Clause BSD license. See CLI11.hpp for
additional info.

----
