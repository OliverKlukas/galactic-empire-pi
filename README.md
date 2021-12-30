# galactic-empire-pi
Reverse engineered implementation of Galactic Empire for C64. Implemented in C and cross-compiled via [CC65](https://cc65.github.io/) into 6502 Assembly compatible .c64 executables. Engineered to natively run on a VICE powered raspberry pi build into a rebuild of the original [Commodore 64](https://en.wikipedia.org/wiki/Commodore_64). Game idea credits belong to the [original](http://www.gamebase64.com/game.php?id=9564) C64 Galactic Empire version.

# product at a glance


# technical documentation
## project structure
```bash
.
├── GalacticEmpire
│   ├── GalacticEmpire.c64              # C64 executable
│   ├── Makefile
│   └── src                                     
│       ├── graphics                    
│       │   ├── box.c                     # player input box
│       │   ├── box.h     
│       │   ├── graphics.c                # global game
│       │   ├── graphics.h
│       │   ├── map.c                     # game map
│       │   ├── map.h
│       │   ├── questions.c               # initial setup question
│       │   ├── questions.h       
│       │   ├── ranking.c                 # award ceremony
│       │   ├── ranking.h
│       │   ├── startscreen.c             # start screen
│       │   ├── startscreen.h
│       │   ├── table.c                   # game table
│       │   └── table.h
│       ├── main.c                      # main game logic
│       ├── main.h
│       └── utils
│           ├── array.c                   # special array operations
│           ├── array.h
│           ├── queue.c                   # mission data structure 
│           └── queue.h
└── README.md
```

## requirements
- [VICE emulator (^3.5)](https://vice-emu.sourceforge.io/)
- [CC65 (V2.19)](https://www.cc65.org/)

## compile instructions
```bash
# Compile as .c64 file
cd GalacticEmpire
make
```

## run instructions
```bash
# Depending on vice installation, choose emulator run commands like:
vice-jz.x64 GalacticEmpire.c64
x64dtv GalacticEmpire.c64
```


## useful links
- [CC65 main page](https://cc65.github.io/)
- [CC65 all header files references](https://www.cc65.org/doc/funcref.html#toc2)
- [Original C64 Galactic Empire](http://www.gamebase64.com/game.php?id=9564)

# game documentation
- Supports 1-5 individual players
- 10-99 years of game time
- 10-40 distinct worlds in the galaxy
- utilize the enter key to navigate