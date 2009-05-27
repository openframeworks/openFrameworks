//
// Void.h
//
// $Id: //poco/1.3/Foundation/include/Poco/Void.h#1 $
//
// Library: Foundation
// Package: Core
// Module:  Void
//
// Definition of the Void class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// 3. Redistributions in any form must be accompanied by information on
//    how to obtain complete source code for this software and any
//    accompanying software that uses this software.  The source code
//    must either be included in the distribution or be available for no
//    more than the cost of distribution plus a nominal fee, and must be
//    freely redistributable under reasonable conditions.  For an
//    executable file, complete source code means the source code for all
//    modules it contains.  It does not include source code for modules or
//    files that typically accompany the major components of the operating
//    system on which the executable file runs.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//


#ifndef Foundation_Void_INCLUDED
#define Foundation_Void_INCLUDED


#include "Poco/Foundation.h"


namespace Poco {


class Foundation_API Void
	/// A dummy class with value-type semantics, 
	/// mostly useful as a template argument.
	///
	/// This class is typically used together with ActiveMethod,
	/// if no argument or return value is needed.
{
public:
	Void();
		/// Creates the Void.

	Void(const Void& v);
		/// Creates the Void from another Void.
		///
		/// The philosophical aspects of this operation 
		/// remain undiscussed for now.
		
	~Void();
		/// Destroys the Void.

	Void& operator = (const Void& v);
		/// Assigns another void.

	bool operator ==(const Void& v) const;
		/// Will return always true due to Voids having no members.

	bool operator !=(const Void& v) const;
		/// Will return always false due to Voids having no members.
};


inline bool Void::operator ==(const Void& v) const
{
	return true;
}


inline bool Void::operator !=(const Void& v) const
{
	return false;
}


} // namespace Poco


#endif // Foundation_Void_INCLUDED
