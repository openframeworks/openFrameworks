// =============================================================================
//
// Copyright (c) 2009-2016 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#pragma once


#include "ofBaseTypes.h"


#include <type_traits>
#include <utility>
#include <typeinfo>
#include <string>
#include <cassert>


namespace ofx {
namespace DOM {


class Element;


typedef ofPoint Position;
typedef ofPoint Size;
typedef ofRectangle Shape;

/// \brief The orientation of a Widget.
/// \todo Replace this with ofOrientation.

//    OF_ORIENTATION_DEFAULT = 1,
//    OF_ORIENTATION_180 = 2,
//    OF_ORIENTATION_90_LEFT = 3,
//    OF_ORIENTATION_90_RIGHT = 4,
//    OF_ORIENTATION_UNKNOWN = 5

enum class Orientation
{
	/// \brief Locks the Orientation to landscape.
	HORIZONTAL,
	/// \brief Locks the Orientation to portrait.
	VERTICAL,
	/// \brief Sets the Orientation based on the aspect ratio.
	DEFAULT
};

/// \brief The float of a gui element in a layout.
enum class LayoutFloat {
	/// \brief Left float.
	LEFT,
	/// \brief Right float.
	RIGHT,
	/// \brief No float.
	NONE
};

template <class T>
using StorageType = typename decay<T>::type;

/// \brief C++11 Any class.
/// \sa https://codereview.stackexchange.com/questions/20058/c11-any-class
/// \note This class may change in the near future.
struct Any
{
	bool is_null() const { return !ptr; }
	bool not_null() const { return ptr; }

	template <typename U> Any(U&& value)
	: ptr(new Derived<StorageType<U>>(std::forward<U>(value)))
	{
	}

	template <class U> bool is() const
	{
		typedef StorageType<U> T;

		auto derived = dynamic_cast<Derived<T>*> (ptr);

		return derived;
	}

	template <class U>
	StorageType<U>& as()
	{
		typedef StorageType<U> T;

		auto derived = dynamic_cast<Derived<T>*> (ptr);

		if (!derived)
			throw bad_cast();

		return derived->value;
	}

	template <class U>
	operator U()
	{
		return as<StorageType<U>>();
	}

	Any(): ptr(nullptr)
	{
	}

	Any(Any& that): ptr(that.clone())
	{

	}

	Any(Any&& that): ptr(that.ptr)
	{
		that.ptr = nullptr;
	}

	Any(const Any& that): ptr(that.clone())
	{
	}


	Any(const Any&& that): ptr(that.clone())
	{
	}


	Any& operator=(const Any& a)
	{
		if (ptr == a.ptr)
			return *this;

		auto old_ptr = ptr;

		ptr = a.clone();

		if (old_ptr)
			delete old_ptr;

		return *this;
	}

	Any& operator=(Any&& a)
	{
		if (ptr == a.ptr)
			return *this;

		std::swap(ptr, a.ptr);

		return *this;
	}

	~Any()
	{
		if (ptr)
			delete ptr;
	}

private:
	struct Base
	{
		virtual ~Base() {}

		virtual Base* clone() const = 0;
	};

	template <typename T>
	struct Derived: Base
	{
		template <typename U> Derived(U&& value) : value(forward<U>(value)) { }

		T value;

		Base* clone() const { return new Derived<T>(value); }
	};

	Base* clone() const
	{
		if (ptr)
			return ptr->clone();
		else
			return nullptr;
	}

	Base* ptr;
};


} } // namespace ofx::DOM
