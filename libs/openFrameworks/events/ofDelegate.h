#include "Poco/AbstractPriorityDelegate.h"

class ofEventAttendedException: public Poco::Exception{

};

template <class TObj, class TArgs, bool useSender = true>
class ofDelegate: public Poco::AbstractPriorityDelegate<TArgs>
{
public:
	typedef bool (TObj::*NotifyMethod)(const void*, TArgs&);

	ofDelegate(TObj* obj, NotifyMethod method, int prio):
		Poco::AbstractPriorityDelegate<TArgs>(prio),
		_receiverObject(obj),
		_receiverMethod(method)
	{
	}

	ofDelegate(const ofDelegate& delegate):
		Poco::AbstractPriorityDelegate<TArgs>(delegate),
		_receiverObject(delegate._receiverObject),
		_receiverMethod(delegate._receiverMethod)
	{
	}

	ofDelegate& operator = (const ofDelegate& delegate)
	{
		if (&delegate != this)
		{
			this->_pTarget        = delegate._pTarget;
			this->_receiverObject = delegate._receiverObject;
			this->_receiverMethod = delegate._receiverMethod;
			this->_priority       = delegate._priority;
		}
		return *this;
	}

	~ofDelegate()
	{
	}

	bool notify(const void* sender, TArgs& arguments)
	{
		Poco::Mutex::ScopedLock lock(_mutex);
		if (_receiverObject)
		{
			if(	!(_receiverObject->*_receiverMethod)(sender, arguments))
				return true;
			else
				throw ofEventAttendedException();
		}
		else return false;
	}

	bool equals(const Poco::AbstractDelegate<TArgs>& other) const
	{
		const ofDelegate* pOtherDelegate = dynamic_cast<const ofDelegate*>(other.unwrap());
		return pOtherDelegate && this->priority() == pOtherDelegate->priority() && _receiverObject == pOtherDelegate->_receiverObject && _receiverMethod == pOtherDelegate->_receiverMethod;
	}

	Poco::AbstractDelegate<TArgs>* clone() const
	{
		return new ofDelegate(*this);
	}

	void disable()
	{
		Poco::Mutex::ScopedLock lock(_mutex);
		_receiverObject = 0;
	}

protected:
	TObj*        _receiverObject;
	NotifyMethod _receiverMethod;
	Poco::Mutex _mutex;

private:
	ofDelegate();
};


template <class TObj, class TArgs>
class ofDelegate<TObj, TArgs, false>: public Poco::AbstractPriorityDelegate<TArgs>
{
public:
	typedef bool (TObj::*NotifyMethod)(TArgs&);

	ofDelegate(TObj* obj, NotifyMethod method, int prio):
		Poco::AbstractPriorityDelegate<TArgs>(prio),
		_receiverObject(obj),
		_receiverMethod(method)
	{
	}

	ofDelegate(const ofDelegate& delegate):
		Poco::AbstractPriorityDelegate<TArgs>(delegate),
		_receiverObject(delegate._receiverObject),
		_receiverMethod(delegate._receiverMethod)
	{
	}

	ofDelegate& operator = (const ofDelegate& delegate)
	{
		if (&delegate != this)
		{
			this->_pTarget        = delegate._pTarget;
			this->_receiverObject = delegate._receiverObject;
			this->_receiverMethod = delegate._receiverMethod;
			this->_priority       = delegate._priority;
		}
		return *this;
	}

	~ofDelegate()
	{
	}

	bool notify(const void* sender, TArgs& arguments)
	{
		Poco::Mutex::ScopedLock lock(_mutex);
		if (_receiverObject)
		{
			if(	!(_receiverObject->*_receiverMethod)(arguments))
				return true;
			else
				throw ofEventAttendedException();
		}
		return false;
	}

	bool equals(const Poco::AbstractDelegate<TArgs>& other) const
	{
		const ofDelegate* pOtherDelegate = dynamic_cast<const ofDelegate*>(other.unwrap());
		return pOtherDelegate && this->priority() == pOtherDelegate->priority() && _receiverObject == pOtherDelegate->_receiverObject && _receiverMethod == pOtherDelegate->_receiverMethod;
	}

	Poco::AbstractDelegate<TArgs>* clone() const
	{
		return new ofDelegate(*this);
	}

