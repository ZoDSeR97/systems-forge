#!/usr/bin/env bash
# build.sh - Clean, configure, and build with CMake + Ninja

set -e  # exit on any error

echo "Removing old build directory..."
rm -rf build
echo "Removed build directory."

echo "Configuring project with CMake..."
cmake -S . -B build

echo "Entering build directory..."
cd build

echo "Running Ninja build..."
ninja

echo "Build completed successfully."