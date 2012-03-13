/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2003, Ximian, Inc.
 */

#ifndef SOUP_TYPES_H
#define SOUP_TYPES_H 1

#include <glib.h>
#include <glib-object.h>

#include <libsoup/soup-status.h>

G_BEGIN_DECLS

typedef struct _SoupAddress           SoupAddress;
typedef struct _SoupAuth              SoupAuth;
typedef struct _SoupAuthDomain        SoupAuthDomain;
typedef struct _SoupCookie            SoupCookie;
typedef struct _SoupCookieJar         SoupCookieJar;
typedef struct _SoupDate              SoupDate;
typedef struct _SoupMessage           SoupMessage;
typedef struct _SoupServer            SoupServer;
typedef struct _SoupSession           SoupSession;
typedef struct _SoupSessionAsync      SoupSessionAsync;
typedef struct _SoupSessionFeature    SoupSessionFeature;
typedef struct _SoupSessionSync       SoupSessionSync;
typedef struct _SoupSocket            SoupSocket;
typedef struct _SoupURI               SoupURI;

/*< private >*/
typedef struct _SoupConnection        SoupConnection;
typedef struct _SoupMessageQueue      SoupMessageQueue;
typedef struct _SoupMessageQueueItem  SoupMessageQueueItem;

G_END_DECLS

#endif
