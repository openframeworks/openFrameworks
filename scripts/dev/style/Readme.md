# Source Code Styling

This directory contains scripts and a config file for auto formatting source code according to the OpenFrameworks coding style guidelines:
https://github.com/openframeworks/openFrameworks/wiki/oF-code-style

The formatter used is `uncrustify`: http://uncrustify.sourceforge.net/

The minimum version of `uncrustify` is 0.58.
If an earlier version is used, certain code segments may be formatted incorrectly.
E.g., 0.58 corrects a formatting edgecase with the `tok_split_gte` flag.

Install it to your standard path for the scripts to work:

* Mac OSX
	* homebrew: brew install uncrustify
	* macports: sudo port install uncrustify
* Ubuntu
	* sudo apt-get install uncrustify

The files currently formatted are .h & .cpp, although Obj-C/C++ support may be added in the future.

## openFrameworks_style.cfg

This is the uncrustify config file for the OpenFrameworks style. It was created using Universal Indent GUI: http://universalindent.sourceforge.net/

It is **highly** recommended that you use Universal Indent GUI to modify this file as there are hundreds of options and little documentation elsewhere. 

## ofStyler

This script runs uncrustify on a given file or directory of files using the OF style config file.

It handless paths and can be used from any folder.

### Usage

	ofStyler dirname filesuffix
	ofStyler filename

Example:

	ofStyler ../../../libs/openFrameworks cpp
	ofStyler myFile.cpp

## styleAddon

This script runs the OF styler on the src directory of a given addon.

### Usage

	styleAddon dirname

Example:

	styleAddon ofxXmlSettings

## styleOFCore

This script runs the OF styler on the core in `libs/openFrameworks/`.
