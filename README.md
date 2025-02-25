# STCFG

## Introduction

This is the official implementation of my master degree paper <name undecided>. The paper introduces a tool which hints
novice reverse engineers about possible hot areas (ie, code blocks which are significantly executed more, or memories
which are referenced to by multiple code blocks) in a given binary. By performing static analysis on the binary's
control flow graph (CFG), this tool can heuristically deduce the possibility and frequency of a certain code block to be
executed. Besides, the pre-mentioned analysis can also reveal the importance of a certain memory according to the number
of different code blocks referring to it.

## Usage

### 1. Compilation

This tool is natively implemented on Ubuntu 24.04 in C++17, with CMake as its builder. Additionally, it relies on 2
external libraries, GTIRB and Capstone, which are not provided by commonly-used package managers. You would need a C++
compiler with C++17 compatibility and CMake no earlier than 3.28 to compile this tool. Ideally a Ubuntu 24.04
installation should satisfy these requirements.

#### 1.1. Install GTIRB

GTIRB allows this tool to analyze the binary at the intermediate representation level. You should be able to access
GTIRB's source code at https://github.com/grammatech/gtirb. The latest version of GTIRB till now is 2.2.0. Simply
checkout the latest source tree in branch master is working well at the moment. If you could see any later version, it
should be working as well.

Once you have the source code, you should be able to compile it through these lines:

```shell
# Assume you are at the root of GTIRB's source tree
cd .. # Back to the parent dir
mkdir gtirb-build
cd gtirb-build
sudo apt install libboost-dev libprotobuf-dev protobuf-compiler # These libraries are relied by GTIRB
cmake -DCMAKE_BUILD_TYPE=Release ../gtirb # Somehow the default build does not perform well
cmake --build .
sudo cmake --install .
```

#### 1.2. Install Capstone

Capstone provides various of disassemblers for different architectures. This tool uses them to understand instructions
in a binary. Capstone's source tree is available at https://github.com/capstone-engine/capstone. The latest
version till now is 5.0.5. Checkout tag 5.0.5 after cloning the source tree should be working well. Again, if you could
see any later version, it should be working as well.

Once you have the source code, you should be able to compile it through these lines:

```shell
# Assume you are at the root of Capstone's source tree
cd ..
mkdir capstone-build
cd capstone-build
cmake -DCMAKE_BUILD_TYPE=Release ../capstone # I am not sure if the default build is well-performing
cmake --build .
sudo cmake --install .
```

Besides, note that Capstone is providing a CMake building script along with its source tree. You should be able to build
using that, but I have not tested it yet.

#### 1.3. Compile STCFG

**CAUTIOUS: If your package manager somehow provides GTIRB and/or Capstone, you might still need an installation through
CMake.** Existing version of Capstone on Ubuntu 24.04 does not provide the config file for CMake to locate the package,
thus cause the build to fail.

After you have installed GTIRB and Capstone, you should be able to compile this tool. You might need to manually specify
the installation dir of GTIRB and Capstone when invoking CMake, if your installation is at a different location other
than system path.

```shell
# Assume you are at the root of STCFG's source tree
cd ..
mkdir stcfg-build
cd stcfg-build
cmake ../STCFG # You are free to specify the build type between Debug (Og) and Release (O3),
               # the default build enables O1
cmake --build .
# You should be able to see the resulting binary at the root of build dir
```

### 2. Preparation

This tool uses GTIRB to generally analyze the binary, thus you need to transform the binary into format GTIRB before you
invoke the tool. You can use another tool ddisasm at https://github.com/grammatech/ddisasm, provided by GrammaTech, to
do the transformation. That tool does provide a docker container for you to conveniently run it. You can read its
README.md for more instructions.

### 3. Invocation

If you are not sure about what to do, simply invoke the tool without any argument. There should be a help message:

```shell
./stcfg
```
