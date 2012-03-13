


#ifndef __GST_PHOTOGRAPHY_ENUM_TYPES_H__
#define __GST_PHOTOGRAPHY_ENUM_TYPES_H__

#include <glib-object.h>

G_BEGIN_DECLS

/* enumerations from "photography.h" */
GType gst_photography_noise_reduction_get_type (void);
#define GST_TYPE_PHOTOGRAPHY_NOISE_REDUCTION (gst_photography_noise_reduction_get_type())
GType gst_white_balance_mode_get_type (void);
#define GST_TYPE_WHITE_BALANCE_MODE (gst_white_balance_mode_get_type())
GType gst_colour_tone_mode_get_type (void);
#define GST_TYPE_COLOUR_TONE_MODE (gst_colour_tone_mode_get_type())
GType gst_scene_mode_get_type (void);
#define GST_TYPE_SCENE_MODE (gst_scene_mode_get_type())
GType gst_flash_mode_get_type (void);
#define GST_TYPE_FLASH_MODE (gst_flash_mode_get_type())
GType gst_focus_status_get_type (void);
#define GST_TYPE_FOCUS_STATUS (gst_focus_status_get_type())
GType gst_photo_caps_get_type (void);
#define GST_TYPE_PHOTO_CAPS (gst_photo_caps_get_type())
GType gst_photo_shake_risk_get_type (void);
#define GST_TYPE_PHOTO_SHAKE_RISK (gst_photo_shake_risk_get_type())
GType gst_flicker_reduction_mode_get_type (void);
#define GST_TYPE_FLICKER_REDUCTION_MODE (gst_flicker_reduction_mode_get_type())
GType gst_focus_mode_get_type (void);
#define GST_TYPE_FOCUS_MODE (gst_focus_mode_get_type())
G_END_DECLS

#endif /* __GST_PHOTOGRAPHY_ENUM_TYPES_H__ */



