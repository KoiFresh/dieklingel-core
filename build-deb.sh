#!/bin/bash

ROOT_DIRECTORY=$(pwd)
BUILD_DIRECTORY=$ROOT_DIRECTORY/.deb-build
BUILD_ARCHITECTURE=arm64

###############################################################################
# Initialize deb package build directory                                      #
###############################################################################

echo "[ dieKlingel (1/5) ]: Initialze the deb package build directory."

rm -rf $BUILD_DIRECTORY
mkdir -p $BUILD_DIRECTORY/$BUILD_ARCHITECTURE/usr/share/dieklingel
mkdir -p $BUILD_DIRECTORY/$BUILD_ARCHITECTURE/usr/share/dieklingel/core
mkdir -p $BUILD_DIRECTORY/$BUILD_ARCHITECTURE/usr/share/dieklingel/exec

cp -r deb/$BUILD_ARCHITECTURE/* $BUILD_DIRECTORY/$BUILD_ARCHITECTURE

###############################################################################
# Build the core package and copy into deb package build folder.              #
###############################################################################

echo "[ dieKlingel (2/5) ]: Build the core package and copy into deb package build folder."

cd $ROOT_DIRECTORY

flutter build linux
cp -r build/linux/$BUILD_ARCHITECTURE/release/bundle/* $BUILD_DIRECTORY/$BUILD_ARCHITECTURE/usr/share/dieklingel/core

cd $ROOT_DIRECTORY

###############################################################################
# Build the core package and copy into deb package build folder.              #
###############################################################################

echo "[ dieKlingel (3/5) ]: Build the core package and copy into deb package build folder."

cd $ROOT_DIRECTORY/apps/exec

dart compile exe bin/exec.dart -o /tmp/exec
cp /tmp/exec $BUILD_DIRECTORY/$BUILD_ARCHITECTURE/usr/share/dieklingel/exec
rm /tmp/exec

cd $ROOT_DIRECTORY

###############################################################################
# Build the .deb package from $BUILD_DIRECTORY/$BUILD_ARCHITECTURE (e.g       #
# .deb-build/arm64) folder.                                                   #
###############################################################################

echo "[ dieKlingel (4/5) ]: Build the .deb package from $BUILD_DIRECTORY/$BUILD_ARCHITECTURE (e.g .deb-build/arm64) folder."

DEB_PACKAGE_VERSION=$(cat $BUILD_DIRECTORY/$BUILD_ARCHITECTURE/DEBIAN/control | grep -Po '(?<=Version:\s)(.*)') 

dpkg-deb --build $BUILD_DIRECTORY/$BUILD_ARCHITECTURE dieklingel_${DEB_PACKAGE_VERSION}_${BUILD_ARCHITECTURE}.deb

###############################################################################
# Cleanup deb package build directory.                                        #
###############################################################################

echo "[ dieKlingel (5/5) ]: Cleanup deb package build directory."

rm -rf $BUILD_DIRECTORY