//
// JSONString.h
//
// $Id: //poco/1.4/Foundation/include/Poco/JSONString.h#1 $
//
// Library: Foundation
// Package: Core
// Module:  String
//
// JSONString utility functions.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_JSONString_INCLUDED
#define Foundation_JSONString_INCLUDED


#include "Poco/Foundation.h"


namespace Poco {


std::string Foundation_API toJSON(char c);
	/// Utility function for escaping JSON characters.


void Foundation_API toJSON(const std::string& value, std::ostream& out, bool wrap = true);
	/// Formats string value into the supplied output stream by 
	/// escaping control characters.
	/// If wrap is true, the resulting string is enclosed in double quotes


std::string Foundation_API toJSON(const std::string& value, bool wrap = true);
	/// Formats string value by escaping control characters.
	/// If wrap is true, the resulting string is enclosed in double quotes
	/// Returns formatted string.


} // namespace Poco


#endif // Foundation_JSONString_INCLUDED
