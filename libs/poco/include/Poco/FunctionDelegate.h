//
// FunctionDelegate.h
//
// $Id: //poco/1.4/Foundation/include/Poco/FunctionDelegate.h#4 $
//
// Library: Foundation
// Package: Events
// Module:  FunctionDelegate
//
// Implementation of the FunctionDelegate template.
//
// Copyright (c) 2006-2011, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_FunctionDelegate_INCLUDED
#define Foundation_FunctionDelegate_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/AbstractDelegate.h"
#include "Poco/Mutex.h"


namespace Poco {


template <class TArgs, bool hasSender = true, bool senderIsConst = true> 
class FunctionDelegate: public AbstractDelegate<TArgs>
	/// Wraps a freestanding function or static member function 
	/// for use as a Delegate.
{
public:
	typedef void (*NotifyMethod)(const void*, TArgs&);

	FunctionDelegate(NotifyMethod method):
		_receiverMethod(method)
	{
	}

	FunctionDelegate(const FunctionDelegate& delegate):
		AbstractDelegate<TArgs>(delegate),
		_receiverMethod(delegate._receiverMethod)
	{
	}

	~FunctionDelegate()
	{
	}
	
	FunctionDelegate& operator = (const FunctionDelegate& delegate)
	{
		if (&delegate != this)
		{
			this->_pTarget        = delegate._pTarget;
			this->_receiverMethod = delegate._receiverMethod;
		}
		return *this;
	}

	bool notify(const void* sender, TArgs& arguments)
	{
		Mutex::ScopedLock lock(_mutex);
		if (_receiverMethod)
		{
			(*_receiverMethod)(sender, arguments);
			return true;
		}
		else return false;
	}

	bool equals(const AbstractDelegate<TArgs>& other) const
	{
		const FunctionDelegate* pOtherDelegate = dynamic_cast<const FunctionDelegate*>(other.unwrap());
		return pOtherDelegate && _receiverMethod == pOtherDelegate->_receiverMethod;
	}

	AbstractDelegate<TArgs>* clone() const
	{
		return new FunctionDelegate(*this);
	}
	
	void disable()
	{
		Mutex::ScopedLock lock(_mutex);
		_receiverMethod = 0;
	}

protected:
	NotifyMethod _receiverMethod;
	Mutex _mutex;

private:
	FunctionDelegate();
};


template <class TArgs> 
class FunctionDelegate<TArgs, true, false>: public AbstractDelegate<TArgs>
{
public:
	typedef void (*NotifyMethod)(void*, TArgs&);

	FunctionDelegate(NotifyMethod method):
		_receiverMethod(method)
	{
	}

	FunctionDelegate(const FunctionDelegate& delegate):
		AbstractDelegate<TArgs>(delegate),
		_receiverMethod(delegate._receiverMethod)
	{
	}

	~FunctionDelegate()
	{
	}
	
	FunctionDelegate& operator = (const FunctionDelegate& delegate)
	{
		if (&delegate != this)
		{
			this->_pTarget        = delegate._pTarget;
			this->_receiverMethod = delegate._receiverMethod;
		}
		return *this;
	}

	bool notify(const void* sender, TArgs& arguments)
	{
		Mutex::ScopedLock lock(_mutex);
		if (_receiverMethod)
		{
			(*_receiverMethod)(const_cast<void*>(sender), arguments);
			return true;
		}
		else return false;
	}

	bool equals(const AbstractDelegate<TArgs>& other) const
	{
		const FunctionDelegate* pOtherDelegate = dynamic_cast<const FunctionDelegate*>(other.unwrap());
		return pOtherDelegate && _receiverMethod == pOtherDelegate->_receiverMethod;
	}

	AbstractDelegate<TArgs>* clone() const
	{
		return new FunctionDelegate(*this);
	}

	void disable()
	{
		Mutex::ScopedLock lock(_mutex);
		_receiverMethod = 0;
	}

protected:
	NotifyMethod _receiverMethod;
	Mutex _mutex;

private:
	FunctionDelegate();
};


template <class TArgs, bool senderIsConst> 
class FunctionDelegate<TArgs, false, senderIsConst>: public AbstractDelegate<TArgs>
{
public:
	typedef void (*NotifyMethod)(TArgs&);

	FunctionDelegate(NotifyMethod method):
		_receiverMethod(method)
	{
	}

	FunctionDelegate(const FunctionDelegate& delegate):
		AbstractDelegate<TArgs>(delegate),
		_receiverMethod(delegate._receiverMethod)
	{
	}

	~FunctionDelegate()
	{
	}
	
	FunctionDelegate& operator = (const FunctionDelegate& delegate)
	{
		if (&delegate != this)
		{
			this->_pTarget        = delegate._pTarget;
			this->_receiverMethod = delegate._receiverMethod;
		}
		return *this;
	}

	bool notify(const void* /*sender*/, TArgs& arguments)
	{
		Mutex::ScopedLock lock(_mutex);
		if (_receiverMethod)
		{
			(*_receiverMethod)(arguments);
			return true; 
		}
		else return false;
	}

	bool equals(const AbstractDelegate<TArgs>& other) const
	{
		const FunctionDelegate* pOtherDelegate = dynamic_cast<const FunctionDelegate*>(other.unwrap());
		return pOtherDelegate && _receiverMethod == pOtherDelegate->_receiverMethod;
	}

