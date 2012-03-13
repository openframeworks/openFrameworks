/* GStreamer Color Balance
 * Copyright (C) 2003 Ronald Bultje <rbultje@ronald.bitfreak.net>
 *
 * color-balance.h: image color balance interface design
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GST_COLOR_BALANCE_H__
#define __GST_COLOR_BALANCE_H__

#include <gst/gst.h>
#include <gst/interfaces/colorbalancechannel.h>
#include <gst/interfaces/interfaces-enumtypes.h>

G_BEGIN_DECLS

#define GST_TYPE_COLOR_BALANCE \
  (gst_color_balance_get_type ())
#define GST_COLOR_BALANCE(obj) \
  (GST_IMPLEMENTS_INTERFACE_CHECK_INSTANCE_CAST ((obj), GST_TYPE_COLOR_BALANCE, \
                                                 GstColorBalance))
#define GST_COLOR_BALANCE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), GST_TYPE_COLOR_BALANCE, \
                            GstColorBalanceClass))
#define GST_IS_COLOR_BALANCE(obj) \
  (GST_IMPLEMENTS_INTERFACE_CHECK_INSTANCE_TYPE ((obj), GST_TYPE_COLOR_BALANCE))
#define GST_IS_COLOR_BALANCE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), GST_TYPE_COLOR_BALANCE))
#define GST_COLOR_BALANCE_GET_CLASS(inst) \
  (G_TYPE_INSTANCE_GET_INTERFACE ((inst), GST_TYPE_COLOR_BALANCE, GstColorBalanceClass))

#define GST_COLOR_BALANCE_TYPE(klass) (klass->balance_type)

typedef struct _GstColorBalance GstColorBalance;
typedef struct _GstColorBalanceClass GstColorBalanceClass;

/**
 * GstColorBalanceType:
 * @GST_COLOR_BALANCE_HARDWARE: Color balance is implemented with dedicated
 *         hardware.
 * @GST_COLOR_BALANCE_SOFTWARE: Color balance is implemented via software
 *         processing.
 *
 * An enumeration indicating whether an element implements color balancing
 * operations in software or in dedicated hardware. In general, dedicated
 * hardware implementations (such as those provided by xvimagesink) are
 * preferred.
 */
typedef enum
{
  GST_COLOR_BALANCE_HARDWARE,
  GST_COLOR_BALANCE_SOFTWARE
} GstColorBalanceType;

struct _GstColorBalanceClass {
  GTypeInterface klass;

  GstColorBalanceType balance_type;

  /* virtual functions */
  const GList * (* list_channels) (GstColorBalance        *balance);

  void          (* set_value)     (GstColorBalance        *balance,
                                   GstColorBalanceChannel *channel,
                                   gint                    value);
  gint          (* get_value)     (GstColorBalance        *balance,
                                   GstColorBalanceChannel *channel);

  /* signals */
  void (* value_changed) (GstColorBalance        *balance,
                          GstColorBalanceChannel *channel,
                          gint                    value);

  gpointer _gst_reserved[GST_PADDING];
};

GType   gst_color_balance_get_type      (void);

/* virtual class function wrappers */
const GList *
        gst_color_balance_list_channels (GstColorBalance        *balance);
void    gst_color_balance_set_value     (GstColorBalance        *balance,
                                         GstColorBalanceChannel *channel,
                                         gint                    value);
gint    gst_color_balance_get_value     (GstColorBalance        *balance,
                                         GstColorBalanceChannel *channel);

GstColorBalanceType
        gst_color_balance_get_balance_type (GstColorBalance        *balance);

/* trigger signal */
void    gst_color_balance_value_changed (GstColorBalance        *balance,
                                         GstColorBalanceChannel *channel,
                                         gint                    value);

G_END_DECLS

#endif /* __GST_COLOR_BALANCE_H__ */
