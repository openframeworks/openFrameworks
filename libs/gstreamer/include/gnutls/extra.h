/*
 * Copyright (C) 2002, 2003, 2004, 2005, 2007, 2008, 2009, 2010 Free
 * Software Foundation, Inc.
 *
 * Author: Nikos Mavrogiannopoulos
 *
 * This file is part of GnuTLS-EXTRA.
 *
 * GnuTLS-extra is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * GnuTLS-extra is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GnuTLS-EXTRA; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 */

/* Note the libgnutls-extra is not a standalone library. It requires
 * to link also against libgnutls.
 */

#ifndef GNUTLS_EXTRA_H
# define GNUTLS_EXTRA_H

#include <gnutls/gnutls.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define GNUTLS_EXTRA_VERSION GNUTLS_VERSION

  /* TLS/IA stuff
   */

  /**
   * gnutls_ia_apptype_t:
   * @GNUTLS_IA_APPLICATION_PAYLOAD: TLS/IA application payload.
   * @GNUTLS_IA_INTERMEDIATE_PHASE_FINISHED: TLS/IA intermediate phase finished.
   * @GNUTLS_IA_FINAL_PHASE_FINISHED: TLS/IA final phase finished.
   *
   * Enumeration of different certificate encoding formats.
   */
  typedef enum
  {
    GNUTLS_IA_APPLICATION_PAYLOAD = 0,
    GNUTLS_IA_INTERMEDIATE_PHASE_FINISHED = 1,
    GNUTLS_IA_FINAL_PHASE_FINISHED = 2
  } gnutls_ia_apptype_t;

  /* TLS/IA credential
   */

  typedef int (*gnutls_ia_avp_func) (gnutls_session_t session, void *ptr,
				     const char *last, size_t lastlen,
				     char **next, size_t * nextlen);

  typedef struct gnutls_ia_server_credentials_st
    *gnutls_ia_server_credentials_t;
  typedef struct gnutls_ia_client_credentials_st
    *gnutls_ia_client_credentials_t;

  /* Allocate and free TLS/IA credentials. */
  extern void
    gnutls_ia_free_client_credentials (gnutls_ia_client_credentials_t sc);
  extern int
    gnutls_ia_allocate_client_credentials (gnutls_ia_client_credentials_t *
					   sc);

  extern void
    gnutls_ia_free_server_credentials (gnutls_ia_server_credentials_t sc);
  extern int
    gnutls_ia_allocate_server_credentials (gnutls_ia_server_credentials_t *
					   sc);

  /* Client TLS/IA credential functions. */
  extern void
    gnutls_ia_set_client_avp_function (gnutls_ia_client_credentials_t cred,
				       gnutls_ia_avp_func avp_func);
  extern void
    gnutls_ia_set_client_avp_ptr (gnutls_ia_client_credentials_t cred,
				  void *ptr);
  extern void *gnutls_ia_get_client_avp_ptr (gnutls_ia_client_credentials_t
					     cred);

  /* Server TLS/IA credential functions. */
  extern void
    gnutls_ia_set_server_avp_function (gnutls_ia_server_credentials_t cred,
				       gnutls_ia_avp_func avp_func);
  extern void
    gnutls_ia_set_server_avp_ptr (gnutls_ia_server_credentials_t cred,
				  void *ptr);
  extern void *gnutls_ia_get_server_avp_ptr (gnutls_ia_server_credentials_t
					     cred);

  /* TLS/IA handshake. */
  extern int gnutls_ia_handshake_p (gnutls_session_t session);

  extern int gnutls_ia_handshake (gnutls_session_t session);

  /* TLS/IA low level interface. */
  extern int
    gnutls_ia_permute_inner_secret (gnutls_session_t session,
				    size_t session_keys_size,
				    const char *session_keys);
  extern int gnutls_ia_endphase_send (gnutls_session_t session, int final_p);

  extern int gnutls_ia_verify_endphase (gnutls_session_t session,
					const char *checksum);

  extern ssize_t gnutls_ia_send (gnutls_session_t session,
				 const char *data, size_t sizeofdata);
  extern ssize_t gnutls_ia_recv (gnutls_session_t session,
				 char *data, size_t sizeofdata);

  /* Utility stuff. */
  extern int gnutls_ia_generate_challenge (gnutls_session_t session,
					   size_t buffer_size, char *buffer);
  extern void gnutls_ia_extract_inner_secret (gnutls_session_t session,
					      char *buffer);

  /* Define whether inner phases are wanted. */
  extern void gnutls_ia_enable (gnutls_session_t session,
				int allow_skip_on_resume);

  int gnutls_global_init_extra (void);

  /* Use internal (HMAC-)MD5 code, typically for use if libgcrypt is
     in FIPS mode. */
  int gnutls_register_md5_handler (void);

/* returns libgnutls-extra version (call it with a NULL argument) 
 */
  const char *gnutls_extra_check_version (const char *req_version);

#ifdef __cplusplus
}
#endif
#endif
