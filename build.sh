# !/bin/bash

set -e

SOURCE_DIR=`pwd`

# 创建build目录
if [ ! -d `pwd`/build ]; then
	mkdir `pwd`/build
fi

# 创建lib目录
if [ ! -d `pwd`/lib ]; then
	mkdir `pwd`/lib
fi

# 创建include目录
if [ ! -d `pwd`/include ]; then
	mkdir `pwd`/include
fi

rm -rf ${SOURCE_DIR}/build/*

cd ${SOURCE_DIR}/build && cmake .. && make install

