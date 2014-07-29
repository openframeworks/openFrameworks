//
// ParallelSocketAcceptor.h
//
// $Id: //poco/1.4/Net/include/Poco/Net/ParallelSocketAcceptor.h#1 $
//
// Library: Net
// Package: Reactor
// Module:  ParallelSocketAcceptor
//
// Definition of the ParallelSocketAcceptor class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_ParallelSocketAcceptor_INCLUDED
#define Net_ParallelSocketAcceptor_INCLUDED


#include "Poco/Net/ParallelSocketReactor.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Environment.h"
#include "Poco/NObserver.h"
#include "Poco/SharedPtr.h"
#include <vector>


using Poco::Net::Socket;
using Poco::Net::SocketReactor;
using Poco::Net::ServerSocket;
using Poco::Net::StreamSocket;
using Poco::NObserver;
using Poco::AutoPtr;


namespace Poco {
namespace Net {


template <class ServiceHandler, class SR>
class ParallelSocketAcceptor
	/// This class implements the Acceptor part of the Acceptor-Connector design pattern.
	/// Only the difference from single-threaded version is documented here, For full 
	/// description see Poco::Net::SocketAcceptor documentation.
	/// 
	/// This is a multi-threaded version of SocketAcceptor, it differs from the
	/// single-threaded version in number of reactors (defaulting to number of processors)
	/// that can be specified at construction time and is rotated in a round-robin fashion
	/// by event handler. See ParallelSocketAcceptor::onAccept and 
	/// ParallelSocketAcceptor::createServiceHandler documentation and implementation for 
	/// details.
{
public:
	typedef Poco::Net::ParallelSocketReactor<SR> ParallelReactor;

	explicit ParallelSocketAcceptor(ServerSocket& socket,
		unsigned threads = Poco::Environment::processorCount()):
		_socket(socket),
		_pReactor(0),
		_threads(threads),
		_next(0)
		/// Creates a ParallelSocketAcceptor using the given ServerSocket, 
		/// sets number of threads and populates the reactors vector.
	{
		init();
	}

	ParallelSocketAcceptor(ServerSocket& socket,
		SocketReactor& reactor,
		unsigned threads = Poco::Environment::processorCount()):
		_socket(socket),
		_pReactor(0),
		_threads(threads),
		_next(0)
		/// Creates a ParallelSocketAcceptor using the given ServerSocket, sets the 
		/// number of threads, populates the reactors vector and registers itself 
		/// with the given SocketReactor.
	{
		init();
		registerAcceptor(reactor);
	}

	virtual ~ParallelSocketAcceptor()
		/// Destroys the ParallelSocketAcceptor.
	{
		unregisterAcceptor();
	}
	
	virtual void registerAcceptor(SocketReactor& reactor)
		/// Registers the ParallelSocketAcceptor with a SocketReactor.
		///
		/// A subclass can override this and, for example, also register
		/// an event handler for a timeout event.
		///
		/// The overriding method must call the baseclass implementation first.
	{
		_pReactor = &reactor;
		_pReactor->addEventHandler(_socket,
			Poco::Observer<ParallelSocketAcceptor,
			ReadableNotification>(*this, &ParallelSocketAcceptor::onAccept));
	}
	
	virtual void unregisterAcceptor()
		/// Unregisters the ParallelSocketAcceptor.
		///
		/// A subclass can override this and, for example, also unregister
		/// its event handler for a timeout event.
		///
		/// The overriding method must call the baseclass implementation first.
	{
		_pReactor->removeEventHandler(_socket,
			Poco::Observer<ParallelSocketAcceptor,
			ReadableNotification>(*this, &ParallelSocketAcceptor::onAccept));
	}
	
	void onAccept(ReadableNotification* pNotification)
		/// Accepts connection and creates event handler.
	{
		pNotification->release();
		StreamSocket sock = _socket.acceptConnection();
		createServiceHandler(sock);
	}

protected:
	virtual ServiceHandler* createServiceHandler(StreamSocket& socket)
		/// Create and initialize a new ServiceHandler instance.
		///
		/// Subclasses can override this method.
	{
		std::size_t next = _next++;
		if (_next == _reactors.size()) _next = 0;
		return new ServiceHandler(socket, *_reactors[next]);
	}

	SocketReactor* reactor()
		/// Returns a pointer to the SocketReactor where
		/// this SocketAcceptor is registered.
		///
		/// The pointer may be null.
	{
		return _pReactor;
	}

	Socket& socket()
		/// Returns a reference to the SocketAcceptor's socket.
	{
		return _socket;
	}

	void init()
		/// Populates the reactors vector.
	{
		poco_assert (_threads > 0);

		for (unsigned i = 0; i < _threads; ++i)
			_reactors.push_back(new ParallelReactor);
	}

private:
	typedef std::vector<typename ParallelReactor::Ptr> ReactorVec;

	ParallelSocketAcceptor();
	ParallelSocketAcceptor(const ParallelSocketAcceptor&);
	ParallelSocketAcceptor& operator = (const ParallelSocketAcceptor&);

	ServerSocket   _socket;
	SocketReactor* _pReactor;
	unsigned       _threads;
	ReactorVec     _reactors;
	std::size_t    _next;
};


} } // namespace Poco::Net


#endif // Net_ParallelSocketAcceptor_INCLUDED
