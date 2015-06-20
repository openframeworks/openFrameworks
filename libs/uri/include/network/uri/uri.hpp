// Copyright 2009-2010 Jeroen Habraken.
// Copyright 2009-2013 Dean Michael Berris, Glyn Matthews.
// Copyright 2012 Google, Inc.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NETWORK_URI_INC
#define NETWORK_URI_INC

/**
 * \file
 * \brief Contains the uri the class.
 */

#include <iterator>
#include <algorithm>
#include <functional>
#include <boost/lexical_cast.hpp>
#include <boost/utility/string_ref.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/optional.hpp>
#include <network/uri/config.hpp>
#include <network/uri/uri_errors.hpp>
#include <network/uri/detail/encode.hpp>
#include <network/uri/detail/decode.hpp>
#include <network/uri/detail/translate.hpp>

namespace network {
#if !defined(DOXYGEN_SHOULD_SKIP_THIS)
  namespace detail {
    template <class FwdIter>
    struct hierarchical_part {
      boost::optional<boost::iterator_range<FwdIter> > user_info;
      boost::optional<boost::iterator_range<FwdIter> > host;
      boost::optional<boost::iterator_range<FwdIter> > port;
      boost::optional<boost::iterator_range<FwdIter> > path;
    };

    template <class FwdIter>
    struct uri_parts {
      boost::optional<boost::iterator_range<FwdIter> > scheme;
      hierarchical_part<FwdIter> hier_part;
      boost::optional<boost::iterator_range<FwdIter> > query;
      boost::optional<boost::iterator_range<FwdIter> > fragment;
    };
  }  // namespace detail
#endif // !defined(DOXYGEN_SHOULD_SKIP_THIS)

  /**
   * \enum uri_comparison_level
   * \brief Defines the steps on the URI comparison ladder.
   */
  enum class uri_comparison_level {
    string_comparison,
    syntax_based,
    scheme_based,
  };

  /**
   * \ingroup uri
   * \class uri_builder network/uri/uri_builder.hpp network/uri.hpp
   * \brief A class that allows complex uri objects to be constructed.
   * \sa uri
   */
  class uri_builder;

  /**
   * \ingroup uri
   * \class uri network/uri/uri.hpp network/uri.hpp
   * \brief A class that parses a URI (Uniform Resource Identifier)
   *        into its component parts.
   *
   * This class conforms to a URI as defined by RFC 3986, RFC 3987 and
   * RFC 2732, including scoped IDs. It provides member functions for
   * normalizing, comparing and resolving URIs.
   *
   * A URI has the syntax:
   *
   * \code
   * [scheme:][user_info@][host][:port][path][?query][#fragment]
   * \endcode
   *
   * Example:
   *
   * \code
   * network::uri instance("http://cpp-netlib.org/");
   * assert(instance.is_absolute());
   * assert(!instance.is_opaque());
   * assert(instance.scheme());
   * assert("http" == *instance.scheme());
   * assert(instance.host());
   * assert("cpp-netlib.org" == *instance.host());
   * assert(instance.path());
   * assert("/" == *instance.path());
   * \endcode
   */
  class NETWORK_URI_DECL uri {

#if !defined(DOXYGEN_SHOULD_SKIP_THIS)
    friend class uri_builder;
#endif // !defined(DOXYGEN_SHOULD_SKIP_THIS)

   public:

    /**
     * \brief The URI string_type.
     */
    typedef std::string string_type;

    /**
     * \brief The URI iterator type.
     */
    typedef string_type::const_iterator iterator;
    /**
     * \brief The URI const_iterator type.
     */
    typedef string_type::const_iterator const_iterator;

    /**
     * \brief The URI value_type.
     */
    typedef std::iterator_traits<iterator>::value_type value_type;

    /**
     * \brief A reference to the underlying string_type parts.
     */
    typedef boost::string_ref string_view;

   private:

    void initialize(boost::optional<string_type> scheme,
                    boost::optional<string_type> user_info,
                    boost::optional<string_type> host,
                    boost::optional<string_type> port,
                    boost::optional<string_type> path,
                    boost::optional<string_type> query,
                    boost::optional<string_type> fragment);

