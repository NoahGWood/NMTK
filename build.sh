#!/usr/bin/env bash
set -e # Exit on error
cmake -B build -G Ninja .
cmake --build build
./build/myapp