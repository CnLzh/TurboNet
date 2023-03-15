# TurboNet
A high-performance network library in C++20 that adopts the one loop per thread approach.

## Introduction
This is a multi-thread network library based on the Reactor pattern and the one loop per thread design, inspired by the `muduo` network library.

Include the following features:
- Network communication function:
Provide encapsulation and implementation of TCP protocol.
- Event-driven framework:
I/O multiplexing and event loop mechanism based on the Reactor pattern.
- Multithreading support:
Utilize thread pool to support thread-safe multithreaded programing.
- Timer mechanism:
Provide timer mechanism to support triggering and handing of timed events.
- Exception handing and logging:
Including asynchronous logging and exception handing mechanisms.

## Development Environment
- OS: `Ubuntu 22.04.1 LTS`
- Language: `C++ 20`
- Coding style: `Google C++`
- Compiler: `g++ 11.3.0`
- Build: `CMake 3.25.3`

## Build Project
The minimum required version of CMake for building the project is 3.24 or above. How to install CMake can be found in the [CMake documentation](https://cmake.org/).

download:
```
git clone git@github.com:CnLzh/TurboNet.git
```

build:
```
cd ./TurboNet && bash build.sh
```