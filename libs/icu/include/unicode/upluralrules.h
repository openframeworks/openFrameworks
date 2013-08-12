/*
*****************************************************************************************
* Copyright (C) 2010-2012, International Business Machines
* Corporation and others. All Rights Reserved.
*****************************************************************************************
*/

#ifndef UPLURALRULES_H
#define UPLURALRULES_H

#include "unicode/utypes.h"

#if !UCONFIG_NO_FORMATTING

#include "unicode/localpointer.h"

/**
 * \file
 * \brief C API: Plural rules, select plural keywords for numeric values.
 *
 * A UPluralRules object defines rules for mapping non-negative numeric
 * values onto a small set of keywords. Rules are constructed from a text
 * description, consisting of a series of keywords and conditions.
 * The uplrules_select function examines each condition in order and
 * returns the keyword for the first condition that matches the number.
 * If none match, the default rule(other) is returned.
 *
 * For more information, see the LDML spec, C.11 Language Plural Rules:
 * http://www.unicode.org/reports/tr35/#Language_Plural_Rules
 *
 * Keywords: ICU locale data has 6 predefined values -
 * 'zero', 'one', 'two', 'few', 'many' and 'other'. Callers need to check
 * the value of keyword returned by the uplrules_select function.
 *
 * These are based on CLDR <i>Language Plural Rules</i>. For these
 * predefined rules, see the CLDR page at
 * http://unicode.org/repos/cldr-tmp/trunk/diff/supplemental/language_plural_rules.html
 */

/**
 * Opaque UPluralRules object for use in C programs.
 * @stable ICU 4.8
 */
struct UPluralRules;
typedef struct UPluralRules UPluralRules;  /**< C typedef for struct UPluralRules. @stable ICU 4.8 */

/**
 * Open a new UPluralRules object using the predefined plural rules for a
 * given locale.
 * @param locale The locale for which the rules are desired.
 * @param status A pointer to a UErrorCode to receive any errors.
 * @return A UPluralRules for the specified locale, or 0 if an error occurred.
 * @stable ICU 4.8
 */
U_DRAFT UPluralRules* U_EXPORT2
uplrules_open(const char *locale,
             UErrorCode *status);

/**
 * Close a UPluralRules object. Once closed it may no longer be used.
 * @param uplrules The UPluralRules object to close.
 * @stable ICU 4.8
 */
U_DRAFT void U_EXPORT2
uplrules_close(UPluralRules *uplrules);


#if U_SHOW_CPLUSPLUS_API

U_NAMESPACE_BEGIN

/**
 * \class LocalUPluralRulesPointer
 * "Smart pointer" class, closes a UPluralRules via uplrules_close().
 * For most methods see the LocalPointerBase base class.
 *
 * @see LocalPointerBase
 * @see LocalPointer
 * @stable ICU 4.8
 */
U_DEFINE_LOCAL_OPEN_POINTER(LocalUPluralRulesPointer, UPluralRules, uplrules_close);

U_NAMESPACE_END

#endif


/**
 * Given a number, returns the keyword of the first rule that
 * applies to the number, according to the supplied UPluralRules object.
 * @param uplrules The UPluralRules object specifying the rules.
 * @param number The number for which the rule has to be determined.
 * @param keyword The keyword of the rule that applies to number.
 * @param capacity The capacity of keyword.
 * @param status A pointer to a UErrorCode to receive any errors.
 * @return The length of keyword.
 * @stable ICU 4.8
 */
U_DRAFT int32_t U_EXPORT2
uplrules_select(const UPluralRules *uplrules,
               double number,
               UChar *keyword, int32_t capacity,
               UErrorCode *status);

#endif /* #if !UCONFIG_NO_FORMATTING */

#endif
