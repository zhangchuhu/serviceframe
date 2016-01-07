/// \file coldata.h
/// \brief Declares classes for converting string data to any of 
/// the basic C types.
///
/// Roughly speaking, this defines classes that are the inverse of
/// mysqlpp::SQLString.

/***********************************************************************
 Copyright (c) 1998 by Kevin Atkinson, (c) 1999, 2000 and 2001 by
 MySQL AB, and (c) 2004-2007 by Educational Technology Resources, Inc.
 Others may also hold copyrights on code in this file.  See the CREDITS
 file in the top directory of the distribution for details.

 This file is part of MySQL++.

 MySQL++ is free software; you can redistribute it and/or modify it
 under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 MySQL++ is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with MySQL++; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
 USA
***********************************************************************/

#ifndef MYSQLPP_COLDATA_H
#define MYSQLPP_COLDATA_H

#include "common.h"

#include "const_string.h"
#include "convert.h"
#include "exceptions.h"
#include "null.h"
#include "string_util.h"
#include "type_info.h"

#include <typeinfo>
#include <string>
#include <sstream>

#include <stdlib.h>

namespace mysqlpp {

/// \brief Template for string data that can convert itself to any
/// standard C data type.
///
/// Do not use this class directly. Use the typedef ColData or
/// MutableColData instead. ColData is a \c ColData_Tmpl<const
/// \c std::string> and MutableColData is a
/// \c ColData_Tmpl<std::string>.
///
/// The ColData types add to the C++ string type the ability to
/// automatically convert the string data to any of the basic C types.
/// This is important with SQL, because all data coming from the
/// database is in string form.  MySQL++ uses this class internally
/// to hold the data it receives from the server, so you can use it
/// naturally, because it does the conversions implicitly:
///
/// \code ColData("12.86") + 2.0 \endcode
///
/// That works fine, but be careful.  If you had said this instead:
/// 
/// \code ColData("12.86") + 2 \endcode
/// 
/// the result would be 14 because 2 is an integer, and C++'s type
/// conversion rules put the ColData object in an integer context.
///
/// If these automatic conversions scare you, define the macro
/// NO_BINARY_OPERS to disable this behavior.
///
/// This class also has some basic information about the type of data
/// stored in it, to allow it to do the conversions more intelligently
/// than a trivial implementation would allow.

template <class Str>
class MYSQLPP_EXPORT ColData_Tmpl : public Str
{
public:
	/// \brief Default constructor
	///
	/// Null flag is set to false, type data is not set, and string
	/// data is left empty.
	///
	/// It's probably a bad idea to use this ctor, becuase there's no
	/// way to set the type data once the object's constructed.
	ColData_Tmpl() :
	null_(false)
	{
	}

	/// \brief Copy ctor
	///
	/// \param cd the other ColData_Tmpl object
	ColData_Tmpl(const ColData_Tmpl<Str>& cd) :
	Str(cd.data(), cd.length()),
	type_(cd.type_),
	null_(cd.null_)
	{
	}

	/// \brief Constructor allowing you to set the null flag and the
	/// type data.
	///
	/// \param n if true, data is a SQL null
	/// \param t MySQL type information for data being stored
	explicit ColData_Tmpl(bool n,
			mysql_type_info t = mysql_type_info::string_type) :
	type_(t),
	null_(n)
	{
	}

	/// \brief C++ string version of full ctor
	///
	/// \param str the string this object represents
	/// \param t MySQL type information for data within str
	/// \param n if true, str is a SQL null
	explicit ColData_Tmpl(const std::string& str,
			mysql_type_info t = mysql_type_info::string_type,
			bool n = false) :
	Str(str),
	type_(t),
	null_(n)
	{
	}

	/// \brief Null-terminated C string version of full ctor
	///
	/// \param str the string this object represents
	/// \param t MySQL type information for data within str
	/// \param n if true, str is a SQL null
	explicit ColData_Tmpl(const char* str,
			mysql_type_info t = mysql_type_info::string_type,
			bool n = false) :
	Str(str),
	type_(t),
	null_(n)
	{
	}

