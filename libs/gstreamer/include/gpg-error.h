/* Output of mkheader.awk.  DO NOT EDIT.  -*- buffer-read-only: t -*- */

/* gpg-error.h - Public interface to libgpg-error.
   Copyright (C) 2003, 2004, 2010 g10 Code GmbH

   This file is part of libgpg-error.
 
   libgpg-error is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1 of
   the License, or (at your option) any later version.
 
   libgpg-error is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
 
   You should have received a copy of the GNU Lesser General Public
   License along with this program; if not, see <http://www.gnu.org/licenses/>.
 */


#ifndef GPG_ERROR_H
#define GPG_ERROR_H	1

#include <stddef.h>

#ifdef __GNUC__
#define GPG_ERR_INLINE __inline__
#elif __STDC_VERSION__ >= 199901L
#define GPG_ERR_INLINE inline
#else
#ifndef GPG_ERR_INLINE
#define GPG_ERR_INLINE
#endif 
#endif


#ifdef __cplusplus
extern "C" {
#if 0 /* just to make Emacs auto-indent happy */
}
#endif
#endif /* __cplusplus */

/* The GnuPG project consists of many components.  Error codes are
   exchanged between all components.  The common error codes and their
   user-presentable descriptions are kept into a shared library to
   allow adding new error codes and components without recompiling any
   of the other components.  The interface will not change in a
   backward incompatible way.

   An error code together with an error source build up an error
   value.  As the error value is been passed from one component to
   another, it preserver the information about the source and nature
   of the error.

   A component of the GnuPG project can define the following macros to
   tune the behaviour of the library:

   GPG_ERR_SOURCE_DEFAULT: Define to an error source of type
   gpg_err_source_t to make that source the default for gpg_error().
   Otherwise GPG_ERR_SOURCE_UNKNOWN is used as default.

   GPG_ERR_ENABLE_GETTEXT_MACROS: Define to provide macros to map the
   internal gettext API to standard names.  This has only an effect on
   Windows platforms.  */


/* The error source type gpg_err_source_t.

   Where as the Poo out of a welle small
   Taketh his firste springing and his sours.
					--Chaucer.  */

/* Only use free slots, never change or reorder the existing
   entries.  */
typedef enum
  {
    GPG_ERR_SOURCE_UNKNOWN = 0,
    GPG_ERR_SOURCE_GCRYPT = 1,
    GPG_ERR_SOURCE_GPG = 2,
    GPG_ERR_SOURCE_GPGSM = 3,
    GPG_ERR_SOURCE_GPGAGENT = 4,
    GPG_ERR_SOURCE_PINENTRY = 5,
    GPG_ERR_SOURCE_SCD = 6,
    GPG_ERR_SOURCE_GPGME = 7,
    GPG_ERR_SOURCE_KEYBOX = 8,
    GPG_ERR_SOURCE_KSBA = 9,
    GPG_ERR_SOURCE_DIRMNGR = 10,
    GPG_ERR_SOURCE_GSTI = 11,
    GPG_ERR_SOURCE_GPA = 12,
    GPG_ERR_SOURCE_KLEO = 13,
    GPG_ERR_SOURCE_G13 = 14,
    GPG_ERR_SOURCE_ANY = 31,
    GPG_ERR_SOURCE_USER_1 = 32,
    GPG_ERR_SOURCE_USER_2 = 33,
    GPG_ERR_SOURCE_USER_3 = 34,
    GPG_ERR_SOURCE_USER_4 = 35,

    /* This is one more than the largest allowed entry.  */
    GPG_ERR_SOURCE_DIM = 128
  } gpg_err_source_t;


/* The error code type gpg_err_code_t.  */

/* Only use free slots, never change or reorder the existing
   entries.  */
