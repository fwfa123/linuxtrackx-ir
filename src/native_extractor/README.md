# Native TrackIR installer extraction

This directory builds a small Linux utility that extracts verified firmware
from supported NaturalPoint TrackIR installers without executing the installer
or using Wine.

Build it independently of the Qt application:

```sh
make -C src/native_extractor
```

Extract the complete `tir_firmware` set:

```sh
src/native_extractor/ltr_installer_extractor \
  --installer TrackIR_5.5.3.exe \
  --destination firmware
```

The installer must match a known version descriptor exactly. Unsupported or
modified installers are rejected by SHA-256. Existing output files are never
overwritten. The complete output contains `gamedata.txt`, `poem1.txt`,
`poem2.txt`, `sn4.fw.gz`, `tir4.fw.gz`, `tir5.fw.gz`, `tir5v2.fw.gz`, and
`TIRViews.dll`.

The CAB/LZX decoder is the CAB decompression subset of libmspack 0.11alpha,
upstream commit `55d501976171397ccd5d5a7a1ca7da065b1d9a06`. It is distributed
under LGPL-2.1; see `third_party/libmspack/COPYING.LIB`. No libmspack source was
modified. The extractor links that subset as separate source modules and uses
only its public `mspack.h` interface.
