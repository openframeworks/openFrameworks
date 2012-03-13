


#ifndef __GST_PB_UTILS_ENUM_TYPES_H__
#define __GST_PB_UTILS_ENUM_TYPES_H__

#include <glib-object.h>

G_BEGIN_DECLS

/* enumerations from "install-plugins.h" */
GType gst_install_plugins_return_get_type (void);
#define GST_TYPE_INSTALL_PLUGINS_RETURN (gst_install_plugins_return_get_type())

/* enumerations from "gstdiscoverer.h" */
GType gst_discoverer_result_get_type (void);
#define GST_TYPE_DISCOVERER_RESULT (gst_discoverer_result_get_type())
G_END_DECLS

#endif /* __GST_PB_UTILS_ENUM_TYPES_H__ */