typedef enum
  {
    GPG_ERR_NO_ERROR = 0,
    GPG_ERR_GENERAL = 1,
    GPG_ERR_UNKNOWN_PACKET = 2,
    GPG_ERR_UNKNOWN_VERSION = 3,
    GPG_ERR_PUBKEY_ALGO = 4,
    GPG_ERR_DIGEST_ALGO = 5,
    GPG_ERR_BAD_PUBKEY = 6,
    GPG_ERR_BAD_SECKEY = 7,
    GPG_ERR_BAD_SIGNATURE = 8,
    GPG_ERR_NO_PUBKEY = 9,
    GPG_ERR_CHECKSUM = 10,
    GPG_ERR_BAD_PASSPHRASE = 11,
    GPG_ERR_CIPHER_ALGO = 12,
    GPG_ERR_KEYRING_OPEN = 13,
    GPG_ERR_INV_PACKET = 14,
    GPG_ERR_INV_ARMOR = 15,
    GPG_ERR_NO_USER_ID = 16,
    GPG_ERR_NO_SECKEY = 17,
    GPG_ERR_WRONG_SECKEY = 18,
    GPG_ERR_BAD_KEY = 19,
    GPG_ERR_COMPR_ALGO = 20,
    GPG_ERR_NO_PRIME = 21,
    GPG_ERR_NO_ENCODING_METHOD = 22,
    GPG_ERR_NO_ENCRYPTION_SCHEME = 23,
    GPG_ERR_NO_SIGNATURE_SCHEME = 24,
    GPG_ERR_INV_ATTR = 25,
    GPG_ERR_NO_VALUE = 26,
    GPG_ERR_NOT_FOUND = 27,
    GPG_ERR_VALUE_NOT_FOUND = 28,
    GPG_ERR_SYNTAX = 29,
    GPG_ERR_BAD_MPI = 30,
    GPG_ERR_INV_PASSPHRASE = 31,
    GPG_ERR_SIG_CLASS = 32,
    GPG_ERR_RESOURCE_LIMIT = 33,
    GPG_ERR_INV_KEYRING = 34,
    GPG_ERR_TRUSTDB = 35,
    GPG_ERR_BAD_CERT = 36,
    GPG_ERR_INV_USER_ID = 37,
    GPG_ERR_UNEXPECTED = 38,
    GPG_ERR_TIME_CONFLICT = 39,
    GPG_ERR_KEYSERVER = 40,
    GPG_ERR_WRONG_PUBKEY_ALGO = 41,
    GPG_ERR_TRIBUTE_TO_D_A = 42,
    GPG_ERR_WEAK_KEY = 43,
    GPG_ERR_INV_KEYLEN = 44,
    GPG_ERR_INV_ARG = 45,
    GPG_ERR_BAD_URI = 46,
    GPG_ERR_INV_URI = 47,
    GPG_ERR_NETWORK = 48,
    GPG_ERR_UNKNOWN_HOST = 49,
    GPG_ERR_SELFTEST_FAILED = 50,
    GPG_ERR_NOT_ENCRYPTED = 51,
    GPG_ERR_NOT_PROCESSED = 52,
    GPG_ERR_UNUSABLE_PUBKEY = 53,
    GPG_ERR_UNUSABLE_SECKEY = 54,
    GPG_ERR_INV_VALUE = 55,
    GPG_ERR_BAD_CERT_CHAIN = 56,
    GPG_ERR_MISSING_CERT = 57,
    GPG_ERR_NO_DATA = 58,
    GPG_ERR_BUG = 59,
    GPG_ERR_NOT_SUPPORTED = 60,
    GPG_ERR_INV_OP = 61,
    GPG_ERR_TIMEOUT = 62,
    GPG_ERR_INTERNAL = 63,
    GPG_ERR_EOF_GCRYPT = 64,
    GPG_ERR_INV_OBJ = 65,
    GPG_ERR_TOO_SHORT = 66,
    GPG_ERR_TOO_LARGE = 67,
    GPG_ERR_NO_OBJ = 68,
    GPG_ERR_NOT_IMPLEMENTED = 69,
    GPG_ERR_CONFLICT = 70,
    GPG_ERR_INV_CIPHER_MODE = 71,
    GPG_ERR_INV_FLAG = 72,
    GPG_ERR_INV_HANDLE = 73,
    GPG_ERR_TRUNCATED = 74,
    GPG_ERR_INCOMPLETE_LINE = 75,
    GPG_ERR_INV_RESPONSE = 76,
    GPG_ERR_NO_AGENT = 77,
    GPG_ERR_AGENT = 78,
    GPG_ERR_INV_DATA = 79,
    GPG_ERR_ASSUAN_SERVER_FAULT = 80,
    GPG_ERR_ASSUAN = 81,
    GPG_ERR_INV_SESSION_KEY = 82,
    GPG_ERR_INV_SEXP = 83,
    GPG_ERR_UNSUPPORTED_ALGORITHM = 84,
    GPG_ERR_NO_PIN_ENTRY = 85,
    GPG_ERR_PIN_ENTRY = 86,
    GPG_ERR_BAD_PIN = 87,
    GPG_ERR_INV_NAME = 88,
    GPG_ERR_BAD_DATA = 89,
    GPG_ERR_INV_PARAMETER = 90,
    GPG_ERR_WRONG_CARD = 91,
    GPG_ERR_NO_DIRMNGR = 92,
    GPG_ERR_DIRMNGR = 93,
    GPG_ERR_CERT_REVOKED = 94,
    GPG_ERR_NO_CRL_KNOWN = 95,
    GPG_ERR_CRL_TOO_OLD = 96,
    GPG_ERR_LINE_TOO_LONG = 97,
    GPG_ERR_NOT_TRUSTED = 98,
    GPG_ERR_CANCELED = 99,
    GPG_ERR_BAD_CA_CERT = 100,
    GPG_ERR_CERT_EXPIRED = 101,
    GPG_ERR_CERT_TOO_YOUNG = 102,
    GPG_ERR_UNSUPPORTED_CERT = 103,
    GPG_ERR_UNKNOWN_SEXP = 104,
    GPG_ERR_UNSUPPORTED_PROTECTION = 105,
    GPG_ERR_CORRUPTED_PROTECTION = 106,
    GPG_ERR_AMBIGUOUS_NAME = 107,
    GPG_ERR_CARD = 108,
    GPG_ERR_CARD_RESET = 109,
    GPG_ERR_CARD_REMOVED = 110,
    GPG_ERR_INV_CARD = 111,
    GPG_ERR_CARD_NOT_PRESENT = 112,
    GPG_ERR_NO_PKCS15_APP = 113,
    GPG_ERR_NOT_CONFIRMED = 114,
    GPG_ERR_CONFIGURATION = 115,
    GPG_ERR_NO_POLICY_MATCH = 116,
    GPG_ERR_INV_INDEX = 117,
    GPG_ERR_INV_ID = 118,
    GPG_ERR_NO_SCDAEMON = 119,
    GPG_ERR_SCDAEMON = 120,
    GPG_ERR_UNSUPPORTED_PROTOCOL = 121,
    GPG_ERR_BAD_PIN_METHOD = 122,
    GPG_ERR_CARD_NOT_INITIALIZED = 123,
    GPG_ERR_UNSUPPORTED_OPERATION = 124,
    GPG_ERR_WRONG_KEY_USAGE = 125,
    GPG_ERR_NOTHING_FOUND = 126,
    GPG_ERR_WRONG_BLOB_TYPE = 127,
    GPG_ERR_MISSING_VALUE = 128,
    GPG_ERR_HARDWARE = 129,
    GPG_ERR_PIN_BLOCKED = 130,
    GPG_ERR_USE_CONDITIONS = 131,
    GPG_ERR_PIN_NOT_SYNCED = 132,
    GPG_ERR_INV_CRL = 133,
    GPG_ERR_BAD_BER = 134,
    GPG_ERR_INV_BER = 135,
    GPG_ERR_ELEMENT_NOT_FOUND = 136,
    GPG_ERR_IDENTIFIER_NOT_FOUND = 137,
    GPG_ERR_INV_TAG = 138,
    GPG_ERR_INV_LENGTH = 139,
    GPG_ERR_INV_KEYINFO = 140,
    GPG_ERR_UNEXPECTED_TAG = 141,
    GPG_ERR_NOT_DER_ENCODED = 142,
    GPG_ERR_NO_CMS_OBJ = 143,
    GPG_ERR_INV_CMS_OBJ = 144,
    GPG_ERR_UNKNOWN_CMS_OBJ = 145,
    GPG_ERR_UNSUPPORTED_CMS_OBJ = 146,
    GPG_ERR_UNSUPPORTED_ENCODING = 147,
    GPG_ERR_UNSUPPORTED_CMS_VERSION = 148,
    GPG_ERR_UNKNOWN_ALGORITHM = 149,
    GPG_ERR_INV_ENGINE = 150,
    GPG_ERR_PUBKEY_NOT_TRUSTED = 151,
    GPG_ERR_DECRYPT_FAILED = 152,
    GPG_ERR_KEY_EXPIRED = 153,
    GPG_ERR_SIG_EXPIRED = 154,
    GPG_ERR_ENCODING_PROBLEM = 155,
    GPG_ERR_INV_STATE = 156,
    GPG_ERR_DUP_VALUE = 157,
    GPG_ERR_MISSING_ACTION = 158,
    GPG_ERR_MODULE_NOT_FOUND = 159,
    GPG_ERR_INV_OID_STRING = 160,
    GPG_ERR_INV_TIME = 161,
    GPG_ERR_INV_CRL_OBJ = 162,
    GPG_ERR_UNSUPPORTED_CRL_VERSION = 163,
    GPG_ERR_INV_CERT_OBJ = 164,
    GPG_ERR_UNKNOWN_NAME = 165,
    GPG_ERR_LOCALE_PROBLEM = 166,
    GPG_ERR_NOT_LOCKED = 167,
    GPG_ERR_PROTOCOL_VIOLATION = 168,
    GPG_ERR_INV_MAC = 169,
    GPG_ERR_INV_REQUEST = 170,
    GPG_ERR_UNKNOWN_EXTN = 171,
    GPG_ERR_UNKNOWN_CRIT_EXTN = 172,
    GPG_ERR_LOCKED = 173,
    GPG_ERR_UNKNOWN_OPTION = 174,
    GPG_ERR_UNKNOWN_COMMAND = 175,
    GPG_ERR_NOT_OPERATIONAL = 176,
    GPG_ERR_NO_PASSPHRASE = 177,
    GPG_ERR_NO_PIN = 178,
    GPG_ERR_NOT_ENABLED = 179,
    GPG_ERR_NO_ENGINE = 180,
    GPG_ERR_MISSING_KEY = 181,
    GPG_ERR_TOO_MANY = 182,
    GPG_ERR_LIMIT_REACHED = 183,
    GPG_ERR_UNFINISHED = 199,
    GPG_ERR_BUFFER_TOO_SHORT = 200,
    GPG_ERR_SEXP_INV_LEN_SPEC = 201,
    GPG_ERR_SEXP_STRING_TOO_LONG = 202,
    GPG_ERR_SEXP_UNMATCHED_PAREN = 203,
    GPG_ERR_SEXP_NOT_CANONICAL = 204,
    GPG_ERR_SEXP_BAD_CHARACTER = 205,
    GPG_ERR_SEXP_BAD_QUOTATION = 206,
    GPG_ERR_SEXP_ZERO_PREFIX = 207,
    GPG_ERR_SEXP_NESTED_DH = 208,
    GPG_ERR_SEXP_UNMATCHED_DH = 209,
    GPG_ERR_SEXP_UNEXPECTED_PUNC = 210,
    GPG_ERR_SEXP_BAD_HEX_CHAR = 211,
    GPG_ERR_SEXP_ODD_HEX_NUMBERS = 212,
    GPG_ERR_SEXP_BAD_OCT_CHAR = 213,
    GPG_ERR_ASS_GENERAL = 257,
    GPG_ERR_ASS_ACCEPT_FAILED = 258,
    GPG_ERR_ASS_CONNECT_FAILED = 259,
    GPG_ERR_ASS_INV_RESPONSE = 260,
    GPG_ERR_ASS_INV_VALUE = 261,
    GPG_ERR_ASS_INCOMPLETE_LINE = 262,
    GPG_ERR_ASS_LINE_TOO_LONG = 263,
    GPG_ERR_ASS_NESTED_COMMANDS = 264,
    GPG_ERR_ASS_NO_DATA_CB = 265,
    GPG_ERR_ASS_NO_INQUIRE_CB = 266,
    GPG_ERR_ASS_NOT_A_SERVER = 267,
    GPG_ERR_ASS_NOT_A_CLIENT = 268,
    GPG_ERR_ASS_SERVER_START = 269,
    GPG_ERR_ASS_READ_ERROR = 270,
    GPG_ERR_ASS_WRITE_ERROR = 271,
    GPG_ERR_ASS_TOO_MUCH_DATA = 273,
    GPG_ERR_ASS_UNEXPECTED_CMD = 274,
    GPG_ERR_ASS_UNKNOWN_CMD = 275,
    GPG_ERR_ASS_SYNTAX = 276,
    GPG_ERR_ASS_CANCELED = 277,
    GPG_ERR_ASS_NO_INPUT = 278,
    GPG_ERR_ASS_NO_OUTPUT = 279,
    GPG_ERR_ASS_PARAMETER = 280,
    GPG_ERR_ASS_UNKNOWN_INQUIRE = 281,
    GPG_ERR_USER_1 = 1024,
    GPG_ERR_USER_2 = 1025,
    GPG_ERR_USER_3 = 1026,
    GPG_ERR_USER_4 = 1027,
    GPG_ERR_USER_5 = 1028,
    GPG_ERR_USER_6 = 1029,
    GPG_ERR_USER_7 = 1030,
    GPG_ERR_USER_8 = 1031,
    GPG_ERR_USER_9 = 1032,
    GPG_ERR_USER_10 = 1033,
    GPG_ERR_USER_11 = 1034,
    GPG_ERR_USER_12 = 1035,
    GPG_ERR_USER_13 = 1036,
    GPG_ERR_USER_14 = 1037,
    GPG_ERR_USER_15 = 1038,
    GPG_ERR_USER_16 = 1039,
    GPG_ERR_MISSING_ERRNO = 16381,
    GPG_ERR_UNKNOWN_ERRNO = 16382,
    GPG_ERR_EOF = 16383,

    /* The following error codes are used to map system errors.  */
#define GPG_ERR_SYSTEM_ERROR	(1 << 15)
    GPG_ERR_E2BIG = GPG_ERR_SYSTEM_ERROR | 0,
    GPG_ERR_EACCES = GPG_ERR_SYSTEM_ERROR | 1,
    GPG_ERR_EADDRINUSE = GPG_ERR_SYSTEM_ERROR | 2,
    GPG_ERR_EADDRNOTAVAIL = GPG_ERR_SYSTEM_ERROR | 3,
    GPG_ERR_EADV = GPG_ERR_SYSTEM_ERROR | 4,
    GPG_ERR_EAFNOSUPPORT = GPG_ERR_SYSTEM_ERROR | 5,
    GPG_ERR_EAGAIN = GPG_ERR_SYSTEM_ERROR | 6,
    GPG_ERR_EALREADY = GPG_ERR_SYSTEM_ERROR | 7,
    GPG_ERR_EAUTH = GPG_ERR_SYSTEM_ERROR | 8,
    GPG_ERR_EBACKGROUND = GPG_ERR_SYSTEM_ERROR | 9,
    GPG_ERR_EBADE = GPG_ERR_SYSTEM_ERROR | 10,
    GPG_ERR_EBADF = GPG_ERR_SYSTEM_ERROR | 11,
    GPG_ERR_EBADFD = GPG_ERR_SYSTEM_ERROR | 12,
    GPG_ERR_EBADMSG = GPG_ERR_SYSTEM_ERROR | 13,
    GPG_ERR_EBADR = GPG_ERR_SYSTEM_ERROR | 14,
    GPG_ERR_EBADRPC = GPG_ERR_SYSTEM_ERROR | 15,
    GPG_ERR_EBADRQC = GPG_ERR_SYSTEM_ERROR | 16,
    GPG_ERR_EBADSLT = GPG_ERR_SYSTEM_ERROR | 17,
    GPG_ERR_EBFONT = GPG_ERR_SYSTEM_ERROR | 18,
    GPG_ERR_EBUSY = GPG_ERR_SYSTEM_ERROR | 19,
    GPG_ERR_ECANCELED = GPG_ERR_SYSTEM_ERROR | 20,
    GPG_ERR_ECHILD = GPG_ERR_SYSTEM_ERROR | 21,
    GPG_ERR_ECHRNG = GPG_ERR_SYSTEM_ERROR | 22,
    GPG_ERR_ECOMM = GPG_ERR_SYSTEM_ERROR | 23,
    GPG_ERR_ECONNABORTED = GPG_ERR_SYSTEM_ERROR | 24,
    GPG_ERR_ECONNREFUSED = GPG_ERR_SYSTEM_ERROR | 25,
    GPG_ERR_ECONNRESET = GPG_ERR_SYSTEM_ERROR | 26,
    GPG_ERR_ED = GPG_ERR_SYSTEM_ERROR | 27,
    GPG_ERR_EDEADLK = GPG_ERR_SYSTEM_ERROR | 28,
    GPG_ERR_EDEADLOCK = GPG_ERR_SYSTEM_ERROR | 29,
    GPG_ERR_EDESTADDRREQ = GPG_ERR_SYSTEM_ERROR | 30,
    GPG_ERR_EDIED = GPG_ERR_SYSTEM_ERROR | 31,
    GPG_ERR_EDOM = GPG_ERR_SYSTEM_ERROR | 32,
    GPG_ERR_EDOTDOT = GPG_ERR_SYSTEM_ERROR | 33,
    GPG_ERR_EDQUOT = GPG_ERR_SYSTEM_ERROR | 34,
    GPG_ERR_EEXIST = GPG_ERR_SYSTEM_ERROR | 35,
    GPG_ERR_EFAULT = GPG_ERR_SYSTEM_ERROR | 36,
    GPG_ERR_EFBIG = GPG_ERR_SYSTEM_ERROR | 37,
    GPG_ERR_EFTYPE = GPG_ERR_SYSTEM_ERROR | 38,
    GPG_ERR_EGRATUITOUS = GPG_ERR_SYSTEM_ERROR | 39,
    GPG_ERR_EGREGIOUS = GPG_ERR_SYSTEM_ERROR | 40,
    GPG_ERR_EHOSTDOWN = GPG_ERR_SYSTEM_ERROR | 41,
    GPG_ERR_EHOSTUNREACH = GPG_ERR_SYSTEM_ERROR | 42,
    GPG_ERR_EIDRM = GPG_ERR_SYSTEM_ERROR | 43,
    GPG_ERR_EIEIO = GPG_ERR_SYSTEM_ERROR | 44,
    GPG_ERR_EILSEQ = GPG_ERR_SYSTEM_ERROR | 45,
    GPG_ERR_EINPROGRESS = GPG_ERR_SYSTEM_ERROR | 46,
    GPG_ERR_EINTR = GPG_ERR_SYSTEM_ERROR | 47,
    GPG_ERR_EINVAL = GPG_ERR_SYSTEM_ERROR | 48,
    GPG_ERR_EIO = GPG_ERR_SYSTEM_ERROR | 49,
    GPG_ERR_EISCONN = GPG_ERR_SYSTEM_ERROR | 50,
    GPG_ERR_EISDIR = GPG_ERR_SYSTEM_ERROR | 51,
    GPG_ERR_EISNAM = GPG_ERR_SYSTEM_ERROR | 52,
    GPG_ERR_EL2HLT = GPG_ERR_SYSTEM_ERROR | 53,
    GPG_ERR_EL2NSYNC = GPG_ERR_SYSTEM_ERROR | 54,
    GPG_ERR_EL3HLT = GPG_ERR_SYSTEM_ERROR | 55,
    GPG_ERR_EL3RST = GPG_ERR_SYSTEM_ERROR | 56,
    GPG_ERR_ELIBACC = GPG_ERR_SYSTEM_ERROR | 57,
    GPG_ERR_ELIBBAD = GPG_ERR_SYSTEM_ERROR | 58,
    GPG_ERR_ELIBEXEC = GPG_ERR_SYSTEM_ERROR | 59,
    GPG_ERR_ELIBMAX = GPG_ERR_SYSTEM_ERROR | 60,
    GPG_ERR_ELIBSCN = GPG_ERR_SYSTEM_ERROR | 61,
    GPG_ERR_ELNRNG = GPG_ERR_SYSTEM_ERROR | 62,
    GPG_ERR_ELOOP = GPG_ERR_SYSTEM_ERROR | 63,
    GPG_ERR_EMEDIUMTYPE = GPG_ERR_SYSTEM_ERROR | 64,
    GPG_ERR_EMFILE = GPG_ERR_SYSTEM_ERROR | 65,
    GPG_ERR_EMLINK = GPG_ERR_SYSTEM_ERROR | 66,
    GPG_ERR_EMSGSIZE = GPG_ERR_SYSTEM_ERROR | 67,
    GPG_ERR_EMULTIHOP = GPG_ERR_SYSTEM_ERROR | 68,
    GPG_ERR_ENAMETOOLONG = GPG_ERR_SYSTEM_ERROR | 69,
    GPG_ERR_ENAVAIL = GPG_ERR_SYSTEM_ERROR | 70,
    GPG_ERR_ENEEDAUTH = GPG_ERR_SYSTEM_ERROR | 71,
    GPG_ERR_ENETDOWN = GPG_ERR_SYSTEM_ERROR | 72,
    GPG_ERR_ENETRESET = GPG_ERR_SYSTEM_ERROR | 73,
    GPG_ERR_ENETUNREACH = GPG_ERR_SYSTEM_ERROR | 74,
    GPG_ERR_ENFILE = GPG_ERR_SYSTEM_ERROR | 75,
    GPG_ERR_ENOANO = GPG_ERR_SYSTEM_ERROR | 76,
    GPG_ERR_ENOBUFS = GPG_ERR_SYSTEM_ERROR | 77,
    GPG_ERR_ENOCSI = GPG_ERR_SYSTEM_ERROR | 78,
    GPG_ERR_ENODATA = GPG_ERR_SYSTEM_ERROR | 79,
    GPG_ERR_ENODEV = GPG_ERR_SYSTEM_ERROR | 80,
    GPG_ERR_ENOENT = GPG_ERR_SYSTEM_ERROR | 81,
    GPG_ERR_ENOEXEC = GPG_ERR_SYSTEM_ERROR | 82,
    GPG_ERR_ENOLCK = GPG_ERR_SYSTEM_ERROR | 83,
    GPG_ERR_ENOLINK = GPG_ERR_SYSTEM_ERROR | 84,
    GPG_ERR_ENOMEDIUM = GPG_ERR_SYSTEM_ERROR | 85,
    GPG_ERR_ENOMEM = GPG_ERR_SYSTEM_ERROR | 86,
    GPG_ERR_ENOMSG = GPG_ERR_SYSTEM_ERROR | 87,
    GPG_ERR_ENONET = GPG_ERR_SYSTEM_ERROR | 88,
    GPG_ERR_ENOPKG = GPG_ERR_SYSTEM_ERROR | 89,
    GPG_ERR_ENOPROTOOPT = GPG_ERR_SYSTEM_ERROR | 90,
    GPG_ERR_ENOSPC = GPG_ERR_SYSTEM_ERROR | 91,
    GPG_ERR_ENOSR = GPG_ERR_SYSTEM_ERROR | 92,
    GPG_ERR_ENOSTR = GPG_ERR_SYSTEM_ERROR | 93,
    GPG_ERR_ENOSYS = GPG_ERR_SYSTEM_ERROR | 94,
    GPG_ERR_ENOTBLK = GPG_ERR_SYSTEM_ERROR | 95,
    GPG_ERR_ENOTCONN = GPG_ERR_SYSTEM_ERROR | 96,
    GPG_ERR_ENOTDIR = GPG_ERR_SYSTEM_ERROR | 97,
    GPG_ERR_ENOTEMPTY = GPG_ERR_SYSTEM_ERROR | 98,
    GPG_ERR_ENOTNAM = GPG_ERR_SYSTEM_ERROR | 99,
    GPG_ERR_ENOTSOCK = GPG_ERR_SYSTEM_ERROR | 100,
    GPG_ERR_ENOTSUP = GPG_ERR_SYSTEM_ERROR | 101,
    GPG_ERR_ENOTTY = GPG_ERR_SYSTEM_ERROR | 102,
    GPG_ERR_ENOTUNIQ = GPG_ERR_SYSTEM_ERROR | 103,
    GPG_ERR_ENXIO = GPG_ERR_SYSTEM_ERROR | 104,
    GPG_ERR_EOPNOTSUPP = GPG_ERR_SYSTEM_ERROR | 105,
    GPG_ERR_EOVERFLOW = GPG_ERR_SYSTEM_ERROR | 106,
    GPG_ERR_EPERM = GPG_ERR_SYSTEM_ERROR | 107,
    GPG_ERR_EPFNOSUPPORT = GPG_ERR_SYSTEM_ERROR | 108,
    GPG_ERR_EPIPE = GPG_ERR_SYSTEM_ERROR | 109,
    GPG_ERR_EPROCLIM = GPG_ERR_SYSTEM_ERROR | 110,
    GPG_ERR_EPROCUNAVAIL = GPG_ERR_SYSTEM_ERROR | 111,
    GPG_ERR_EPROGMISMATCH = GPG_ERR_SYSTEM_ERROR | 112,
    GPG_ERR_EPROGUNAVAIL = GPG_ERR_SYSTEM_ERROR | 113,
    GPG_ERR_EPROTO = GPG_ERR_SYSTEM_ERROR | 114,
    GPG_ERR_EPROTONOSUPPORT = GPG_ERR_SYSTEM_ERROR | 115,
    GPG_ERR_EPROTOTYPE = GPG_ERR_SYSTEM_ERROR | 116,
    GPG_ERR_ERANGE = GPG_ERR_SYSTEM_ERROR | 117,
    GPG_ERR_EREMCHG = GPG_ERR_SYSTEM_ERROR | 118,
    GPG_ERR_EREMOTE = GPG_ERR_SYSTEM_ERROR | 119,
    GPG_ERR_EREMOTEIO = GPG_ERR_SYSTEM_ERROR | 120,
    GPG_ERR_ERESTART = GPG_ERR_SYSTEM_ERROR | 121,
    GPG_ERR_EROFS = GPG_ERR_SYSTEM_ERROR | 122,
    GPG_ERR_ERPCMISMATCH = GPG_ERR_SYSTEM_ERROR | 123,
    GPG_ERR_ESHUTDOWN = GPG_ERR_SYSTEM_ERROR | 124,
    GPG_ERR_ESOCKTNOSUPPORT = GPG_ERR_SYSTEM_ERROR | 125,
    GPG_ERR_ESPIPE = GPG_ERR_SYSTEM_ERROR | 126,
    GPG_ERR_ESRCH = GPG_ERR_SYSTEM_ERROR | 127,
    GPG_ERR_ESRMNT = GPG_ERR_SYSTEM_ERROR | 128,
    GPG_ERR_ESTALE = GPG_ERR_SYSTEM_ERROR | 129,
    GPG_ERR_ESTRPIPE = GPG_ERR_SYSTEM_ERROR | 130,
    GPG_ERR_ETIME = GPG_ERR_SYSTEM_ERROR | 131,
    GPG_ERR_ETIMEDOUT = GPG_ERR_SYSTEM_ERROR | 132,
    GPG_ERR_ETOOMANYREFS = GPG_ERR_SYSTEM_ERROR | 133,
    GPG_ERR_ETXTBSY = GPG_ERR_SYSTEM_ERROR | 134,
    GPG_ERR_EUCLEAN = GPG_ERR_SYSTEM_ERROR | 135,
    GPG_ERR_EUNATCH = GPG_ERR_SYSTEM_ERROR | 136,
    GPG_ERR_EUSERS = GPG_ERR_SYSTEM_ERROR | 137,
    GPG_ERR_EWOULDBLOCK = GPG_ERR_SYSTEM_ERROR | 138,
    GPG_ERR_EXDEV = GPG_ERR_SYSTEM_ERROR | 139,
    GPG_ERR_EXFULL = GPG_ERR_SYSTEM_ERROR | 140,

    /* This is one more than the largest allowed entry.  */
    GPG_ERR_CODE_DIM = 65536
  } gpg_err_code_t;


