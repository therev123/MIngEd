#!/bin/bash

ENV_DIR=$(dirname $0)

# Try and guess where we want this installed
# NOTE: this script would need write access to wherever it is
if [ $# -ge 1 ] ; then
    INSTALL_DIR=$1
else
    INSTALL_DIR=$HOME
fi

# Try and guess where we're wanting to stick bash profile
# information
if [ $# -ge 2 ] ; then
    PROFILE=$2
else
    PROFILE=~/.bashrc
fi

# Copy the wrapper scripts to the install directory
cp -R $ENV_DIR/bin $INSTALL_DIR/

# Set up the environment
echo 'export MSDKDIR=$HOME/bin/.MSDK/' >> $PROFILE
echo 'export PATH=$HOME/bin:$PATH:$MSDKDIR/Tools' >> $PROFILE

# Load the new environment variables
source $PROFILE

# Link the prepared output directory to somewhere our
# development environment can easily find
ln -sf $ENV_DIR/../../pkg/Maratis $INSTALL_DIR/MIngEd

# set up the internal symlink
$INSTALL_DIR/bin/MSDK minged
