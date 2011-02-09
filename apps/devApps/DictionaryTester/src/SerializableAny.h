#pragma once


#include "Poco/Exception.h"
#include <algorithm>
#include <typeinfo>
#include <ofMain.h>


namespace PocoHacks{
	
	namespace private_impl {
		typedef char yes;
		typedef char (&no)[2];
		
		struct anyx { template <class T> anyx(const T &);};
		
		
		no operator << (const anyx &, const anyx &);
		no operator >> (const anyx &, const anyx &);
		
		
		template <class T> yes checkk(T const&);
		no checkk(no);
		
		template <typename StreamType, typename T>
		struct has_loading_support {
			static StreamType & stream;
			static T & x;
			static const bool value = sizeof(checkk(stream << x)) == sizeof(yes);
		};
		
	};
	
	template<typename T>
	struct supports_ostream : private_impl::has_loading_support<std::ostream, T> {};
	
	template<bool C, typename T = void>
	struct enable_if {
		typedef T type;
	};
	
	template<typename T>
	struct enable_if<false, T> { };
	
	template<typename T>
	typename enable_if<supports_ostream<T>::value, std::string >::type StringCast(const T & t) {
		ostringstream out;
		out << t;
		return out.str();
	}
	
	template<typename T>
	typename enable_if<!supports_ostream<T>::value, std::string >::type StringCast(const T & t) {
		throw Poco::BadCastException("RefAnyCast: Failed to convert between Any types");
	}
	
	
	class SerializableAny
	/// An Any class represents a general type and is capable of storing Any type, supporting type-safe extraction
	/// of the internally stored data.
	///
	/// Code taken from the Boost 1.33.1 library. Original copyright by Kevlin Henney. Modified for Poco
	/// by Applied Informatics.
	{
	public:
		SerializableAny():
		_content(0)
		/// Creates an empty Any type.
		{
		}
		
		template <typename ValueType>
		SerializableAny(const ValueType& _value):_content(new Holder<ValueType>(_value))
		{
			/// Creates an Any which stores the init parameter inside.
			///
			/// Example:
			///     Any a(13);
			///     Any a(string("12345"));
			
		}
		
		SerializableAny(const SerializableAny& other):
		_content(other._content ? other._content->clone() : 0)
		/// Copy constructor, works with empty Anys and initialized Any values.
		{
		}
		
		~SerializableAny()
		{
			delete _content;
		}
		
		SerializableAny& swap(SerializableAny& rhs)
		/// Swaps the content of the two Anys.
		{
			std::swap(_content, rhs._content);
			return *this;
		}
		
		template <typename ValueType>
		SerializableAny& operator = (const ValueType& rhs)
		/// Assignment operator for all types != Any.
		///
		/// Example:
		///    Any a = 13;
		///    Any a = string("12345");
		{
			SerializableAny(rhs).swap(*this);
			return *this;
		}
		
		SerializableAny& operator = (const SerializableAny& rhs)
		/// Assignment operator for Any.
		{
			SerializableAny(rhs).swap(*this);
			return *this;
		}
		
		bool empty() const
		/// returns true if the Any is empty
		{
			return !_content;
		}
		
		const std::type_info& type() const
		/// Returns the type information of the stored content.
		/// If the Any is empty typeid(void) is returned.
		/// It is suggested to always query an Any for its type info before trying to extract
		/// data via an AnyCast/RefAnyCast.
		{
			return _content ? _content->type() : typeid(void);
		}
		
	private:
		
		class Placeholder
		{
		public:
			virtual ~Placeholder()
			{
			}
			
			virtual const std::type_info& type() const = 0;
			virtual Placeholder* clone() const = 0;
			virtual std::string toString() const = 0;
			
		};
		
		template <typename ValueType>
		class Holder: public Placeholder
		{
		public:
			Holder(const ValueType& value):
			_held(value)
			{
			}
			
			virtual const std::type_info& type() const
			{
				return typeid(ValueType);
			}
			
			virtual Placeholder* clone() const
			{
				return new Holder(_held);
			}
			
			std::string toString() const{
				return StringCast(_held);
			}
			
			
			
			ValueType _held;
		};
		
	private:
		template <typename ValueType>
		friend ValueType* AnyCast(SerializableAny*);
		
		template <typename ValueType>
		friend ValueType* UnsafeAnyCast(SerializableAny*);
		
		template<typename ValueType>
		friend ValueType AnyCast(SerializableAny& operand);
		template<typename ValueType>
		friend ValueType AnyCast(const SerializableAny& operand);
		
		Placeholder* _content;
		
	};
	
	
	template <typename ValueType>
	ValueType* AnyCast(SerializableAny* operand)
	/// AnyCast operator used to extract the ValueType from an Any*. Will return a pointer
	/// to the stored value.
	///
	/// Example Usage:
	///     MyType* pTmp = AnyCast<MyType*>(pAny).
	/// Will return NULL if the cast fails, i.e. types don't match.
	{
		return operand && operand->type() == typeid(ValueType)
		? &static_cast<SerializableAny::Holder<ValueType>*>(operand->_content)->_held
		: 0;
	}
	
	
	
