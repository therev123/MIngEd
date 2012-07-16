#!/bin/bash

set -e

tools/linux/premake4 gmake
make
build/MTool scripts/MPackager.lua