/* The error value type gpg_error_t.  */

/* We would really like to use bit-fields in a struct, but using
   structs as return values can cause binary compatibility issues, in
   particular if you want to do it effeciently (also see
   -freg-struct-return option to GCC).  */
typedef unsigned int gpg_error_t;

/* We use the lowest 16 bits of gpg_error_t for error codes.  The 16th
   bit indicates system errors.  */
#define GPG_ERR_CODE_MASK	(GPG_ERR_CODE_DIM - 1)

/* Bits 17 to 24 are reserved.  */

/* We use the upper 7 bits of gpg_error_t for error sources.  */
#define GPG_ERR_SOURCE_MASK	(GPG_ERR_SOURCE_DIM - 1)
#define GPG_ERR_SOURCE_SHIFT	24

/* The highest bit is reserved.  It shouldn't be used to prevent
   potential negative numbers when transmitting error values as
   text.  */


/* GCC feature test.  */
#undef _GPG_ERR_HAVE_CONSTRUCTOR
#if __GNUC__
#define _GPG_ERR_GCC_VERSION (__GNUC__ * 10000 \
                            + __GNUC_MINOR__ * 100 \
                            + __GNUC_PATCHLEVEL__)

#if _GPG_ERR_GCC_VERSION > 30100
#define _GPG_ERR_CONSTRUCTOR	__attribute__ ((__constructor__))
#define _GPG_ERR_HAVE_CONSTRUCTOR
#endif
#endif

