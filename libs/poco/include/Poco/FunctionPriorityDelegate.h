//
// FunctionPriorityDelegate.h
//
// $Id: //poco/1.4/Foundation/include/Poco/FunctionPriorityDelegate.h#5 $
//
// Library: Foundation
// Package: Events
// Module:  FunctionPriorityDelegate
//
// Implementation of the FunctionPriorityDelegate template.
//
// Copyright (c) 2006-2011, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_FunctionPriorityDelegate_INCLUDED
#define Foundation_FunctionPriorityDelegate_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/AbstractPriorityDelegate.h"
#include "Poco/Mutex.h"


namespace Poco {


template <class TArgs, bool useSender = true, bool senderIsConst = true> 
class FunctionPriorityDelegate: public AbstractPriorityDelegate<TArgs>
	/// Wraps a freestanding function or static member function 
	/// for use as a PriorityDelegate.
{
public:
	typedef void (*NotifyMethod)(const void*, TArgs&);

	FunctionPriorityDelegate(NotifyMethod method, int prio):
		AbstractPriorityDelegate<TArgs>(prio),
		_receiverMethod(method)
	{
	}
	
	FunctionPriorityDelegate(const FunctionPriorityDelegate& delegate):
		AbstractPriorityDelegate<TArgs>(delegate),
		_receiverMethod(delegate._receiverMethod)
	{
	}
	
	FunctionPriorityDelegate& operator = (const FunctionPriorityDelegate& delegate)
	{
		if (&delegate != this)
		{
			this->_pTarget        = delegate._pTarget;
			this->_receiverMethod = delegate._receiverMethod;
			this->_priority       = delegate._priority;
		}
		return *this;
	}

	~FunctionPriorityDelegate()
	{
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
		const FunctionPriorityDelegate* pOtherDelegate = dynamic_cast<const FunctionPriorityDelegate*>(other.unwrap());
		return pOtherDelegate && this->priority() == pOtherDelegate->priority() && _receiverMethod == pOtherDelegate->_receiverMethod;
	}

	AbstractDelegate<TArgs>* clone() const
	{
		return new FunctionPriorityDelegate(*this);
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
	FunctionPriorityDelegate();
};


template <class TArgs> 
class FunctionPriorityDelegate<TArgs, true, false>: public AbstractPriorityDelegate<TArgs>
{
public:
	typedef void (*NotifyMethod)(void*, TArgs&);

	FunctionPriorityDelegate(NotifyMethod method, int prio):
		AbstractPriorityDelegate<TArgs>(prio),
		_receiverMethod(method)
	{
	}
	
	FunctionPriorityDelegate(const FunctionPriorityDelegate& delegate):
		AbstractPriorityDelegate<TArgs>(delegate),
		_receiverMethod(delegate._receiverMethod)
	{
	}
	
	FunctionPriorityDelegate& operator = (const FunctionPriorityDelegate& delegate)
	{
		if (&delegate != this)
		{
			this->_pTarget        = delegate._pTarget;
			this->_receiverMethod = delegate._receiverMethod;
			this->_priority       = delegate._priority;
		}
		return *this;
	}

	~FunctionPriorityDelegate()
	{
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
		const FunctionPriorityDelegate* pOtherDelegate = dynamic_cast<const FunctionPriorityDelegate*>(other.unwrap());
		return pOtherDelegate && this->priority() == pOtherDelegate->priority() && _receiverMethod == pOtherDelegate->_receiverMethod;
	}

	AbstractDelegate<TArgs>* clone() const
	{
		return new FunctionPriorityDelegate(*this);
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
	FunctionPriorityDelegate();
};


template <class TArgs> 
class FunctionPriorityDelegate<TArgs, false>: public AbstractPriorityDelegate<TArgs>
{
public:
	typedef void (*NotifyMethod)(TArgs&);

	FunctionPriorityDelegate(NotifyMethod method, int prio):
		AbstractPriorityDelegate<TArgs>(prio),
		_receiverMethod(method)
	{
	}
	
	FunctionPriorityDelegate(const FunctionPriorityDelegate& delegate):
		AbstractPriorityDelegate<TArgs>(delegate),
		_receiverMethod(delegate._receiverMethod)
	{
	}
	
	FunctionPriorityDelegate& operator = (const FunctionPriorityDelegate& delegate)
	{
		if (&delegate != this)
		{
			this->_pTarget        = delegate._pTarget;
			this->_receiverMethod = delegate._receiverMethod;
			this->_priority       = delegate._priority;
		}
		return *this;
	}

	~FunctionPriorityDelegate()
	{
	}

	bool notify(const void* sender, TArgs& arguments)
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
		const FunctionPriorityDelegate* pOtherDelegate = dynamic_cast<const FunctionPriorityDelegate*>(other.unwrap());
		return pOtherDelegate && this->priority() == pOtherDelegate->priority() && _receiverMethod == pOtherDelegate->_receiverMethod;
	}

	AbstractDelegate<TArgs>* clone() const
	{
		return new FunctionPriorityDelegate(*this);
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
	FunctionPriorityDelegate();
};


template <>
class FunctionPriorityDelegate<void,true,true>: public AbstractPriorityDelegate<void>
	/// Wraps a freestanding function or static member function
	/// for use as a PriorityDelegate.
{
public:
	typedef void (*NotifyMethod)(const void*);

	FunctionPriorityDelegate(NotifyMethod method, int prio):
		AbstractPriorityDelegate<void>(prio),
		_receiverMethod(method)
	{
	}

	FunctionPriorityDelegate(const FunctionPriorityDelegate& delegate):
		AbstractPriorityDelegate<void>(delegate),
		_receiverMethod(delegate._receiverMethod)
	{
	}

	FunctionPriorityDelegate& operator = (const FunctionPriorityDelegate& delegate)
	{
		if (&delegate != this)
		{
			this->_receiverMethod = delegate._receiverMethod;
			this->_priority       = delegate._priority;
		}
		return *this;
	}

	~FunctionPriorityDelegate()
	{
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
		const FunctionPriorityDelegate* pOtherDelegate = dynamic_cast<const FunctionPriorityDelegate*>(other.unwrap());
		return pOtherDelegate && this->priority() == pOtherDelegate->priority() && _receiverMethod == pOtherDelegate->_receiverMethod;
	}

	AbstractDelegate<void>* clone() const
	{
		return new FunctionPriorityDelegate(*this);
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
	FunctionPriorityDelegate();
};


template <>
class FunctionPriorityDelegate<void, true, false>: public AbstractPriorityDelegate<void>
{
public:
	typedef void (*NotifyMethod)(void*);

	FunctionPriorityDelegate(NotifyMethod method, int prio):
		AbstractPriorityDelegate<void>(prio),
		_receiverMethod(method)
	{
	}

	FunctionPriorityDelegate(const FunctionPriorityDelegate& delegate):
		AbstractPriorityDelegate<void>(delegate),
		_receiverMethod(delegate._receiverMethod)
	{
	}

	FunctionPriorityDelegate& operator = (const FunctionPriorityDelegate& delegate)
	{
		if (&delegate != this)
		{
			this->_receiverMethod = delegate._receiverMethod;
			this->_priority       = delegate._priority;
		}
		return *this;
	}

	~FunctionPriorityDelegate()
	{
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
		const FunctionPriorityDelegate* pOtherDelegate = dynamic_cast<const FunctionPriorityDelegate*>(other.unwrap());
		return pOtherDelegate && this->priority() == pOtherDelegate->priority() && _receiverMethod == pOtherDelegate->_receiverMethod;
	}

	AbstractDelegate<void>* clone() const
	{
		return new FunctionPriorityDelegate(*this);
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
	FunctionPriorityDelegate();
};


template <>
class FunctionPriorityDelegate<void, false>: public AbstractPriorityDelegate<void>
{
public:
	typedef void (*NotifyMethod)();

	FunctionPriorityDelegate(NotifyMethod method, int prio):
		AbstractPriorityDelegate<void>(prio),
		_receiverMethod(method)
	{
	}

	FunctionPriorityDelegate(const FunctionPriorityDelegate& delegate):
		AbstractPriorityDelegate<void>(delegate),
		_receiverMethod(delegate._receiverMethod)
	{
	}

	FunctionPriorityDelegate& operator = (const FunctionPriorityDelegate& delegate)
	{
		if (&delegate != this)
		{
			this->_receiverMethod = delegate._receiverMethod;
			this->_priority       = delegate._priority;
		}
		return *this;
	}

	~FunctionPriorityDelegate()
	{
	}

	bool notify(const void* sender)
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
		const FunctionPriorityDelegate* pOtherDelegate = dynamic_cast<const FunctionPriorityDelegate*>(other.unwrap());
		return pOtherDelegate && this->priority() == pOtherDelegate->priority() && _receiverMethod == pOtherDelegate->_receiverMethod;
	}

	AbstractDelegate<void>* clone() const
	{
		return new FunctionPriorityDelegate(*this);
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
	FunctionPriorityDelegate();
};

} // namespace Poco


#endif // Foundation_FunctionPriorityDelegate_INCLUDED
