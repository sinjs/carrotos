#!/usr/bin/env bash
set -e

COMPILER="i686-elf-gcc"
ASSEMBLER="i686-elf-as"
OUTDIR="build/"
ISODIR="build/iso/"
PROJECT="carrot"

if [ ! -d "$OUTDIR" ]; then
  mkdir -p ./build
fi

if [ "$1" == "build" ]; then
  # Assemble the boot file
  $ASSEMBLER boot.s -o "$OUTDIR/boot.o"

  # Compile the kernel
  $COMPILER -c kernel/kernel.c -o "$OUTDIR/kernel.o" -std=gnu99 -ffreestanding -O2 -Wall -Wextra

  # Link the kernel
  $COMPILER -T linker.ld -o "$OUTDIR/$PROJECT.bin" -ffreestanding -O2 -nostdlib $OUTDIR/boot.o $OUTDIR/kernel.o -lgcc

  exit 0
fi

if [ "$1" == "iso" ]; then
  # Check multiboot
  if ! grub-file --is-x86-multiboot $OUTDIR/$PROJECT.bin; then
    echo "The file is not multiboot. Exiting"
    exit 1
  fi;

  if [ -d $ISODIR ]; then
      rm $ISODIR -rv
  fi

  mkdir -p $ISODIR/boot/grub
  cp -v $OUTDIR/$PROJECT.bin $ISODIR/boot/$PROJECT.bin
  cp -v grub.cfg $ISODIR/boot/grub/grub.cfg

  grub-mkrescue -o $OUTDIR/$PROJECT.iso $ISODIR

  exit 0
fi

if [ "$1" == "run" ]; then
  qemu-system-i386 -cdrom $OUTDIR/$PROJECT.iso

  exit 0
fi

echo "No/Invalid options specified."
echo "usage: $0 {build,run,iso}"

exit 1