#ifndef _GPG_ERR_CONSTRUCTOR
#define _GPG_ERR_CONSTRUCTOR
#endif


/* Initialization function.  */

/* Initialize the library.  This function should be run early.  */
gpg_error_t gpg_err_init (void) _GPG_ERR_CONSTRUCTOR;

/* If this is defined, the library is already initialized by the
   constructor and does not need to be initialized explicitely.  */
#undef GPG_ERR_INITIALIZED
#ifdef _GPG_ERR_HAVE_CONSTRUCTOR
#define GPG_ERR_INITIALIZED	1
#endif

/* See the source on how to use the deinit function; it is usually not
   required.  */
void gpg_err_deinit (int mode);


/* Constructor and accessor functions.  */

/* Construct an error value from an error code and source.  Within a
   subsystem, use gpg_error.  */
static GPG_ERR_INLINE gpg_error_t
gpg_err_make (gpg_err_source_t source, gpg_err_code_t code)
{
  return code == GPG_ERR_NO_ERROR ? GPG_ERR_NO_ERROR
    : (((source & GPG_ERR_SOURCE_MASK) << GPG_ERR_SOURCE_SHIFT)
       | (code & GPG_ERR_CODE_MASK));
}


/* The user should define GPG_ERR_SOURCE_DEFAULT before including this
   file to specify a default source for gpg_error.  */
