# galactic-empire-pi
<b>Reverse engineered implementation of Galactic Empire for C64 that natively runs on a VICE powered raspberry pi.</b>

# instructions
## compile
```bash
# Compile as .c64 file
cd GalacticEmpire
make
```

## run
```bash
# Option 1: execute on VICE c64 real time simulation
x64sc GalacticEmpire.prg

# Option 2: execute on VICE c64 warp simulation
x64 GalacticEmpire.prg
```

# c64 specifications
- 320x200 pixels resolution
- available TGI colors:
  1. TGI_COLOR_BLACK
  2. TGI_COLOR_WHITE
  3. TGI_COLOR_RED
  4. TGI_COLOR_BLUE
  5. TGI_COLOR_GREEN
  6. TGI_COLOR_ORANGE


# requirements
- [VICE emulator (^3.5)](https://vice-emu.sourceforge.io/)
- [CC65 (V2.19)](https://www.cc65.org/)

# design
[![figma file](https://user-images.githubusercontent.com/39443615/146673103-c91f6679-2e21-4eb4-a8cf-3425f44cd46c.png)](https://www.figma.com/file/eUCdAuNY5zCJLcmdWapxJb/Galactic-Empire?node-id=0%3A1)

# useful links
- [VICE Debugger](https://codebase64.org/doku.php?id=base:using_the_vice_monitor)
- [CC65 main page](https://cc65.github.io/)
- [CC65 all header files references](https://www.cc65.org/doc/funcref.html#toc2)
- [tgi.h reference](https://www.cc65.org/doc/funcref-40.html) or [tgi.h wiki](https://cc65.github.io/doc/tgi.html)
- [GEOSLib](https://www.cc65.org/doc/geos.html#toc3)
