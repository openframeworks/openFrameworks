// Copyright 2013 Glyn Matthews.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef NETWORK_URI_ERRORS_INC
#define NETWORK_URI_ERRORS_INC

/**
 * \file
 * \brief Contains functions and exceptions for URI error handling.
 */

#include <system_error>
#include <network/uri/config.hpp>

namespace network {

#if !defined(DOXYGEN_SHOULD_SKIP_THIS)
  enum class uri_error {

    // parser errors
    invalid_syntax = 1,

    // builder errors
    invalid_uri,
    invalid_scheme,
    invalid_user_info,
    invalid_host,
    invalid_port,
    invalid_path,
    invalid_query,
    invalid_fragment,

    // encoding errors
    not_enough_input,
    non_hex_input,
    conversion_failed,
  };

  const std::error_category &uri_category();

  std::error_code make_error_code(uri_error e);
#endif // !defined(DOXYGEN_SHOULD_SKIP_THIS)

  /**
   * \class uri_syntax_error uri.hpp network/uri.hpp
   * \brief An exception thrown by the \c uri constructor when a URI
   *        cannot be parsed.
   */
  class uri_syntax_error : public std::system_error {

  public:

    /**
     * \brief Constructor.
     */
    uri_syntax_error();

    /**
     * \brief Destructor.
     */
    virtual ~uri_syntax_error() NETWORK_URI_NOEXCEPT;

  };

  /**
   * \class uri_builder_error uri.hpp network/uri.hpp
   * \brief An exception thrown when the \c uri_builder cannot build a
   *        valid URI.
   */
  class uri_builder_error : public std::system_error {

  public:

    /**
     * \brief Constructor.
     */
    uri_builder_error();

    /**
     * \brief Destructor.
     */
    virtual ~uri_builder_error() NETWORK_URI_NOEXCEPT;

  };

  /**
   * \class percent_decoding_error uri.hpp network/uri.hpp
   * \brief An exception thrown when during percent decoding.
   */
  class percent_decoding_error : public std::system_error {

  public:

    /**
     * \brief Constructor.
     */
    explicit percent_decoding_error(uri_error error);

    /**
     * \brief Destructor.
     */
    virtual ~percent_decoding_error() NETWORK_URI_NOEXCEPT;

  };
} // namespace network


#endif // NETWORK_URI_ERRORS_INC
