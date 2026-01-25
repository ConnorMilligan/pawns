#!/usr/bin/env bash
set -euo pipefail

# Build the PDCurses DOS port inside a Docker container and copy artifacts to ./build_dos
# Requires Docker to be installed and the current directory to be the repo root.

IMAGE="arlaneenalra/watcom-docker:latest"
PDC_VERSION="3.9"
OUTDIR="$(pwd)/build_dos"
SRCDIR="$(pwd)/src"
PROJECT="pawns"
mkdir -p "$OUTDIR"

# Download PDCurses

if [ -f "${OUTDIR}/pdcurses/curses.h" ]; then
  echo "PDCurses already built, skipping download and build."
else
  echo "Downloading PDCurses ${PDC_VERSION}..."
  curl -sSL https://github.com/wmcbrine/PDCurses/archive/refs/tags/${PDC_VERSION}.tar.gz -o ${OUTDIR}/pdcurses.tar.gz
  echo "Extracting PDCurses..."
  mkdir -p ${OUTDIR}/pdcurses
  tar -xzf ${OUTDIR}/pdcurses.tar.gz -C ${OUTDIR}/pdcurses --strip-components=1
  rm ${OUTDIR}/pdcurses.tar.gz
  # Patch common/watcom.mif to use .o instead of .obj and rm instead of del
  sed -i 's/\.obj/.o/g' ${OUTDIR}/pdcurses/common/watcom.mif
  sed -i 's/RM\t= del/RM\t= rm/g' ${OUTDIR}/pdcurses/common/watcom.mif
  # Remove -oneatx from CFLAGS in watcom.mif
  sed -i 's/-oneatx//g' ${OUTDIR}/pdcurses/common/watcom.mif
fi


# Download the PDCurses release inside the container and build the DOS port there.
# This avoids assuming the repo contains a local 'dos/' directory.
echo "Building PDCurses DOS port using Docker..."
docker run --rm -it --volume=${OUTDIR}/pdcurses:/src/pdcurses --volume=${OUTDIR}:/src --volume=${SRCDIR}:/source --env PDCURSES_SRCDIR=.. $IMAGE \
  sh -c "cd /src/pdcurses/dos && wmake -f Makefile.wcc"

# Now build program itself inside the container, linking against the built PDCurses library.
echo "Building $PROJECT DOS executable using Docker..."
docker run --rm -it --volume=${OUTDIR}/pdcurses:/src/pdcurses --volume=${OUTDIR}:/src --volume=${SRCDIR}:/source --env PDCURSES_SRCDIR=.. $IMAGE \
  sh -c "wcl -bt=dos -ml -I/src/pdcurses -fe=/src/$PROJECT.exe \$(find /source -name '*.c') /src/pdcurses/dos/pdcurses.lib" 

echo "DOS build complete — artifacts in: $OUTDIR"