   public:

    /**
     * \brief Constructor.
     */
    uri();

    /**
     * \brief Constructor.
     * \param first The first element in a string sequence.
     * \param last The end + 1th element in a string sequence.
     * \throws uri_syntax_error if the sequence is not a valid URI.
     * \throws std::bad_alloc If the underlying string cannot be allocated.
     */
    template <class InputIter>
    uri(InputIter first, InputIter last) {
      if (!initialize(string_type(first, last))) {
        throw uri_syntax_error();
      }
    }

#if !defined(DOXYGEN_SHOULD_SKIP_THIS)
    template <class InputIter>
    explicit uri(InputIter first, InputIter last, std::error_code &ec) {
      if (!initialize(string_type(first, last))) {
        ec = make_error_code(uri_error::invalid_syntax);
      }
    }
#endif // !defined(DOXYGEN_SHOULD_SKIP_THIS)

    /**
     * \brief Constructor.
     * \param source A source string that is to be parsed as a URI.
     * \throws uri_syntax_error if the source is not a valid URI.
     * \throws std::bad_alloc If the underlying string cannot be allocated.
     */
    template <class Source>
    explicit uri(const Source &source) {
      if (!initialize(detail::translate(source))) {
        throw uri_syntax_error();
      }
    }

#if !defined(DOXYGEN_SHOULD_SKIP_THIS)
    template <class Source>
    explicit uri(const Source &source, std::error_code &ec) {
      if (!initialize(detail::translate(source))) {
        ec = make_error_code(uri_error::invalid_syntax);
      }
    }
#endif // !defined(DOXYGEN_SHOULD_SKIP_THIS)

#if !defined(DOXYGEN_SHOULD_SKIP_THIS)
    explicit uri(const uri_builder &builder);
#endif // !defined(DOXYGEN_SHOULD_SKIP_THIS)

    /**
     * \brief Copy constructor.
     * \throws std::bad_alloc If the underlying string cannot be allocated.
     */
    uri(const uri &other);

    /**
     * \brief Move constructor.
     */
    uri(uri &&other) NETWORK_URI_NOEXCEPT;

    /**
     * \brief Destructor.
     */
    ~uri() NETWORK_URI_NOEXCEPT;

    /**
     * \brief Assignment operator.
     * \throws std::bad_alloc If the underlying string cannot be allocated.
     */
    uri &operator=(uri other);

    /**
     * \brief Swaps one uri object with another.
     * \param other The other uri object.
     */
    void swap(uri &other) NETWORK_URI_NOEXCEPT;

    /**
     * \brief Returns an iterator at the first element in the
     *        underlying sequence.
     * \return An iterator starting at the first element.
     */
    const_iterator begin() const;

    /**
     * \brief Returns an iterator at the end + 1th element in the
     *        underlying sequence.
     * \return An iterator starting at the end + 1th element.
     */
    const_iterator end() const;

    /**
     * \brief Returns the URI scheme.
     * \return The scheme, if it exists, or boost::none.
     */
    boost::optional<string_view> scheme() const;

    /**
     * \brief Returns the URI user info.
     * \return The user info, if it exists, or boost::none.
     */
    boost::optional<string_view> user_info() const;

    /**
     * \brief Returns the URI host.
     * \return The host, if it exists, or boost::none.
     */
    boost::optional<string_view> host() const;

    /**
     * \brief Returns the URI port.
     * \return The port, if it exists, or boost::none.
     */
    boost::optional<string_view> port() const;

    /**
     * \brief Returns the URI port as an integer.
     * \return The port, if it exists, or boost::none.
     */
    template <typename IntT>
    boost::optional<IntT> port(typename std::is_integral<IntT>::type * = 0) const {
      if (auto p = port()) {
        try {
          return boost::lexical_cast<IntT>(string_type(std::begin(*p), std::end(*p)));
        }
        catch (boost::bad_lexical_cast &) {
          return boost::optional<IntT>();
        }
      }
      return boost::optional<IntT>();
    }