	/// \brief Full constructor.
	///
	/// \param str the string this object represents
	/// \param len the length of the string; embedded nulls are legal
	/// \param t MySQL type information for data within str
	/// \param n if true, str is a SQL null
	explicit ColData_Tmpl(const char* str, typename Str::size_type len,
			mysql_type_info t = mysql_type_info::string_type,
			bool n = false) :
	Str(str, len),
	type_(t),
	null_(n)
	{
	}

	/// \brief Get this object's current MySQL type.
	mysql_type_info type() const { return type_; }

	/// \brief Returns true if data of this type should be quoted, false
	/// otherwise.
	bool quote_q() const { return type_.quote_q(); }

	/// \brief Returns true if data of this type should be escaped, false
	/// otherwise.
	bool escape_q() const { return type_.escape_q(); }
	
	/// \brief Template for converting data from one type to another.
	template <class Type> Type conv(Type dummy) const;

	/// \brief Set a flag indicating that this object is a SQL null.
	void it_is_null() { null_ = true; }

	/// \brief Returns true if this object is a SQL null.
	inline bool is_null() const { return null_; }
	
	/// \brief Returns this object's data in C++ string form.
	///
	/// This method is inefficient, and not recommended.  It makes a
	/// duplicate copy of the string that lives as long as the
	/// \c ColData object itself.
	///
	/// If you are using the \c MutableColData typedef for this
	/// template, you can avoid the duplicate copy entirely.  You can
	/// pass a \c MutableColData object to anything expecting a
	/// \c std::string and get the right result.  (This didn't work
	/// reliably prior to v2.3.)
	///
	/// This method is arguably useful with plain \c ColData objects,
	/// but there are more efficient alternatives.  If you know your
	/// data is a null-terminated C string, just cast this object to
	/// a \c const \c char* or call the \c data() method.  This gives
	/// you a pointer to our internal buffer, so the copy isn't needed.
	/// If the \c ColData can contain embedded null characters, you do
	/// need to make a copy, but it's better to make your own copy of 
	/// the string, instead of calling get_string(), so you can better
	/// control its lifetime:
	///
	/// \code
	/// ColData cd = ...;
	/// std::string s(cd.data(), cd.length());
	/// \endcode
	inline const std::string& get_string() const
	{
		temp_buf_.assign(Str::data(), Str::length());
		return temp_buf_;
	}

	/// \brief Returns a const char pointer to the object's raw data
	operator cchar*() const { return Str::data(); }
	
	/// \brief Converts this object's string data to a signed char
	operator signed char() const
			{ return conv(static_cast<signed char>(0)); }
	
	/// \brief Converts this object's string data to an unsigned char
	operator unsigned char() const
			{ return conv(static_cast<unsigned char>(0)); }
	
	/// \brief Converts this object's string data to an int
	operator int() const
			{ return conv(static_cast<int>(0)); }
	
	/// \brief Converts this object's string data to an unsigned int
	operator unsigned int() const
			{ return conv(static_cast<unsigned int>(0)); }
	
	/// \brief Converts this object's string data to a short int
	operator short int() const
			{ return conv(static_cast<short int>(0)); }
	
	/// \brief Converts this object's string data to an unsigned short
	/// int
	operator unsigned short int() const
			{ return conv(static_cast<unsigned short int>(0)); }
	
	/// \brief Converts this object's string data to a long int
	operator long int() const
			{ return conv(static_cast<long int>(0)); }
	
	/// \brief Converts this object's string data to an unsigned long
	/// int
	operator unsigned long int() const
			{ return conv(static_cast<unsigned long int>(0)); }
	
#if !defined(NO_LONG_LONGS)
	/// \brief Converts this object's string data to the platform-
	/// specific 'longlong' type, usually a 64-bit integer.
	operator longlong() const
			{ return conv(static_cast<longlong>(0)); }
	
	/// \brief Converts this object's string data to the platform-
	/// specific 'ulonglong' type, usually a 64-bit unsigned integer.
	operator ulonglong() const
			{ return conv(static_cast<ulonglong>(0)); }
#endif
	