#ifndef GPG_ERR_SOURCE_DEFAULT
#define GPG_ERR_SOURCE_DEFAULT	GPG_ERR_SOURCE_UNKNOWN
#endif

static GPG_ERR_INLINE gpg_error_t
gpg_error (gpg_err_code_t code)
{
  return gpg_err_make (GPG_ERR_SOURCE_DEFAULT, code);
}


/* Retrieve the error code from an error value.  */
static GPG_ERR_INLINE gpg_err_code_t
gpg_err_code (gpg_error_t err)
{
  return (gpg_err_code_t) (err & GPG_ERR_CODE_MASK);
}


/* Retrieve the error source from an error value.  */
static GPG_ERR_INLINE gpg_err_source_t
gpg_err_source (gpg_error_t err)
{
  return (gpg_err_source_t) ((err >> GPG_ERR_SOURCE_SHIFT)
			     & GPG_ERR_SOURCE_MASK);
}


/* String functions.  */

/* Return a pointer to a string containing a description of the error
   code in the error value ERR.  This function is not thread-safe.  */
const char *gpg_strerror (gpg_error_t err);

/* Return the error string for ERR in the user-supplied buffer BUF of
   size BUFLEN.  This function is, in contrast to gpg_strerror,
   thread-safe if a thread-safe strerror_r() function is provided by
   the system.  If the function succeeds, 0 is returned and BUF
   contains the string describing the error.  If the buffer was not
   large enough, ERANGE is returned and BUF contains as much of the
   beginning of the error string as fits into the buffer.  */
