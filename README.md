# pawns
## Quick Start

Build with the default port (on Linux this uses the x11 port):

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

Build with the SDL2 port (requires SDL2 dev packages):

```bash
cmake -S . -B build-sdl2 -DPDC_PORT=sdl2 -DCMAKE_BUILD_TYPE=Release
cmake --build build-sdl2 -j
```

Use system-installed curses instead of fetching PDCurses:

```bash
cmake -S . -B build-system -DPDC_USE_SYSTEM=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build-system -j
```

Build for DOS (Requires Docker):

```bash
./scripts/build-dos.sh
```
