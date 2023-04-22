#!/bin/bash
cd "$(dirname "$0")"
c++ main.cpp  -stdlib=libc++ -std=c++17
./a.out