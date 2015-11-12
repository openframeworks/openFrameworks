//
// JSONRowFormatter.h
//
// $Id: //poco/Main/Data/include/Poco/Data/JSONRowFormatter.h#9 $
//
// Library: Data
// Package: DataCore
// Module:  JSONRowFormatter
//
// Definition of the JSONRowFormatter class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_JSONRowFormatter_INCLUDED
#define Data_JSONRowFormatter_INCLUDED


#include "Poco/Data/RowFormatter.h"


namespace Poco {
namespace Data {


class Data_API JSONRowFormatter: public Poco::Data::RowFormatter
	/// Class for JSON formatting of data rows.
	///
	/// Formatter can be configured to operate in four modes (and
	/// certain combinations thereof) :
	///
	///    - small (condensed mode, only array of values)
	///
	///      Example:
	///      {
	///       [["Simpson", "Bart", "Springfield", 12],
	///       ["Simpson", "Lisa", "Springfield", 10]]
	///      }
	///
	///    - row count (total row count provided)
	///
	///      Example:
	///      {
	///       "count":2,
	///       [["Simpson", "Bart", "Springfield", 12],
	///        ["Simpson", "Lisa", "Springfield", 10]]
	///      }
	///
	///    - column names (column names provided as a string array)
	///
	///      Example:
	///      {
	///       "names":["LastName", "FirstName", "Address", "Age"],
	///       [["Simpson", "Bart", "Springfield", 12],
	///       ["Simpson", "Lisa", "Springfield", 10]]
	///      }
	///
	///    - full (total row count, column names provided in every row of data)
	///
	///      Example:
	///      {
	///       "count":2,
	///        [
	///         {"LastName": "Simpson", "FirstName": "Bart", "Address": "Springfield", "Age": 12},
	///         {"LastName": "Simpson", "FirstName": "Lisa", "Address": "Springfield", "Age": 10}
	///        ]
	///      }
	///
	/// Total row count will be specified by the Poco::DataRecordSet. Note, however, that this is
	/// not possible to do accurately in case of result set paging. For those cases, there is
	/// setTotalRowCount() member function, which allows to explicitly set the total row count.
	/// If the total row count is preset on the formatter, the Data framework shall not interfere.
{
public:
	static const int JSON_FMT_MODE_SMALL        = 1;
	static const int JSON_FMT_MODE_ROW_COUNT    = 2;
	static const int JSON_FMT_MODE_COLUMN_NAMES = 4;
	static const int JSON_FMT_MODE_FULL         = 8;

	JSONRowFormatter(int mode = (JSON_FMT_MODE_COLUMN_NAMES | JSON_FMT_MODE_SMALL));
		/// Creates a new JSONRowFormatter.

	~JSONRowFormatter();
		/// Destroys the JSONRowFormatter.

	std::string& formatNames(const NameVecPtr pNames, std::string& formattedNames);
		/// Formats names.

	std::string& formatValues(const ValueVec& vals, std::string& formattedValues);
		// Formats values.

	void setJSONMode(int mode);
		/// Sets the mode. Valid mode values are:
		///   JSON_FMT_MODE_SMALL
		///   JSON_FMT_MODE_ROW_COUNT
		///   JSON_FMT_MODE_COLUMN_NAMES
		///   JSON_FMT_MODE_FULL

	bool printRowCount();
		/// Returns true if row count printing is enabled,
		/// false otherwise.

	bool printColumnNames();
		/// Returns true if column names printing is enabled,
		/// false otherwise.

	bool isSmall();
		/// Returns true if compact mode formatting is enabled,
		/// false otherwise.

	bool isFull();
		/// Returns true if full mode formatting is enabled,
		/// false otherwise.


private:
	void adjustPrefix();

	NameVecPtr _pNames;
	int        _mode;
	bool       _firstTime;
};


//
// inlines
//


inline bool JSONRowFormatter::printRowCount()
{
	return (_mode & JSON_FMT_MODE_ROW_COUNT) != 0;
}


inline bool JSONRowFormatter::printColumnNames()
{
	return (_mode & JSON_FMT_MODE_COLUMN_NAMES) != 0;
}


inline bool JSONRowFormatter::isSmall()
{
	return (_mode & JSON_FMT_MODE_SMALL) != 0;
}


inline bool JSONRowFormatter::isFull()
{
	return (_mode & JSON_FMT_MODE_FULL) != 0;
}


} } // namespace Poco::Data


#endif // Data_JSONRowFormatter_INCLUDED