    /**
     * \brief Returns the URI path.
     * \return The path, if it exists, or boost::none.
     */
    boost::optional<string_view> path() const;

    /**
     * \brief Returns the URI query.
     * \return The query, if it exists, or boost::none.
     */
    boost::optional<string_view> query() const;

    /**
     * \brief Returns the URI fragment.
     * \return The fragment, if it exists, or boost::none.
     */
    boost::optional<string_view> fragment() const;

    /**
     * \brief Returns the URI authority.
     * \return The authority, if it exists, or boost::none.
     */
    boost::optional<string_view> authority() const;

#if !defined(_MSC_VER)
    /**
     * \brief Returns the URI as a std::basic_string object.
     * \return A URI string.
     */
    template <typename CharT, class CharTraits = std::char_traits<CharT>,
              class Alloc = std::allocator<CharT> >
    std::basic_string<CharT, CharTraits, Alloc> to_string(const Alloc &alloc =
                                                              Alloc()) const {
      return std::basic_string<CharT, CharTraits, Alloc>(begin(), end());
    }

#if !defined(DOXYGEN_SHOULD_SKIP_THIS)
    template <typename CharT, class CharTraits = std::char_traits<CharT>,
              class Alloc = std::allocator<CharT> >
    std::basic_string<CharT, CharTraits, Alloc> string(const Alloc &alloc =
                                                           Alloc()) const {
      return to_string<CharT, CharTraits, Alloc>(alloc);
    }
#endif // !defined(DOXYGEN_SHOULD_SKIP_THIS)
#else
#if !defined(DOXYGEN_SHOULD_SKIP_THIS)
    template <typename CharT, class CharTraits, class Alloc>
    std::basic_string<CharT, CharTraits, Alloc> to_string(const Alloc &alloc =
                                                              Alloc()) const {
      return std::basic_string<CharT, CharTraits, Alloc>(begin(), end());
    }

    template <typename CharT, class CharTraits = std::char_traits<CharT>,
              class Alloc = std::allocator<CharT> >
    std::basic_string<CharT, CharTraits, Alloc> string(const Alloc &alloc =
                                                           Alloc()) const {
      return to_string<CharT, CharTraits, Alloc>(alloc);
    }
#endif // !defined(DOXYGEN_SHOULD_SKIP_THIS)
#endif // !/defined(_MSC_VER)

    /**
     * \brief Returns the URI as a std::string object.
     * \returns A URI string.
     */
    std::string string() const;

    /**
     * \brief Returns the URI as a std::wstring object.
     * \returns A URI string.
     */
    std::wstring wstring() const;

    /**
     * \brief Returns the URI as a std::u16string object.
     * \returns A URI string.
     */
    std::u16string u16string() const;

    /**
     * \brief Returns the URI as a std::u32string object.
     * \returns A URI string.
     */
    std::u32string u32string() const;

    /**
     * \brief Checks if the uri object is empty, i.e. it has no parts.
     * \returns \c true if there are no parts, \c false otherwise.
     */
    bool empty() const NETWORK_URI_NOEXCEPT;

    /**
     * \brief Checks if the uri is absolute, i.e. it has a scheme.
     * \returns \c true if it is absolute, \c false if it is relative.
     */
    bool is_absolute() const;

    /**
     * \brief Checks if the uri is opaque, i.e. if it doesn't have an
     *        authority.
     * \returns \c true if it is opaque, \c false if it is hierarchical.
     */
    bool is_opaque() const;

    /**
     * \brief Normalizes a uri object at a given level in the
     *        comparison ladder.
     * \param level The comparison level.
     * \returns A normalized uri.
     * \post compare(normalize(uri, level), level) == 0
     * \throws std::bad_alloc
     */
    uri normalize(uri_comparison_level level) const;

