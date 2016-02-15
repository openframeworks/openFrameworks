//
// Mutex_WIN32.h
//
// $Id: //poco/1.4/Foundation/include/Poco/Mutex_WIN32.h#1 $
//
// Library: Foundation
// Package: Threading
// Module:  Mutex
//
// Definition of the MutexImpl and FastMutexImpl classes for WIN32.
//
// Copyright (c) 2004-2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Mutex_WIN32_INCLUDED
#define Foundation_Mutex_WIN32_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Exception.h"
#include "Poco/UnWindows.h"


namespace Poco {


class Foundation_API MutexImpl
{
public:
	enum MutexTypeImpl
	{
		MUTEX_RECURSIVE_IMPL,
		MUTEX_NONRECURSIVE_IMPL,
	};

protected:
	explicit MutexImpl(MutexTypeImpl type);
	~MutexImpl();
	void lockImpl();
	bool tryLockImpl();
	bool tryLockImpl(long milliseconds);
	void unlockImpl();
	
private:
	CRITICAL_SECTION _cs;
	int _lockCount;
	const bool _recursive;

private:
	MutexImpl(const MutexImpl&);
	MutexImpl& operator = (const MutexImpl&);
};


class Foundation_API FastMutexImpl
{
protected:
	FastMutexImpl();
	~FastMutexImpl();
	void lockImpl();
	bool tryLockImpl();
	bool tryLockImpl(long milliseconds);
	void unlockImpl();
	
private:
	CRITICAL_SECTION _cs;
};


//
// inlines
//
inline void MutexImpl::unlockImpl()
{
	--_lockCount;
	LeaveCriticalSection(&_cs);
}


inline void FastMutexImpl::lockImpl()
{
	try
	{
		EnterCriticalSection(&_cs);
	}
	catch (...)
	{
		throw SystemException("cannot lock mutex");
	}
}


inline bool FastMutexImpl::tryLockImpl()
{
	try
	{
		return TryEnterCriticalSection(&_cs) != 0;
	}
	catch (...)
	{
	}
	throw SystemException("cannot lock mutex");
}


inline void FastMutexImpl::unlockImpl()
{
	LeaveCriticalSection(&_cs);
}


} // namespace Poco


#endif // Foundation_Mutex_WIN32_INCLUDED
