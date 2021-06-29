#!/bin/bash
set -e

BINUTILS_VERSION="2.34"
GCC_VERSION="10.3.0"

error_target_not_in_path() {
  echo -e "\nFATAL ERROR: The $PREFIX/bin dir must be in the PATH.\n"
  exit 12
}

error_cc_failed() {
  echo -e "\nFATAL ERROR: The cross compiler does not work. (exit $0)"
  exit 13
}

echo -e "\nThis script will set up the cross compiler."
echo -e "This may take a very long time depending on the speed of your machine, it may even crash or hang.\n"

mkdir -p ~/src

echo -e "Now downloading GNU binutils and gcc"
cd ~/src
wget https://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.gz
wget https://ftp.gnu.org/gnu/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.gz

echo -e "Extracting"
tar -xzf gcc-$GCC_VERSION.tar.gz
tar -xzf binutils-$BINUTILS_VERSION.tar.gz

echo -e "Setting variables..."
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

echo -e "Now compiling binutils, please wait..."
cd ~/src
mkdir build-binutils
cd build-binutils
../binutils-$BINUTILS_VERSION/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install

echo -e "Now compiling GCC, please wait..."
cd ~/src
which -- $TARGET-as || error_target_not_in_path
mkdir build-gcc
cd build-gcc
../gcc-$GCC_VERSION/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc

echo -e "\nTesting the cross compiler..."
cd ~
~/opt/cross/bin/$TARGET-gcc --version || cc_failed $?
echo -e "It works!\n"

export PATH="$HOME/opt/cross/bin:$PATH"
echo -e "Cross compiler installation finished!\nPlease note that when you restart your shell the path will be reset.
You will need to run 'export PATH=\"\$HOME/opt/cross/bin:\$PATH\"' to make the compiler work again\n"