	void disable()
	{
		Poco::Mutex::ScopedLock lock(_mutex);
		_receiverObject = 0;
	}

protected:
	TObj*        _receiverObject;
	NotifyMethod _receiverMethod;
	Poco::Mutex _mutex;

private:
	ofDelegate();
};


template <class TObj>
class ofDelegate<TObj, void, true>: public Poco::AbstractPriorityDelegate<void>
{
public:
	typedef bool (TObj::*NotifyMethod)(const void*);

	ofDelegate(TObj* obj, NotifyMethod method, int prio):
		Poco::AbstractPriorityDelegate<void>(prio),
		_receiverObject(obj),
		_receiverMethod(method)
	{
	}

	ofDelegate(const ofDelegate& delegate):
		Poco::AbstractPriorityDelegate<void>(delegate),
		_receiverObject(delegate._receiverObject),
		_receiverMethod(delegate._receiverMethod)
	{
	}

	ofDelegate& operator = (const ofDelegate& delegate)
	{
		if (&delegate != this)
		{
			this->_pTarget        = delegate._pTarget;
			this->_receiverObject = delegate._receiverObject;
			this->_receiverMethod = delegate._receiverMethod;
			this->_priority       = delegate._priority;
		}
		return *this;
	}

	~ofDelegate()
	{
	}

	bool notify(const void* sender)
	{
		Poco::Mutex::ScopedLock lock(_mutex);
		if (_receiverObject)
		{
			if(	!(_receiverObject->*_receiverMethod)(sender))
				return true;
			else
				throw ofEventAttendedException();
		}
		else return false;
	}

	bool equals(const Poco::AbstractDelegate<void>& other) const
	{
		const ofDelegate* pOtherDelegate = dynamic_cast<const ofDelegate*>(other.unwrap());
		return pOtherDelegate && this->priority() == pOtherDelegate->priority() && _receiverObject == pOtherDelegate->_receiverObject && _receiverMethod == pOtherDelegate->_receiverMethod;
	}

	Poco::AbstractDelegate<void>* clone() const
	{
		return new ofDelegate(*this);
	}

	void disable()
	{
		Poco::Mutex::ScopedLock lock(_mutex);
		_receiverObject = 0;
	}

protected:
	TObj*        _receiverObject;
	NotifyMethod _receiverMethod;
	Poco::Mutex _mutex;

private:
	ofDelegate();
};

template <class TObj>
class ofDelegate<TObj, void, false>: public Poco::AbstractPriorityDelegate<void>
{
public:
	typedef void (TObj::*NotifyMethod)();

	ofDelegate(TObj* obj, NotifyMethod method, int prio):
		Poco::AbstractPriorityDelegate<void>(prio),
		_receiverObject(obj),
		_receiverMethod(method)
	{
	}

	ofDelegate(const ofDelegate& delegate):
		Poco::AbstractPriorityDelegate<void>(delegate),
		_receiverObject(delegate._receiverObject),
		_receiverMethod(delegate._receiverMethod)
	{
	}

	ofDelegate& operator = (const ofDelegate& delegate)
	{
		if (&delegate != this)
		{
			this->_pTarget        = delegate._pTarget;
			this->_receiverObject = delegate._receiverObject;
			this->_receiverMethod = delegate._receiverMethod;
			this->_priority       = delegate._priority;
		}
		return *this;
	}

	~ofDelegate()
	{
	}

	bool notify(const void* sender)
	{
		Poco::Mutex::ScopedLock lock(_mutex);
		if (_receiverObject)
		{
			if(	!(_receiverObject->*_receiverMethod)())
				return true;
			else
				throw ofEventAttendedException();
		}
		return false;
	}

	bool equals(const Poco::AbstractDelegate<void>& other) const
	{
		const ofDelegate* pOtherDelegate = dynamic_cast<const ofDelegate*>(other.unwrap());
		return pOtherDelegate && this->priority() == pOtherDelegate->priority() && _receiverObject == pOtherDelegate->_receiverObject && _receiverMethod == pOtherDelegate->_receiverMethod;
	}

	Poco::AbstractDelegate<void>* clone() const
	{
		return new ofDelegate(*this);
	}

	void disable()
	{
		Poco::Mutex::ScopedLock lock(_mutex);
		_receiverObject = 0;
	}

protected:
	TObj*        _receiverObject;
	NotifyMethod _receiverMethod;
	Poco::Mutex _mutex;

private:
	ofDelegate();
};