	AbstractDelegate<TArgs>* clone() const
	{
		return new FunctionDelegate(*this);
	}

	void disable()
	{
		Mutex::ScopedLock lock(_mutex);
		_receiverMethod = 0;
	}

protected:
	NotifyMethod _receiverMethod;
	Mutex _mutex;

private:
	FunctionDelegate();
};






template <> 
class FunctionDelegate<void,true,true>: public AbstractDelegate<void>
	/// Wraps a freestanding function or static member function 
	/// for use as a Delegate.
{
public:
	typedef void (*NotifyMethod)(const void*);

	FunctionDelegate(NotifyMethod method):
		_receiverMethod(method)
	{
	}

	FunctionDelegate(const FunctionDelegate& delegate):
		AbstractDelegate<void>(delegate),
		_receiverMethod(delegate._receiverMethod)
	{
	}

	~FunctionDelegate()
	{
	}
	
	FunctionDelegate& operator = (const FunctionDelegate& delegate)
	{
		if (&delegate != this)
		{
			//this->_pTarget        = delegate._pTarget;
			this->_receiverMethod = delegate._receiverMethod;
		}
		return *this;
	}

	bool notify(const void* sender)
	{
		Mutex::ScopedLock lock(_mutex);
		if (_receiverMethod)
		{
			(*_receiverMethod)(sender);
			return true;
		}
		else return false;
	}

	bool equals(const AbstractDelegate<void>& other) const
	{
		const FunctionDelegate* pOtherDelegate = dynamic_cast<const FunctionDelegate*>(other.unwrap());
		return pOtherDelegate && _receiverMethod == pOtherDelegate->_receiverMethod;
	}

	AbstractDelegate<void>* clone() const
	{
		return new FunctionDelegate(*this);
	}
	
	void disable()
	{
		Mutex::ScopedLock lock(_mutex);
		_receiverMethod = 0;
	}

protected:
	NotifyMethod _receiverMethod;
	Mutex _mutex;

private:
	FunctionDelegate();
};


template <> 
class FunctionDelegate<void, true, false>: public AbstractDelegate<void>
{
public:
	typedef void (*NotifyMethod)(void*);

	FunctionDelegate(NotifyMethod method):
		_receiverMethod(method)
	{
	}

	FunctionDelegate(const FunctionDelegate& delegate):
		AbstractDelegate<void>(delegate),
		_receiverMethod(delegate._receiverMethod)
	{
	}

	~FunctionDelegate()
	{
	}
	
	FunctionDelegate& operator = (const FunctionDelegate& delegate)
	{
		if (&delegate != this)
		{
			//this->_pTarget        = delegate._pTarget;
			this->_receiverMethod = delegate._receiverMethod;
		}
		return *this;
	}

	bool notify(const void* sender)
	{
		Mutex::ScopedLock lock(_mutex);
		if (_receiverMethod)
		{
			(*_receiverMethod)(const_cast<void*>(sender));
			return true;
		}
		else return false;
	}

	bool equals(const AbstractDelegate<void>& other) const
	{
		const FunctionDelegate* pOtherDelegate = dynamic_cast<const FunctionDelegate*>(other.unwrap());
		return pOtherDelegate && _receiverMethod == pOtherDelegate->_receiverMethod;
	}

	AbstractDelegate<void>* clone() const
	{
		return new FunctionDelegate(*this);
	}

	void disable()
	{
		Mutex::ScopedLock lock(_mutex);
		_receiverMethod = 0;
	}

protected:
	NotifyMethod _receiverMethod;
	Mutex _mutex;

private:
	FunctionDelegate();
};


template <bool senderIsConst> 
class FunctionDelegate<void, false, senderIsConst>: public AbstractDelegate<void>
{
public:
	typedef void (*NotifyMethod)();

	FunctionDelegate(NotifyMethod method):
		_receiverMethod(method)
	{
	}

	FunctionDelegate(const FunctionDelegate& delegate):
		AbstractDelegate<void>(delegate),
		_receiverMethod(delegate._receiverMethod)
	{
	}

	~FunctionDelegate()
	{
	}
	
	FunctionDelegate& operator = (const FunctionDelegate& delegate)
	{
		if (&delegate != this)
		{
			//this->_pTarget        = delegate._pTarget;
			this->_receiverMethod = delegate._receiverMethod;
		}
		return *this;
	}

	bool notify(const void* /*sender*/)
	{
		Mutex::ScopedLock lock(_mutex);
		if (_receiverMethod)
		{
			(*_receiverMethod)();
			return true; 
		}
		else return false;
	}

	bool equals(const AbstractDelegate<void>& other) const
	{
		const FunctionDelegate* pOtherDelegate = dynamic_cast<const FunctionDelegate*>(other.unwrap());
		return pOtherDelegate && _receiverMethod == pOtherDelegate->_receiverMethod;
	}

	AbstractDelegate<void>* clone() const
	{
		return new FunctionDelegate(*this);
	}

	void disable()
	{
		Mutex::ScopedLock lock(_mutex);
		_receiverMethod = 0;
	}

protected:
	NotifyMethod _receiverMethod;
	Mutex _mutex;

private:
	FunctionDelegate();
};


} // namespace Poco


#endif // Foundation_FunctionDelegate_INCLUDED
