#!/bin/bash

set -e

SOURCE_DIR=$(pwd | tr -d '\0')

rm -rf "${SOURCE_DIR}/build/"

# 创建build目录
if [ ! -d "${SOURCE_DIR}/build" ]; then
	mkdir "${SOURCE_DIR}/build"
fi

# 创建lib目录
if [ ! -d "${SOURCE_DIR}/lib" ]; then
	mkdir "${SOURCE_DIR}/lib"
fi

# 创建include目录
if [ ! -d "${SOURCE_DIR}/include" ]; then
	mkdir "${SOURCE_DIR}/include"
fi

# 创建bin目录
if [ ! -d "${SOURCE_DIR}/bin" ]; then
	mkdir "${SOURCE_DIR}/bin"
fi

# 默认编译release,若指定编译debug,同时编译单元测试
if [[ $1 == "-dbg" ]]; then
	rm -rf "${SOURCE_DIR}/bin/"
	cd "${SOURCE_DIR}/build" && cmake -DCMAKE_BUILD_TYPE=Debug .. && make install
else
	cd "${SOURCE_DIR}/build" && cmake -DCMAKE_BUILD_TYPE=Release .. && make install
fi
