#!/bin/bash
set -e
cd $OF_ROOT
cp scripts/linux/template/linux/Makefile examples/empty/emptyExample/
cp scripts/linux/template/linux/config.make examples/empty/emptyExample/
cd examples/empty/emptyExample
make Debug