int gpg_strerror_r (gpg_error_t err, char *buf, size_t buflen);

/* Return a pointer to a string containing a description of the error
   source in the error value ERR.  */
const char *gpg_strsource (gpg_error_t err);


/* Mapping of system errors (errno).  */

/* Retrieve the error code for the system error ERR.  This returns
   GPG_ERR_UNKNOWN_ERRNO if the system error is not mapped (report
   this). */
gpg_err_code_t gpg_err_code_from_errno (int err);


/* Retrieve the system error for the error code CODE.  This returns 0
   if CODE is not a system error code.  */
int gpg_err_code_to_errno (gpg_err_code_t code);


/* Retrieve the error code directly from the ERRNO variable.  This
   returns GPG_ERR_UNKNOWN_ERRNO if the system error is not mapped
   (report this) and GPG_ERR_MISSING_ERRNO if ERRNO has the value 0. */
gpg_err_code_t gpg_err_code_from_syserror (void);


/* Set the ERRNO variable.  This function is the preferred way to set
   ERRNO due to peculiarities on WindowsCE.  */
void gpg_err_set_errno (int err);

/* Decide whether to use the format_arg attribute.  */
#if _GPG_ERR_GCC_VERSION > 20800
# define _GPG_ERR_ATTR_FORMAT_ARG(a)  __attribute__ ((__format_arg__ (a)))
#else
# define _GPG_ERR_ATTR_FORMAT_ARG(a)
#endif