    /**
     * \brief Returns a relative reference against the base URI.
     * \param base The base URI.
     * \returns A relative reference of this URI against the base.
     * \throws std::bad_alloc
     */
    uri make_relative(const uri &base) const;

#if !defined(DOXYGEN_SHOULD_SKIP_THIS)
    uri make_reference(const uri &base) const { return make_relative(base); }
#endif // !defined(DOXYGEN_SHOULD_SKIP_THIS)

    /**
     * \brief Resolves a relative reference against the given URI.
     * \param base The base URI to resolve against.
     * \returns An absolute URI.
     * \throws std::bad_alloc
     */
    uri resolve(const uri &base) const;

    /**
     * \brief Compares this URI against another, corresponding to the
     *        level in the comparison ladder.
     * \param other The other URI.
     * \param level The level in the comparison ladder.
     * \returns \c 0 if the URIs are considered equal, \c -1 if this is
     *         less than other and and 1 if this is greater than
     *         other.
     */
    int compare(const uri &other, uri_comparison_level level) const NETWORK_URI_NOEXCEPT;

    /**
     * \brief Encodes a sequence according to the rules for encoding a
     *        user info part.
     * \param first The iterator at first element in the input
     *        sequence.
     * \param last The iterator at end + 1th element in the input
     *        sequence.
     * \param out The iterator at the first element in the output
     *        sequence.
     * \returns The iterator at the end + 1th in the output sequence.
     */
    template <typename InputIter, typename OutputIter>
    static OutputIter encode_user_info(InputIter first, InputIter last,
                                       OutputIter out) {
      return detail::encode_user_info(first, last, out);
    }

    /**
     * \brief Encodes a sequence according to the rules for encoding a
     *        host part.
     * \param first The iterator at first element in the input
     *        sequence.
     * \param last The iterator at end + 1th element in the input
     *        sequence.
     * \param out The iterator at the first element in the output
     *        sequence.
     * \returns The iterator at the end + 1th in the output sequence.
     */
    template <typename InputIter, typename OutputIter>
    static OutputIter encode_host(InputIter first, InputIter last,
                                  OutputIter out) {
      return detail::encode_host(first, last, out);
    }

    /**
     * \brief Encodes a sequence according to the rules for encoding a
     *        port part.
     * \param first The iterator at first element in the input
     *        sequence.
     * \param last The iterator at end + 1th element in the input
     *        sequence.
     * \param out The iterator at the first element in the output
     *        sequence.
     * \returns The iterator at the end + 1th in the output sequence.
     */
    template <typename InputIter, typename OutputIter>
    static OutputIter encode_port(InputIter first, InputIter last,
                                  OutputIter out) {
      return detail::encode_port(first, last, out);
    }

    /**
     * \brief Encodes a sequence according to the rules for encoding a
     *        path part.
     * \param first The iterator at first element in the input
     *        sequence.
     * \param last The iterator at end + 1th element in the input
     *        sequence.
     * \param out The iterator at the first element in the output
     *        sequence.
     * \returns The iterator at the end + 1th in the output sequence.
     */
    template <typename InputIter, typename OutputIter>
    static OutputIter encode_path(InputIter first, InputIter last,
                                  OutputIter out) {
      return detail::encode_path(first, last, out);
    }

    /**
     * \brief Encodes a sequence according to the rules for encoding a
     *        query part.
     * \param first The iterator at first element in the input
     *        sequence.
     * \param last The iterator at end + 1th element in the input
     *        sequence.
     * \param out The iterator at the first element in the output
     *        sequence.
     * \returns The iterator at the end + 1th in the output sequence.
     */
    template <typename InputIter, typename OutputIter>
    static OutputIter encode_query(InputIter first, InputIter last,
                                   OutputIter out) {
      return detail::encode_query(first, last, out);
    }

    /**
     * \brief Encodes a sequence according to the rules for encoding a
     *        fragment part.
     * \param first The iterator at first element in the input
     *        sequence.
     * \param last The iterator at end + 1th element in the input
     *        sequence.
     * \param out The iterator at the first element in the output
     *        sequence.
     * \returns The iterator at the end + 1th in the output sequence.
     */
    template <typename InputIter, typename OutputIter>
    static OutputIter encode_fragment(InputIter first, InputIter last,
                                      OutputIter out) {
      return detail::encode_fragment(first, last, out);
    }

