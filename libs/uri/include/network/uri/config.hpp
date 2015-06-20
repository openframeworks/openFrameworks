// Copyright (c) Glyn Matthews 2012, 2013.
// Copyright 2012 Dean Michael Berris <dberris@google.com>
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef NETWORK_URI_CONFIG_INC
#define NETWORK_URI_CONFIG_INC

#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>

#if defined(BOOST_ALL_DYN_LINK) || defined(BOOST_URI_DYN_LINK)
#define NETWORK_URI_DECL BOOST_SYMBOL_EXPORT
#else
#define NETWORK_URI_DECL
#endif // defined(BOOST_ALL_DYN_LINK) || defined(BOOST_URI_DYN_LINK)

#if defined(BOOST_NO_CXX11_NOEXCEPT)
#define NETWORK_URI_NOEXCEPT throw()
#else
#define NETWORK_URI_NOEXCEPT noexcept
#endif // defined(BOOST_NO_CXX11_NOEXCEPT)

#if !defined(NETWORK_URI_HOST_CONVERTER_BOOST_ASIO)
#define NETWORK_URI_HOST_CONVERTER_BOOST_ASIO 1
#endif // !defined(NETWORK_URI_HOST_CONVERTER_BOOST_ASIO)

#if !defined(NETWORK_URI_HOST_CONVERTER_BOOST_FILESYSTEM)
#define NETWORK_URI_HOST_CONVERTER_BOOST_FILESYSTEM 1
#endif // !defined(NETWORK_URI_HOST_CONVERTER_BOOST_FILESYSTEM)

#endif // NETWORK_URI_CONFIG_INC
