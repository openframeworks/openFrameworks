// Copyright (c) Glyn Matthews 2011, 2012, 2013, 2014.
// Copyright 2012 Dean Michael Berris <dberris@google.com>
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_URI_ENCODE_INC
#define NETWORK_URI_ENCODE_INC

#include <network/uri/uri_errors.hpp>
#include <iterator>
#include <cstring>
#include <cassert>
#include <algorithm>

namespace network {
  namespace detail {
    template <
      typename CharT
      >
    inline
    CharT hex_to_letter(CharT in) {
      if ((in >= 0) && (in < 10)) {
	return in + '0';
      }

      if ((in >= 10) && (in < 16)) {
	return in - 10 + 'A';
      }

      return in;
    }

    template <
      typename CharT,
      class OutputIterator
      >
    void encode_char(CharT in, OutputIterator &out, const char *ignore = "") {
      if (((in >= 'a') && (in <= 'z')) ||
	  ((in >= 'A') && (in <= 'Z')) ||
	  ((in >= '0') && (in <= '9')) ||
	  (in == '-') ||
	  (in == '.') ||
	  (in == '_') ||
	  (in == '~')) {
	out++ = in;
      }
      else {
	auto first = ignore, last = ignore + std::strlen(ignore);
	if (std::find(first, last, in) != last) {
	  out++ = in;
	}
	else {
	  out++ = '%';
	  out++ = hex_to_letter((in >> 4) & 0x0f);
	  out++ = hex_to_letter(in & 0x0f);
	}
      }
    }

    template <
      typename InputIterator,
      typename OutputIterator
      >
    OutputIterator encode_user_info(InputIterator first,
				    InputIterator last,
				    OutputIterator out) {
      auto it = first;
      while (it != last) {
	detail::encode_char(*it, out, ":");
	++it;
      }
      return out;
    }

    template <
      typename InputIterator,
      typename OutputIterator
      >
    OutputIterator encode_host(InputIterator first,
			       InputIterator last,
			       OutputIterator out) {
      auto it = first;
      while (it != last) {
	detail::encode_char(*it, out, "[:]");
	++it;
      }
      return out;
    }

    template <
      typename InputIterator,
      typename OutputIterator
      >
    OutputIterator encode_port(InputIterator first,
			       InputIterator last,
			       OutputIterator out) {
      auto it = first;
      while (it != last) {
	detail::encode_char(*it, out);
	++it;
      }
      return out;
    }

    template <
      typename InputIterator,
      typename OutputIterator
      >
    OutputIterator encode_path(InputIterator first,
			       InputIterator last,
			       OutputIterator out) {
      auto it = first;
      while (it != last) {
	detail::encode_char(*it, out, "/.@%;=");
	++it;
      }
      return out;
    }

    template <
      typename InputIterator,
      typename OutputIterator
      >
    OutputIterator encode_query(InputIterator first,
				InputIterator last,
				OutputIterator out) {
      auto it = first;
      while (it != last) {
	detail::encode_char(*it, out, "/.@&%;=");
	++it;
      }
      return out;
    }

    template <
      typename InputIterator,
      typename OutputIterator
      >
    OutputIterator encode_fragment(InputIterator first,
				   InputIterator last,
				   OutputIterator out) {
      auto it = first;
      while (it != last) {
	detail::encode_char(*it, out, "/.@&l;=%");
	++it;
      }
      return out;
    }

    template <class String>
    String encode_user_info(const String &user_info) {
      String encoded;
      encode_user_info(std::begin(user_info), std::end(user_info),
		       std::back_inserter(encoded));
      return encoded;
    }

    template <class String>
    String encode_host(const String &host) {
      String encoded;
      encode_host(std::begin(host), std::end(host),
		  std::back_inserter(encoded));
      return encoded;
    }

    template <class String>
    String encode_port(const String &port) {
      String encoded;
      encode_port(std::begin(port), std::end(port),
		  std::back_inserter(encoded));
      return encoded;
    }

    template <class String>
    String encode_path(const String &path) {
      String encoded;
      encode_path(std::begin(path), std::end(path),
		  std::back_inserter(encoded));
      return encoded;
    }

    template <class String>
    String encode_query(const String &query) {
      String encoded;
      encode_query(std::begin(query), std::end(query),
		   std::back_inserter(encoded));
      return encoded;
    }

    template <class String>
    String encode_fragment(const String &fragment) {
      String encoded;
      encode_fragment(std::begin(fragment), std::end(fragment),
		      std::back_inserter(encoded));
      return encoded;
    }
  } // namespace detail
} // namespace network

#endif // NETWORK_URI_ENCODE_INC
