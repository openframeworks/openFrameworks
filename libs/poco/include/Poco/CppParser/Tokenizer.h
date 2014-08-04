//
// Tokenizer.h
//
// $Id: //poco/1.4/CppParser/include/Poco/CppParser/Tokenizer.h#1 $
//
// Library: CppParser
// Package: CppParser
// Module:  Tokenizer
//
// Definition of the Tokenizer class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CppParser_Tokenizer_INCLUDED
#define CppParser_Tokenizer_INCLUDED


#include "Poco/CppParser/CppParser.h"
#include "Poco/StreamTokenizer.h"


namespace Poco {
namespace CppParser {


class CppParser_API Tokenizer: public Poco::StreamTokenizer
	/// A Tokenizer for C++.
{
public:
	Tokenizer(std::istream& istr);
		/// Creates the Tokenizer.

	~Tokenizer();
		/// Destroys the Tokenizer.
};


} } // namespace Poco::CppParser


#endif // CppParser_Tokenizer_INCLUDED
