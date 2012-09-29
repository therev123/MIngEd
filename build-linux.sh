#!/bin/bash

set -e

3rdparty/premake4/linux/premake4 gmake
make
