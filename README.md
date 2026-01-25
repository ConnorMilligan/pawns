# pawns
## Quick Start

Build with the SDL2 port (requires SDL2 dev packages):

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
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
