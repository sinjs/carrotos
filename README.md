# CarrotOS

---

### ⚠ WARNING ⚠
#### This is still a "work in progress" and is in no way usable right now!
#### I'm also doing a rewrite in the [`sinjs/rewrite`](https://github.com/sinmineryt/carrotos/tree/sinjs/rewrite) branch

---

CarrotOS is an operating system made to celebrate carrot day 100 of alex

## Building

Docs in progress!

### The dependencies

#### Debian based

```shell
sudo apt install build-essential curl libmpfr-dev libmpc-dev libgmp-dev e2fsprogs qemu-system-i386 qemu-utils ccache
```

**GCC 10**

On Ubuntu gcc-10 is available in the repositories of 20.04 (Focal) and later - add the `ubuntu-toolchain-r/test` PPA if
you're running an older version:

```shell
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
```

On Debian you can use the Debian testing branch:

```shell
sudo echo "deb http://http.us.debian.org/debian/ testing non-free contrib main" >> /etc/apt/sources.list
sudo apt update
```

Now on Ubuntu or Debian you can install gcc-10 with apt like this:

```shell
sudo apt install gcc-10 g++-10
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 900 --slave /usr/bin/g++ g++ /usr/bin/g++-10
```

#### Fedora

```shell
sudo dnf install binutils-devel curl mpfr-devel libmpc-devel gmp-devel e2fsprogs patch ccache @"C Development Tools and Libraries" @Virtualization
```

#### openSUSE

```shell
sudo zypper install curl mpfr-devel mpc-devel gmp-devel e2fsprogs patch qemu-x86 qemu-audio-pa gcc gcc-c++ ccache patterns-devel-C-C++-devel_C_C++
```

#### Arch Linux

```shell
sudo pacman -S --needed base-devel curl mpfr libmpc gmp e2fsprogs qemu qemu-arch-extra ccache
```

### Setting up the cross compiler

This step may take a long time, depending on your computers speed.

```shell
./crosscompiler.sh
```

### Compiling

To compile just run the following command:

```shell
./build.sh
```

### Running and other commands

* To create an ISO file run `./iso.sh`
* To run the QEMU emulator run `./qemu.sh`
* To run a cleanup, execute `./clean.sh`

## License

GNU GPL v3