	/// \brief Converts this object's string data to a float
	operator float() const
			{ return conv(static_cast<float>(0)); }
	
	/// \brief Converts this object's string data to a double
	operator double() const
			{ return conv(static_cast<double>(0)); }
	
	/// \brief Converts this object's string data to a bool
	operator bool() const { return conv(0); }

	template <class T, class B> operator Null<T, B>() const;

private:
	mysql_type_info type_;
	mutable std::string temp_buf_;	
	bool null_;
};

/// \typedef ColData_Tmpl<const_string> ColData
/// \brief The type that is returned by constant rows
typedef ColData_Tmpl<const_string> ColData;

/// \typedef ColData_Tmpl<std::string> MutableColData
/// \brief The type that is returned by mutable rows
typedef ColData_Tmpl<std::string> MutableColData;


#if !defined(NO_BINARY_OPERS) && !defined(DOXYGEN_IGNORE)
// Ignore this section is NO_BINARY_OPERS is defined, or if this section
// is being parsed by Doxygen.  In the latter case, it's ignored because
// Doxygen doesn't understand it correctly, and we can't be bothered to
// explain it to Doxygen.

#define oprsw(opr, other, conv) \
  template<class Str> \
  inline other operator opr (ColData_Tmpl<Str> x, other y) \
    {return static_cast<conv>(x) opr y;} \
  template<class Str> \
  inline other operator opr (other x, ColData_Tmpl<Str> y) \
    {return x opr static_cast<conv>(y);}

#define operator_binary(other, conv) \
  oprsw(+, other, conv) \
  oprsw(-, other, conv) \
  oprsw(*, other, conv) \
  oprsw(/, other, conv)

#define operator_binary_int(other, conv) \
  operator_binary(other, conv) \
  oprsw(%, other, conv) \
  oprsw(&, other, conv) \
  oprsw(^, other, conv) \
  oprsw(|, other, conv) \
  oprsw(<<, other, conv) \
  oprsw(>>, other, conv)

operator_binary(float, double)
operator_binary(double, double)

operator_binary_int(char, long int)
operator_binary_int(int, long int)
operator_binary_int(short int, long int)
operator_binary_int(long int, long int)

operator_binary_int(unsigned char, unsigned long int)
operator_binary_int(unsigned int, unsigned long int)
operator_binary_int(unsigned short int, unsigned long int)
operator_binary_int(unsigned long int, unsigned long int)

#if !defined(NO_LONG_LONGS)
operator_binary_int(longlong, longlong)
operator_binary_int(ulonglong, ulonglong)
#endif
#endif // NO_BINARY_OPERS

/// \brief Converts this object to a SQL null
///
/// Returns a copy of the global null object if the string data held by
/// the object is exactly equal to "NULL".  Else, it constructs an empty
/// object of type T and tries to convert it to Null<T, B>.
template <class Str> template<class T, class B>
ColData_Tmpl<Str>::operator Null<T, B>() const
{
	if ((Str::size() == 4) &&
			(*this)[0] == 'N' &&
			(*this)[1] == 'U' &&
			(*this)[2] == 'L' &&
			(*this)[3] == 'L') {
		return Null<T, B>(null);
	}
	else {
		return Null<T, B>(conv(T()));
	}
}

template <class Str> template <class Type>
Type ColData_Tmpl<Str>::conv(Type /* dummy */) const
{
	std::string strbuf(Str::data(), Str::length());
	strip_all_blanks(strbuf);
	std::string::size_type len = strbuf.size();
	const char* str = strbuf.c_str();
	const char* end = str;
	Type num = mysql_convert<Type>(str, end);

	if (*end == '.') {
		++end;
		for (; *end == '0'; ++end) ;
	}
	
	if (*end != '\0' && end != 0) {
		throw BadConversion(typeid(Type).name(), Str::c_str(),
				end - str, len);
	}

	return num;
}

} // end namespace mysqlpp

#endif