/* A lean gettext implementation based on GNU style mo files which are
   required to be encoded in UTF-8.  There is a limit on 65534 entries
   to save some RAM.  Only Germanic plural rules are supported.  */
const char *_gpg_w32_bindtextdomain (const char *domainname,
                                     const char *dirname);
const char *_gpg_w32_textdomain (const char *domainname);
const char *_gpg_w32_gettext (const char *msgid)
            _GPG_ERR_ATTR_FORMAT_ARG (1);
const char *_gpg_w32_dgettext (const char *domainname, const char *msgid)
            _GPG_ERR_ATTR_FORMAT_ARG (2);
const char *_gpg_w32_dngettext (const char *domainname, const char *msgid1,
                                const char *msgid2, unsigned long int n)
            _GPG_ERR_ATTR_FORMAT_ARG (2) _GPG_ERR_ATTR_FORMAT_ARG (3);
const char *_gpg_w32_gettext_localename (void);
int _gpg_w32_gettext_use_utf8 (int value);

#ifdef GPG_ERR_ENABLE_GETTEXT_MACROS
# define bindtextdomain(a,b) _gpg_w32_bindtextdomain ((a), (b))
# define textdomain(a)       _gpg_w32_textdomain ((a))
# define gettext(a)          _gpg_w32_gettext ((a))
# define dgettext(a,b)       _gpg_w32_dgettext ((a), (b))
# define ngettext(a,b,c)     _gpg_w32_dngettext (NULL, (a), (b), (c))
# define dngettext(a,b,c,d)  _gpg_w32_dngettext ((a), (b), (c), (d))
# define gettext_localename() _gpg_w32_gettext_localename ()
# define gettext_use_utf8(a) _gpg_w32_gettext_use_utf8 (a)
#endif /*GPG_ERR_ENABLE_GETTEXT_MACROS*/



/* Self-documenting convenience functions.  */

static GPG_ERR_INLINE gpg_error_t
gpg_err_make_from_errno (gpg_err_source_t source, int err)
{
  return gpg_err_make (source, gpg_err_code_from_errno (err));
}


static GPG_ERR_INLINE gpg_error_t
gpg_error_from_errno (int err)
{
  return gpg_error (gpg_err_code_from_errno (err));
}

static GPG_ERR_INLINE gpg_error_t
gpg_error_from_syserror (void)
{
  return gpg_error (gpg_err_code_from_syserror ());
}

#ifdef __cplusplus
}
#endif


#endif	/* GPG_ERROR_H */
