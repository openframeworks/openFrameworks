#!/bin/bash
cd "$(dirname "$0")"
# c++ main.cpp -stdlib=libc++ -std=c++17
c++ main.cpp -std=c++17 -lstdc++fs -o a
./a
