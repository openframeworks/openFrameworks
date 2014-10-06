//
// DefaultStrategy.h
//
// $Id: //poco/1.4/Foundation/include/Poco/DefaultStrategy.h#3 $
//
// Library: Foundation
// Package: Events
// Module:  DefaultStrategy
//
// Implementation of the DefaultStrategy template.
//
// Copyright (c) 2006-2011, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_DefaultStrategy_INCLUDED
#define Foundation_DefaultStrategy_INCLUDED


#include "Poco/NotificationStrategy.h"
#include "Poco/SharedPtr.h"
#include <vector>


namespace Poco {


template <class TArgs, class TDelegate> 
class DefaultStrategy: public NotificationStrategy<TArgs, TDelegate>
	/// Default notification strategy.
	///
	/// Internally, a std::vector<> is used to store
	/// delegate objects. Delegates are invoked in the
	/// order in which they have been registered.
{
public:
	typedef SharedPtr<TDelegate>         DelegatePtr;
	typedef std::vector<DelegatePtr>     Delegates;
	typedef typename Delegates::iterator Iterator;

public:
	DefaultStrategy()
	{
	}

	DefaultStrategy(const DefaultStrategy& s):
		_delegates(s._delegates)
	{
	}

	~DefaultStrategy()
	{
	}

	void notify(const void* sender, TArgs& arguments)
	{
		for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
		{
			(*it)->notify(sender, arguments);
		}
	}

	void add(const TDelegate& delegate)
	{
		_delegates.push_back(DelegatePtr(static_cast<TDelegate*>(delegate.clone())));
	}

	void remove(const TDelegate& delegate)
	{
		for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
		{
			if (delegate.equals(**it))
			{
				(*it)->disable();
				_delegates.erase(it);
				return;
			}
		}
	}

	DefaultStrategy& operator = (const DefaultStrategy& s)
	{
		if (this != &s)
		{
			_delegates = s._delegates;
		}
		return *this;
	}

	void clear()
	{
		for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
		{
			(*it)->disable();
		}
		_delegates.clear();
	}

	bool empty() const
	{
		return _delegates.empty();
	}

protected:
	Delegates _delegates;
};

template <class TDelegate>
class DefaultStrategy<void,TDelegate>: public NotificationStrategy<void, TDelegate>
	/// Default notification strategy.
	///
	/// Internally, a std::vector<> is used to store
	/// delegate objects. Delegates are invoked in the
	/// order in which they have been registered.
{
public:
	typedef SharedPtr<TDelegate>         DelegatePtr;
	typedef std::vector<DelegatePtr>     Delegates;
	typedef typename Delegates::iterator Iterator;

public:
	DefaultStrategy()
	{
	}

	DefaultStrategy(const DefaultStrategy& s):
		_delegates(s._delegates)
	{
	}

	~DefaultStrategy()
	{
	}

	void notify(const void* sender)
	{
		for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
		{
			(*it)->notify(sender);
		}
	}

	void add(const TDelegate& delegate)
	{
		_delegates.push_back(DelegatePtr(static_cast<TDelegate*>(delegate.clone())));
	}

	void remove(const TDelegate& delegate)
	{
		for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
		{
			if (delegate.equals(**it))
			{
				(*it)->disable();
				_delegates.erase(it);
				return;
			}
		}
	}

	DefaultStrategy& operator = (const DefaultStrategy& s)
	{
		if (this != &s)
		{
			_delegates = s._delegates;
		}
		return *this;
	}

	void clear()
	{
		for (Iterator it = _delegates.begin(); it != _delegates.end(); ++it)
		{
			(*it)->disable();
		}
		_delegates.clear();
	}

	bool empty() const
	{
		return _delegates.empty();
	}

protected:
	Delegates _delegates;
};

} // namespace Poco


#endif // Foundation_DefaultStrategy_INCLUDED
