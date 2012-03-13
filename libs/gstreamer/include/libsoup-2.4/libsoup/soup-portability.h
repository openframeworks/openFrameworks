/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2005, Novell, Inc.
 */

#ifndef SOUP_PORTABILITY_H
#define SOUP_PORTABILITY_H

#include <glibconfig.h>

#ifdef G_OS_WIN32

#include <winsock2.h>
#undef interface

#include <ws2tcpip.h>

#else

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

#endif

#endif /* SOUP_PORTABILITY_H */
