# Introduction

ofxCv represents an alternative approach to wrapping OpenCV for openFrameworks. It is designed for openFrameworks 007+ compatibility only. My first goal is to have a complete substitute for ofxOpenCv, at which point I will start versioning releases. Until then, I don't recommend that anyone use it as it will be undergoing irregular massive rehauling.

# Goals

ofxCv has a few goals driving its development.

### Wrap complex things in a helpful way

Sometimes this means: providing wrapper functions that require fewer arguments than the real CV functions, providing a smart interface that handles dynamic memory allocation to make things faster for you, or providing in place and out of place alternatives.

### Present the power of OpenCv clearly

This means naming things in an intuitive way, and, more importantly, providing classes that have methods that transform the data represented by that class. It also means providing demos of CV functions, and generally being more useful than ofxOpenCv.

### Interoperability of openFrameworks and OpenCv

Making it easy to work directly with CV by providing lightweight conversion functions, and providing wrappers for CV functions that do the conversions for you.

### Elegant internal OpenCv code

Provide clean implementations of all functions in order to provide a stepping stone to direct OpenCV use. This means using function names and variable names that follow the OpenCV documentation, and spending the time to learn proper CV usage so I can explain it clearly to others through code. Sometimes there will be heavy templating or other difficult constructs, but these should be avoided as much as possible.

# Usage

Notes on using ofxCv with your project.

## Important files

Using ofxCv requires:

* ofxCv/src/ The core of ofxCv.
* opencv/include/ The OpenCv headers, located in addons/ofxOpenCv/
* opencv/lib/ The precompiled static OpenCv libraries, located in addons/ofxOpenCv/

Your linker will also need to know where the OpenCv headers are. In XCode this means modifying one line in Project.xconfig:

	HEADER_SEARCH_PATHS = $(OF_CORE_HEADERS) "../../../addons/ofxOpenCv/libs/opencv/include/"

## Including ofxCv

Inside your testApp.h you will need:

	#include "ofxCv.h"

There is only one include. You can also make your code shorter by adding:

	using namespace cv;
	using namespace ofxCv;

This way, instead of writing `ofxCv::toCv()` you can just write `toCv()`. Likewise, if you're using OpenCv directly, instead of saying `cv::blur()` you can just say `blur()`. Sometimes a function or object might be ambiguous, and you'll need to say `cv::` or `ofxCv::` anyway to disambiguate. For example, `cv::Point`.

## Working with ofxCv

Unlike ofxOpenCv, ofxCv encourages you to use either native openFrameworks types or native OpenCv types rather than introducing a third type (e.g., ofxCvGrayscaleImage). To work with OF and OpenCv types in a fluid way, ofxCv includes the `toCv()` and `toOf()` functions. They provide the ability to convert openFrameworks data to OpenCv data and vice versa. For large data, like images, this is done by wrapping the data rather than copying it. For small data, like vectors, this is done by copying the data.

The rest of ofxCv is mostly helper functions (for example, `threshold()`) and wrapper classes (for example, `Calibration`).

### toCv()

`toCv()` is used to convert openFrameworks data to OpenCv data. For example:

	ofImage img;
	img.loadImage("image.png");
	Mat imgMat = toCv(img);

This creates a wrapper for `img` called `imgMat`. To create a deep copy, use `clone()`:

	Mat imgMatClone = toCv(img).clone();

### imitate()

`imitate()` is primarily used internally by ofxCv. When doing CV work, you regularly want to allocate multiple buffers of similar dimensions and channels. `imitate()` follows a prototype pattern, where you pass a prototype image `original` and the image to be allocated `mirror` to `imitate(mirror, original)`. `imitate()` has two big advantages:

* It works with `Mat`, `ofImage`, `ofPixels`, `ofVideoGrabber`, and anything else that extends `ofBaseHasPixels`.
* It will only reallocate memory if necessary. This means it can be used liberally.

- - --

ofxCv was developed with support from YCAM http://ycam.jp/