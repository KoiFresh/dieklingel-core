#!/bin/bash

ROOT_DIRECTORY=$(pwd)

flutter build linux

BUILD_DIRECTORY=.deb-build
BUILD_ARCHITECTURE=arm64

rm -rf $BUILD_DIRECTORY
mkdir -p $BUILD_DIRECTORY/$BUILD_ARCHITECTURE/usr/share/dieklingel
mkdir -p $BUILD_DIRECTORY/$BUILD_ARCHITECTURE/usr/share/dieklingel/core
mkdir -p $BUILD_DIRECTORY/$BUILD_ARCHITECTURE/usr/share/dieklingel/exec

cp -r deb/$BUILD_ARCHITECTURE/* $BUILD_DIRECTORY/$BUILD_ARCHITECTURE
cp -r build/linux/$BUILD_ARCHITECTURE/release/bundle/* $BUILD_DIRECTORY/$BUILD_ARCHITECTURE/usr/share/dieklingel/core

DEB_PACKAGE_VERSION=$(cat $BUILD_DIRECTORY/$BUILD_ARCHITECTURE/DEBIAN/control | grep -Po '(?<=Version:\s)(.*)') 

dpkg-deb --build $BUILD_DIRECTORY/$BUILD_ARCHITECTURE dieklingel_${DEB_PACKAGE_VERSION}_${BUILD_ARCHITECTURE}.deb