    /**
     * \brief Decodes a sequence according to the percent decoding
     *        rules.
     * \param first The iterator at first element in the input
     *        sequence.
     * \param last The iterator at end + 1th element in the input
     *        sequence.
     * \param out The iterator at the first element in the output
     *        sequence.
     * \returns The iterator at the end + 1th in the output sequence.
     */
    template <typename InputIter, typename OutputIter>
    static OutputIter decode(InputIter first, InputIter last, OutputIter out) {
      return detail::decode(first, last, out);
    }

   private:

    bool initialize(const string_type &uri);

    string_type uri_;
    detail::uri_parts<string_type::iterator> uri_parts_;
  };

  /**
   * \brief \c uri factory function.
   * \param first The first element in a string sequence.
   * \param last The end + 1th element in a string sequence.
   * \param ec Error code set if the sequence is not a valid URI.
   */
  template <class InputIter>
  inline uri make_uri(InputIter first, InputIter last,
                      std::error_code &ec) {
    return uri(first, last, ec);
  }

  /**
   * \brief \c uri factory function.
   * \param source A source string that is to be parsed as a URI.
   * \param ec Error code set if the source is not a valid URI.
   */
  template <class Source>
  inline uri make_uri(const Source &source,
                      std::error_code &ec) {
    return uri(source, ec);
  }

  /**
   * \brief Swaps one uri object with another.
   */
   void swap(uri &lhs, uri &rhs) NETWORK_URI_NOEXCEPT;

   /**
   * \brief Equality operator for the \c uri.
   */
  bool operator==(const uri &lhs, const uri &rhs) NETWORK_URI_NOEXCEPT;

  /**
   * \brief Equality operator for the \c uri.
   */
  bool operator==(const uri &lhs, const char *rhs) NETWORK_URI_NOEXCEPT;

  /**
   * \brief Equality operator for the \c uri.
   */
  inline bool operator==(const char *lhs, const uri &rhs) NETWORK_URI_NOEXCEPT {
    return rhs == lhs;
  }

  /**
   * \brief Inequality operator for the \c uri.
   */
  inline bool operator!=(const uri &lhs, const uri &rhs) NETWORK_URI_NOEXCEPT {
    return !(lhs == rhs);
  }

  /**
   * \brief Less-than operator for the \c uri.
   */
  bool operator<(const uri &lhs, const uri &rhs) NETWORK_URI_NOEXCEPT ;

  /**
   * \brief Greater-than operator for the \c uri.
   */
  inline bool operator>(const uri &lhs, const uri &rhs) NETWORK_URI_NOEXCEPT {
    return rhs < lhs;
  }

  /**
   * \brief Less-than-or-equal-to operator for the \c uri.
   */
  inline bool operator<=(const uri &lhs, const uri &rhs) NETWORK_URI_NOEXCEPT {
    return !(rhs < lhs);
  }

  /**
   * \brief Greater-than-or-equal-to operator for the \c uri.
   */
  inline bool operator>=(const uri &lhs, const uri &rhs) NETWORK_URI_NOEXCEPT {
    return !(lhs < rhs);
  }
}  // namespace network

#if !defined(DOXYGEN_SHOULD_SKIP_THIS)
namespace std {
  template <>
  struct is_error_code_enum<network::uri_error> : public true_type {};
}  // namespace std

namespace std {
  template <>
  struct hash<network::uri> {

    std::size_t operator()(const network::uri &uri_) const {
      std::size_t seed = 0;
      std::for_each(std::begin(uri_), std::end(uri_),
                    [&seed](network::uri::value_type v) {
        std::hash<network::uri::value_type> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
      });
      return seed;
    }
  };
}  // namespace std
#endif // !defined(DOXYGEN_SHOULD_SKIP_THIS)

#include <network/uri/uri_builder.hpp>

#endif  // NETWORK_URI_INC
