/*
** Copyright (C) 2008-2011 Erik de Castro Lopo <erikd@mega-nerd.com>
**
** This program is free software ; you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation ; either version 2.1 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY ; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program ; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef SF_SRC_OGG_H

enum
{	OGG_ANNODEX = 300,
	OGG_ANXDATA,
	OGG_FLAC,
	OGG_FLAC0,
	OGG_PCM,
	OGG_SPEEX,
	OGG_VORBIS,
} ;

typedef struct
{	/* Sync and verify incoming physical bitstream */
	ogg_sync_state osync ;
	/* Take physical pages, weld into a logical stream of packets */
	ogg_stream_state ostream ;
	/* One Ogg bitstream page.  Vorbis packets are inside */
	ogg_page opage ;
	/* One raw packet of data for decode */
	ogg_packet opacket ;
	int eos ;
	int codec ;
} OGG_PRIVATE ;


#define readint(buf, base) (((buf [base + 3] << 24) & 0xff000000) | \
								((buf [base + 2] <<16) & 0xff0000) | \
								((buf [base + 1] << 8) & 0xff00) | \
								(buf [base] & 0xff))



#endif /* SF_SRC_OGG_H */
