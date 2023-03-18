#!/bin/bash

set -e

SOURCE_DIR=$(pwd | tr -d '\0')

if [ ! -d "${SOURCE_DIR}/bin" ]; then
	echo "Cannot found test file, please run build.sh."
	exit 0
fi

cd "${SOURCE_DIR}/build" && make test ARGS="-VV --output-on-failure"
