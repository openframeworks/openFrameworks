# Introduction

ofxCv represents an alternative approach to wrapping OpenCV for openFrameworks. It is designed for openFrameworks 007+ compatibility only. My first goal is to have a complete substitute for ofxOpenCv, at which point I will start versioning releases. Until then, I don't recommend that anyone use it as it will be undergoing irregular massive rehauling.

# Installation

Either clone out the source code using git:

	> cd openFrameworks/addons/
	> git clone https://github.com/kylemcdonald/ofxCv.git

Or download the source from GitHub [here](https://github.com/kylemcdonald/ofxCv/archive/master.zip), unzip the folder, rename it from `ofxCv-master` to `ofxCv` and place it in your `openFrameworks/addons` folder.

You don't need to move any of the examples anywhere, you can run them in place.

# Goals

ofxCv has a few goals driving its development.

### Wrap complex things in a helpful way

Sometimes this means: providing wrapper functions that require fewer arguments than the real CV functions, providing a smart interface that handles dynamic memory allocation to make things faster for you, or providing in place and out of place alternatives.

### Present the power of OpenCv clearly

This means naming things in an intuitive way, and, more importantly, providing classes that have methods that transform the data represented by that class. It also means providing demos of CV functions, and generally being more useful than ofxOpenCv.

### Interoperability of openFrameworks and OpenCv

Making it easy to work directly with CV by providing lightweight conversion functions, and providing wrappers for CV functions that do the conversions for you.

### Elegant internal OpenCv code

Provide clean implementations of all functions in order to provide a stepping stone to direct OpenCV use. This means using function names and variable names that follow the OpenCV documentation, and spending the time to learn proper CV usage so I can explain it clearly to others through code. Sometimes there will be heavy templating in order to make OF interoperable with OpenCV, but this should be avoided in favor of using straight OpenCV as often as possible.

# Usage

Sometimes this readme will fall out of date. Please refer to the examples as the primary reference in that case.

## Project setup

Using ofxCv requires:

* ofxCv/libs/ofxCv/include/ Which contains all the ofxCv headers.
* ofxCv/libs/ofxCv/src/ Which contains all the ofxCv source.
* ofxCv/src/ Which ties together all of ofxCv into a single include.
* opencv/include/ The OpenCv headers, located in addons/ofxOpenCv/
* opencv/lib/ The precompiled static OpenCv libraries, located in addons/ofxOpenCv/

Your linker will also need to know where the OpenCv headers are. In XCode this means modifying one line in Project.xconfig:

	HEADER_SEARCH_PATHS = $(OF_CORE_HEADERS) "../../../addons/ofxOpenCv/libs/opencv/include/" "../../../addons/ofxCv/libs/ofxCv/include/"

## Including ofxCv

Inside your testApp.h you will need one include:

	#include "ofxCv.h"

OpenCv uses the `cv` namespace, and ofxCv uses the `ofxCv` namespace. You can automatically import them by writing this in your `.cpp` files:

	using namespace cv;
	using namespace ofxCv;

If you look inside the ofxCv source, you'll find lots of cases of `ofxCv::` and `cv::`. In some rare cases, you'll need to write `cv::` in your code. For example, on OSX `Rect` and `Point` are defined by OpenCv, but also `MacTypes.h`. So if you're using an OpenCv `Rect` or `Point` you'll need to say so explicitly with `cv::Rect` or `cv::Point` to disambiguate.

ofxCv takes advantage of namespaces by using overloaded function names. This means that the ofxCv wrapper for `cv::Canny()` is also called `ofxCv::Canny()`. If you write simply `Canny()`, the correct function will be chosen based on the arguments you pass.

## Working with ofxCv

Unlike ofxOpenCv, ofxCv encourages you to use either native openFrameworks types or native OpenCv types, rather than introducing a third type like `ofxCvImage`. To work with OF and OpenCv types in a fluid way, ofxCv includes the `toCv()` and `toOf()` functions. They provide the ability to convert openFrameworks data to OpenCv data and vice versa. For large data, like images, this is done by wrapping the data rather than copying it. For small data, like vectors, this is done by copying the data.

The rest of ofxCv is mostly helper functions (for example, `threshold()`) and wrapper classes (for example, `Calibration`).

### toCv() and copy()

`toCv()` is used to convert openFrameworks data to OpenCv data. For example:

	ofImage img;
	img.loadImage("image.png");
	Mat imgMat = toCv(img);

This creates a wrapper for `img` called `imgMat`. To create a deep copy, use `clone()`:

	Mat imgMatClone = toCv(img).clone();

Or `copy()`, which works with any type supported by `toCv()`:

	Mat imgCopy;
	copy(img, imgCopy);

`toCv()` is similar to ofxOpenCv's `ofxCvImage::getCvImage()` method, which returns an `IplImage*`. The biggest difference is that you can't always use `toCv()` "in place" when calling OpenCv code directly. In other words, you can always write this:

	Mat imgMat = toCv(img);
	cv::someFunction(imgMat, ...);

But you should avoid using `toCv()` like this:

	cv::someFunction(toCv(img), ...);

Because there are cases where in place usage will cause a compile error. More specifically, calling `toCv()` in place will fail if the function requires a non-const reference for that parameter.

### imitate()

`imitate()` is primarily used internally by ofxCv. When doing CV, you regularly want to allocate multiple buffers of similar dimensions and channels. `imitate()` follows a kind of prototype pattern, where you pass a prototype image `original` and the image to be allocated `mirror` to `imitate(mirror, original)`. `imitate()` has two big advantages:

* It works with `Mat`, `ofImage`, `ofPixels`, `ofVideoGrabber`, and anything else that extends `ofBaseHasPixels`.
* It will only reallocate memory if necessary. This means it can be used liberally.

If you are writing a function that returns data, the ofxCv style is to call `imitate()` on the data to be returned from inside the function, allocating it as necessary.

### drawMat()

Sometimes you want to draw a `Mat` to the screen directly, as quickly and easily as possible, and `drawMat()` will do this for you. `drawMat()` is not the most optimal way of drawing images to the screen, because it creates a texture every time it draws. If you want to draw things for efficiently, you should allocate an `ofImage img;`, use `Mat mat = toCv(img);` to treat it as a `Mat`, and then call `img.update()` to upload the modified pixels to the graphics card so you can see the results in `img.draw()`.

# Working with OpenCv 2

OpenCv 2 is an incredibly well designed API, and ofxCv encourages you to use it directly. Here are some hints on using OpenCv.

### OpenCv Types

OpenCv 2 uses the `Mat` class in place of the old `IplImage`. Memory allocation, copying, and deallocation are all handled automatically. `operator=` is a shallow, reference-counted copy. A `Mat` contains a collection of `Scalar` objects. A `Scalar` contains a collection of basic types (unsigned char, bool, double, etc.). `Scalar` is a short vector for representing color or other multidimensional information. The hierarchy is: `Mat` contains `Scalar`, `Scalar` contains basic types.

Different functions accept `Mat` in different ways:

* `Mat` will create a lightweight copy of the underlying data. It's easy to write, and it allows you to use `toCv()` "in-place" when passing arguments to the function.
* `Mat&` allows the function to modify the header passed in. This means the function can allocate if necessary.
* `const Mat&` means that the function isn't going to modify the underlying data. This should be used instead of `Mat` when possible. It also allows "in-place" `toCv()` usage.

### Mat creation

If you're working with `Mat` directly, it's important to remember that OpenCv talks about `rows` and `cols` rather than `width` and `height`. This means that the arguments are "backwards" when they appear in the `Mat` constructor. Here's an example of creating a `Mat` wrapper for some grayscale `unsigned char* pixels` for which we know the `width` and `height`:

	Mat mat = Mat(height, width, CV_8UC1, pixels, 0);

### Mat operations

Basic mathematical operations on `Mat` objects of the same size and type can be accomplished with matrix expressions. Matrix expressions are a collection of overloaded operators that accept `Mat`, `Scalar`, and basic types. A normal mathematical operation might look like:

	float x, a, b;
	...
	x = (a + b) * 10;

A matrix operation looks similar:

	Mat x, a, b;
	...
	x = (a + b) * 10;

This will add every element of `a` and `b`, then multiply the results by 10, and finally assign the result to `x`.

Available matrix expressions include mathematical operators `+`, `-`, `/` (per element division), `*` (matrix multiplication), `.mul()` (per-element multiplication). As well as comparison operators `!=`, `==`, `<`, `>`, `>=`, `<=` (useful for thresholding). Binary operators `&`, `|`, `^`, `~`. And a few others like `abs()`, `min()`, and `max()`. For the complete listing see the OpenCv documention or `mat.hpp`.

# Code Style

ofxCv tries to have a consistent code style. It's most similar to the K&R variant used for Java, and the indentation is primarily determined by XCode's auto-indent feature.

Multiline comments are used for anything beyond two lines.

Case statements have a `default:` fall-through with the last case.

When two or three similar variables are initialized, commas are used instead of multiple lines. For example `Mat srcMat = toCv(src), dstMat = toCv(dst);`. This style was inherited from reading Jason Saragih's FaceTracker.

- - --

*ofxCv was developed with support from [Yamaguchi Center for Arts and Media](http://ycam.jp/).*