	template <typename ValueType>
	const ValueType* AnyCast(const SerializableAny* operand)
	/// AnyCast operator used to extract a const ValueType pointer from an const Any*. Will return a const pointer
	/// to the stored value.
	///
	/// Example Usage:
	///     const MyType* pTmp = AnyCast<MyType*>(pAny).
	/// Will return NULL if the cast fails, i.e. types don't match.
	{
		return AnyCast<ValueType>(const_cast<SerializableAny*>(operand));
	}
	
	
	template <typename ValueType>
	ValueType AnyCast(const SerializableAny& operand)
	/// AnyCast operator used to extract a copy of the ValueType from an const Any&.
	///
	/// Example Usage:
	///     MyType tmp = AnyCast<MyType>(anAny).
	/// Will throw a BadCastException if the cast fails.
	/// Dont use an AnyCast in combination with references, i.e. MyType& tmp = ... or const MyType& = ...
	/// Some compilers will accept this code although a copy is returned. Use the RefAnyCast in
	/// these cases.
	{
		ValueType* result = AnyCast<ValueType>(const_cast<SerializableAny*>(&operand));
		if (!result) throw Poco::BadCastException("Failed to convert between const Any types");
		return *result;
	}
	
	
	template <typename ValueType>
	ValueType AnyCast(SerializableAny& operand)
	/// AnyCast operator used to extract a copy of the ValueType from an Any&.
	///
	/// Example Usage:
	///     MyType tmp = AnyCast<MyType>(anAny).
	/// Will throw a BadCastException if the cast fails.
	/// Dont use an AnyCast in combination with references, i.e. MyType& tmp = ... or const MyType& tmp = ...
	/// Some compilers will accept this code although a copy is returned. Use the RefAnyCast in
	/// these cases.
	{
		ValueType* result = AnyCast<ValueType>(&operand);
		if (!result) throw Poco::BadCastException("Failed to convert between Any types");
		return *result;
	}
	
	template <>
	inline std::string AnyCast(SerializableAny& operand)
	/// AnyCast operator used to extract the ValueType from an Any*. Will return a pointer
	/// to the stored value.
	///
	/// Example Usage:
	///     MyType* pTmp = AnyCast<MyType*>(pAny).
	/// Will return NULL if the cast fails, i.e. types don't match.
	{
		if(!operand.empty()) return operand._content->toString();
		else
			return "";
	}
	
	template <>
	inline std::string AnyCast(const SerializableAny& operand)
	/// AnyCast operator used to extract the ValueType from an Any*. Will return a pointer
	/// to the stored value.
	///
	/// Example Usage:
	///     MyType* pTmp = AnyCast<MyType*>(pAny).
	/// Will return NULL if the cast fails, i.e. types don't match.
	{
		if(!operand.empty()) return operand._content->toString();
		else
			return "";
	}
	
	template <typename ValueType>
	const ValueType& RefAnyCast(const SerializableAny & operand)
	/// AnyCast operator used to return a const reference to the internal data.
	///
	/// Example Usage:
	///     const MyType& tmp = RefAnyCast<MyType>(anAny);
	{
		ValueType* result = AnyCast<ValueType>(const_cast<SerializableAny*>(&operand));
		if (!result) throw Poco::BadCastException("RefAnyCast: Failed to convert between const Any types");
		return *result;
	}
	
	
	template <typename ValueType>
	ValueType& RefAnyCast(SerializableAny& operand)
	/// AnyCast operator used to return a reference to the internal data.
	///
	/// Example Usage:
	///     MyType& tmp = RefAnyCast<MyType>(anAny);
	{
		ValueType* result = AnyCast<ValueType>(&operand);
		if (!result) throw Poco::BadCastException("RefAnyCast: Failed to convert between Any types");
		return *result;
	}
	
	
	template <typename ValueType>
	ValueType* UnsafeAnyCast(SerializableAny* operand)
	/// The "unsafe" versions of AnyCast are not part of the
	/// public interface and may be removed at Any time. They are
	/// required where we know what type is stored in the Any and can't
	/// use typeid() comparison, e.g., when our types may travel across
	/// different shared libraries.
	{
		return &static_cast<SerializableAny::Holder<ValueType>*>(operand->_content)->_held;
	}
	
	
	template <typename ValueType>
	const ValueType* UnsafeAnyCast(const SerializableAny* operand)
	/// The "unsafe" versions of AnyCast are not part of the
	/// public interface and may be removed at Any time. They are
	/// required where we know what type is stored in the Any and can't
	/// use typeid() comparison, e.g., when our types may travel across
	/// different shared libraries.
	{
		return AnyCast<ValueType>(const_cast<SerializableAny*>(operand));
	}
}
