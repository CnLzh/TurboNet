#!/bin/bash

set -e

SOURCE_DIR=$(pwd | tr -d '\0')

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

rm -rf "${SOURCE_DIR}/build/*"

cd "${SOURCE_DIR}/build" && cmake .. && make install

