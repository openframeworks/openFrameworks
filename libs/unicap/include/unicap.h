/*
    unicap
    Copyright (C) 2004  Arne Caspari

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __UNICAP_H__
#define __UNICAP_H__

#ifdef __cplusplus
#define UNICAP_BEGIN_DECLS extern "C" {
#define UNICAP_END_DECLS }
#else
#define UNICAP_BEGIN_DECLS
#define UNICAP_END_DECLS
#endif

#include <sys/types.h>
#include <sys/time.h>

#include <unistd.h>

#include <unicap_status.h>
#include <unicap_version.h>


/**
 * unicap_buffer_type_t:
 * @UNICAP_BUFFER_TYPE_USER: buffer is provided by the application
 * @UNICAP_BUFFER_TYPE_SYSTEM: buffer is provided by the driver or library
 */
typedef enum
{
	UNICAP_BUFFER_TYPE_USER = 0, 
	UNICAP_BUFFER_TYPE_SYSTEM,
} unicap_buffer_type_t;


typedef enum
{
   UNICAP_FLAGS_BUFFER_TYPE_USER   = 1,      
   UNICAP_FLAGS_BUFFER_TYPE_SYSTEM = 1 << 1,
   
   UNICAP_FLAGS_BUFFER_LOCKED      = 1 << 16,
} unicap_buffer_flags_t;

/**
 * unicap_device_t:
 * @identifier: unique textual if of a device
 * @model_name: model name of the device
 * @vendor_name: name of the device vendor
 * @model_id: unique model id, ie. serial number
 * @vendor_id: ID of the vendor like USB Vendor ID for example
 * @cpi_layer: name of the plugin used to communicate with the device
 * @device: name of the device file, if any
 * @flags: 
 */
struct unicap_device_t
{
	char identifier[128];
	char model_name[128];
	char vendor_name[128];
		
	unsigned long long model_id;
	unsigned int vendor_id;
	
	char cpi_layer[1024];
	char device[1024];
		
	unsigned int flags;
};


typedef struct unicap_device_t unicap_device_t;

/**
 * unicap_rect_t:
 * @x: 
 * @y: 
 * @width: 
 * @height:
 *
 * A struct defining a rectangle
 */
struct unicap_rect_t
{
	int x;
	int y;	
	int width;
	int height;
};

typedef struct unicap_rect_t unicap_rect_t;

/**
 * unicap_format_t:
 * @identifier: unique textual identifier of this format
 * @size: size of the format
 * @min_size: minimum size
 * @size: maximum size
 * @h_stepping: horizontal stepping
 * @v_stepping: vertical stepping
 * @sizes: array of allowed sizes. Might be NULL
 * @size_count: number of element in the sizes array
 * @bpp: bits per pixel
 * @fourcc: FOURCC describing the colour format
 * @flags: 
 * @buffer_types:
 * @system_buffer_count: 
 * @buffer_size: amount of memory required by one data buffer of this
 * format
 * @buffer_type: 
 * 
 */
struct unicap_format_t
{
      char identifier[128];
      
      // default
      unicap_rect_t size;
      
      // min and max extends
      unicap_rect_t min_size;
      unicap_rect_t max_size;
      
      // stepping: 
      // 0 if no free scaling available
      int h_stepping;
      int v_stepping;
      // array of possible sizes
      unicap_rect_t *sizes;
      int size_count;
      
      int bpp;
      unsigned int fourcc;
      unsigned int flags;

      unsigned int buffer_types; // available buffer types
      int system_buffer_count;

      size_t buffer_size;

      unicap_buffer_type_t buffer_type;
};

typedef struct unicap_format_t unicap_format_t;

/**
 * unicap_data_buffer_t:
 * @format: a #unicap_format_t describing the layout of the data
 * buffer
 * @frame_number:
 * @fill_time: the time when capturing completed on this buffer
 * @duration: frame duration of this buffer ( might not be present on
 * all devices )
 * @capture_start_time: the time when the capturing started on this
 * buffer ( might not be supported on all devices )
 * @data: the image data
 * @buffer_size: size of the memory buffer pointed to by data
 * @type: 
 * 
 */
struct unicap_data_buffer_t
{
      unicap_format_t format;
      
      int frame_number;
      struct timeval fill_time; //
      struct timeval duration;
      struct timeval capture_start_time;// optional, time of first copy action of image data of this frame
      
      unsigned char *data;
      size_t buffer_size;	

      unicap_buffer_type_t type; // system pointers or user pointers

      unicap_buffer_flags_t flags;
      unsigned int reserved[7];		
};

typedef struct unicap_data_buffer_t unicap_data_buffer_t;


/**
 * unicap_property_range_t:
 * @min: minimum value
 * @max: maximum value
 */
struct unicap_property_range_t
{
	double min;
	double max;
};

typedef struct unicap_property_range_t unicap_property_range_t;

/**
 * unicap_property_value_list_t:
 * @values: array of values
 * @value_count: number of elements in values array
 * 
 */
struct unicap_property_value_list_t
{
	double *values;
	int value_count;	
};

typedef struct unicap_property_value_list_t unicap_property_value_list_t;

/**
 * unicap_property_menu_t:
 * @menu_items: an array of menu items
 * @menu_item_count: number of elements in the menu_items array
 *
 */
struct unicap_property_menu_t
{
	char **menu_items;
	int menu_item_count;
};

typedef struct unicap_property_menu_t unicap_property_menu_t;

/**
 * unicap_property_type_enum_t:
 * @UNICAP_PROPERTY_TYPE_RANGE: a property with a value in a given
 * range
 * @UNICAP_PROPERTY_TYPE_VALUE_LIST: a property with a value out of a
 * list of values
 * @UNICAP_PROPERTY_TYPE_MENU: 
 * @UNICAP_PROPERTY_TYPE_DATA:
 * @UNICAP_PROPERTY_TYPE_FLAGS: a property where only the flags are valid
 */
typedef enum 
{
	UNICAP_PROPERTY_TYPE_RANGE = 0,
	UNICAP_PROPERTY_TYPE_VALUE_LIST,
	UNICAP_PROPERTY_TYPE_MENU,
	UNICAP_PROPERTY_TYPE_DATA,
	UNICAP_PROPERTY_TYPE_FLAGS, 

	UNICAP_PROPERTY_TYPE_UNKNOWN
} unicap_property_type_enum_t;


typedef enum
{
   UNICAP_FLAGS_MANUAL              = 1ULL, 
   UNICAP_FLAGS_AUTO                = (1ULL<<1ULL),
   UNICAP_FLAGS_ONE_PUSH            = (1ULL<<2ULL),
// property may be read during auto
   UNICAP_FLAGS_READ_OUT            = (1ULL<<3ULL),
   UNICAP_FLAGS_ON_OFF              = (1ULL<<4ULL),
   UNICAP_FLAGS_READ_ONLY           = (1ULL<<5ULL),
   UNICAP_FLAGS_FORMAT_CHANGE       = (1ULL<<6ULL),

// "system" flags
   UNICAP_FLAGS_CHECK_STEPPING      = (1ULL<<32ULL),
   
   UNICAP_FLAGS_DUMMY_VALUE         = (0xffffffffffffffffULL)
} unicap_property_flags_t;
   
   

/**
 * unicap_property_t:
 * 
 * FIXME: gtk-doc fails to parse structs with anonymous unions. 
 * 
 * <informalexample>
 *  <programlisting>

      char identifier[128]; //mandatory
      char category[128];
      char unit[128]; // 
      
      // list of properties identifier which value / behaviour may change if this property changes
      char **relations;
      int relations_count;	
      
      union
      {
	    double value; // default if enumerated
	    char menu_item[128]; };
      
      
      union{	
	    unicap_property_range_t range; // if UNICAP_USE_RANGE is asserted
	    unicap_property_value_list_t value_list; // if UNICAP_USE_VALUE_LIST is asserted
	    unicap_property_menu_t menu; };
	    
      
      double stepping;
      
      unicap_property_type_enum_t type;	
      u_int64_t flags;        // defaults if enumerated
      u_int64_t flags_mask;	// defines capabilities if enumerated
      
      // optional data
      void *property_data; 
      size_t property_data_size;	


 * identifier:   unique textual identifier of this properties
 * category:     a category for this property, for example: 'Lens Control' for zoom and focus properties
 * unit:         optional unit, for example: 's' for Exposure
 * denoting the Exposure time in seconds
 * relations:    properties that might their state or value when
 * changing this property
 * relations_count: size of the relations array
 * value:           for UNICAP_PROPERTY_TYPE_RANGE and
 * UNICAP_PROPERTY_TYPE_VALUE_LIST properties: current value
 * menu_item:       for UNICAP_PROPERTY_TYPE_MENU properties: selected menu entry
 * range:           for UNICAP_PROPERTY_TYPE_RANGE properties: valid range for value
 * value_list:      for UNICAP_PROPERTY_TYPE_VALUE_LIST properties: list
 * of valid values
 * menu:            for UNICAP_PROPERTY_TYPE_MENU properties: menu
 * stepping:        for UNICAP_PROPERTY_TYPE_RANGE properties: stepping 
 * type: 
 * flags: when enumerated, this field contains the 
 * flags_mask:
 * property_data:
 * property_data_size:
 *  </programlisting>
 * </informalexample>
 * 
 */
struct unicap_property_t
{
      char identifier[128]; //mandatory
      char category[128];
      char unit[128]; // 
      
      // list of properties identifier which value / behaviour may change if this property changes
      char **relations;
      int relations_count;	
      
      union
      {
	    double value; // default if enumerated
	    char menu_item[128]; 
      };
      
      
      union{	
	    unicap_property_range_t range; // if UNICAP_USE_RANGE is asserted
	    unicap_property_value_list_t value_list; // if UNICAP_USE_VALUE_LIST is asserted
	    unicap_property_menu_t menu; 
      };
	    
      
      double stepping;
      
      unicap_property_type_enum_t type;	
      unicap_property_flags_t flags;        // defaults if enumerated
      unicap_property_flags_t flags_mask;	// defines capabilities if enumerated
      
      // optional data
      void *property_data; 
      size_t property_data_size;	
};

typedef struct unicap_property_t unicap_property_t;




typedef enum 
{
	UNICAP_EVENT_FIRST = 0,
	UNICAP_EVENT_DEVICE_REMOVED = 0, 
	UNICAP_EVENT_NEW_DEVICE,
	UNICAP_EVENT_NEW_FRAME, 
	UNICAP_EVENT_DROP_FRAME, 
	UNICAP_EVENT_LAST
} unicap_event_t;

typedef struct _unicap_handle *unicap_handle_t;

typedef void (*unicap_callback_t)( unicap_event_t event, ... );
typedef void (*unicap_new_frame_callback_t)( unicap_event_t event, unicap_handle_t handle, unicap_data_buffer_t *buffer, void *user_ptr );
typedef void (*unicap_drop_frame_callback_t)( unicap_event_t event, unicap_handle_t handle, void *user_ptr );
typedef void (*unicap_new_device_callback_t)( unicap_event_t event, unicap_device_t *device, void *user_ptr );

UNICAP_BEGIN_DECLS

// Functions without a handle

/**
 * unicap_check_version
 * @major: major version to check against
 * @minor: minor version to check against
 * @micro: micro version to check against
 *
 * Checks that the unicap library version is compatible with given
 * version
 *
 * Returns: TRUE if compatible
 */
unicap_status_t unicap_check_version( unsigned int major, unsigned int minor, unsigned int micro );
   
	

/**
 * unicap_reenumerate_devices:
 * @count: if not NULL, receives number of devices found
 * 
 * Rebuild internal list of devices.
 *
 * Returns: status
 */
unicap_status_t unicap_reenumerate_devices( int *count );

/**
 * unicap_enumerate_devices: 
 * @specifier: specifies which devices should be returned, or NULL
 * @device: receives the device
 * @index: 
 * 
 * Enumerates currently connected video capture devices
 * 
 * Returns: status
 */
unicap_status_t unicap_enumerate_devices( unicap_device_t *specifier, unicap_device_t *device, int index );

/**
 * unicap_open:
 * @handle: receives the new handle
 * @device: device to open, as returned by #unicap_enumerate_devices
 * 
 * Acquire a handle to a device. 
 *
 * Returns: status
 */
unicap_status_t unicap_open( unicap_handle_t *handle, unicap_device_t *device );

/**
 * unicap_register_callback:
 * @handle: 
 * @event: 
 * @callback: 
 * @user_ptr: user provided data that gets passed to the callback function
 * 
 */
unicap_status_t unicap_register_callback( unicap_handle_t handle, 
					  unicap_event_t event, 
					  unicap_callback_t func, 
					  void *user_ptr );


/**
 * unicap_unregister_callback:
 * @handle:
 * @event:
 *
 * Returns: status
 */
unicap_status_t unicap_unregister_callback( unicap_handle_t handle, 
					    unicap_event_t event );


/**
 * unicap_close: 
 * @handle: 
 * 
 * Clsoing a handle decrements the reference count on the device. If
 * the reference count is 0, all resources associated with the device
 * get freed.
 * 
 * Returns: status
 */
unicap_status_t unicap_close( unicap_handle_t handle );

/** 
 * unicap_get_device:
 * @handle:
 * @device: 
 * 
 * Gets the device controled by handle
 * 
 * Returns: status
 */
unicap_status_t unicap_get_device( unicap_handle_t handle, unicap_device_t *device );	

/**
 * unicap_clone_handle: 
 * @old_handle: 
 * 
 * Copies the handle, increment the reference count
 * 
 * Returns: new handle
 */
unicap_handle_t unicap_clone_handle( unicap_handle_t old_handle );

/**
 * unicap_reenumerate_formats: 
 * @handle: 
 * @count: Receives the number of formats currently supported by the device. Might be NULL
 * 
 * Re-create the list of formats supported by the device. Invalidates all data returned by prior calls 
 * to unicap_emumerate_formats() and unicap_get_format(). 
 *
 * Returns: status
 */
unicap_status_t unicap_reenumerate_formats( unicap_handle_t handle, 
					    int *count );

/**
 * unicap_enumerate_formats:
 * @handle:
 * @specifier: limits the enumerated formats to the ones matching the fields in specifier. Fields set to -1 in the specifier are ignored
 * @format: 
 * @index: index of the format in the enumeration
 * 
 * Enumerate formats known to the device 
 *
 * Returns: STATUS_NO_MORE_FORMATS: end of the list of matching formats has been reached
 */
unicap_status_t unicap_enumerate_formats( unicap_handle_t handle, unicap_format_t *specifier, unicap_format_t *format, int index );

/**
 * unicap_set_format:
 * @handle:
 * @format:
 * 
 * Set a format. 
 * 
 * Returns: STATUS_NO_MATCH: given format not valid for device
 */
unicap_status_t unicap_set_format( unicap_handle_t handle, unicap_format_t *format );

/**
 * unicap_get_format:
 * @handle:
 * @format:
 * 
 * Get the current format
 * 
 * Returns: status
*/
unicap_status_t unicap_get_format( unicap_handle_t handle, unicap_format_t *format );

/**
 * unicap_reenumerate_properties:
 * @handle:
 * @count: receives number of properties supported by the device. Might be NULL
 * 
 * Re-create the list of properties supported by the device. This invalidates all data returned by prior calls to 
 * unicap_enumerate_properties() and unicap_get_property()
 *
 * Returns: status
 */
unicap_status_t unicap_reenumerate_properties( unicap_handle_t handle, int *count );

/**
 * unicap_enumerate_properties:
 * @handle:
 * @specifier:
 * @property: 
 * @index: 
 * 
 * Enumerate properties matching "specifier"
 * 
 * Returns: status
 */
unicap_status_t unicap_enumerate_properties( unicap_handle_t handle, unicap_property_t *specifier, unicap_property_t *property, int index );

/**
 * unicap_set_property:
 * @handle:
 * @property:
 * 
 * Set a device property
 * 
 * Returns: status
 */
unicap_status_t unicap_set_property( unicap_handle_t handle, unicap_property_t *property );

/**
 * unicap_set_property_value: Sets the value of a property
 * @handle:
 * @identifier: identifier of the property to set
 * @value: new value
 * 
 * Set a RANGE or VALUE_LIST property
 * 
 * Returns: status
 */
unicap_status_t unicap_set_property_value( unicap_handle_t handle, char *identifier, double value );

/**
 * unicap_set_property_manual: 
 * @handle:
 * @identifier: identifier of the property to set
 * 
 * Sets a property to manual mode, disabling the automatic mode
 *
 * Returns: status
 */
unicap_status_t unicap_set_property_manual( unicap_handle_t handle, char *identifier );

/**
 * unicap_set_property_auto: 
 * @handle:
 * @identifier: identifier of the property to set
 * 
 * Sets the property to automatic mode
 *
 * Returns: status
 */
unicap_status_t unicap_set_property_auto( unicap_handle_t handle, char *identifier );

/**
 * unicap_set_property_auto: 
 * @handle:
 * @identifier: identifier of the property to set
 * 
 * Enable one push mode on a property
 *
 * Returns: status
 */
unicap_status_t unicap_set_property_one_push( unicap_handle_t handle, char *identifier );

/**
 * unicap_get_property:
 * @handle: 
 * @property: 
 * 
 * Get a device property
 * 
 * Returns: status
 */
unicap_status_t unicap_get_property( unicap_handle_t handle, unicap_property_t *property );

/**
 * unicap_get_property_value: 
 * @handle: 
 * @identifier: identifier of the property to query
 * 
 * Returns the value of a property
 * 
 * Returns: status
 */
unicap_status_t unicap_get_property_value( unicap_handle_t handle, const char *identifier, double *value );

/**
 * unicap_get_property_menu:
 * @handle:
 * @identifier: identifier of the property to query
 * @value: pointer to store the string location
 * 
 * Returns the menu string of a property. Property must be of
 * UNICAP_PROPERTY_TYPE_MENU type. The returned string is owned by the
 * unicap library and might be overwritten by subsequent calls to unicap_get_property_menu.
 * 
 * Returns: status
 */
unicap_status_t unicap_get_property_menu( unicap_handle_t handle, const char *identifier, char **value );

/**
 * unicap_get_property_auto:
 * @handle:
 * @identifier: identifier of the property to query
 * @enabled: location to store the enabled flag
 * 
 * Returns whether the auto mode of a property is enabled.
 */
unicap_status_t unicap_get_property_auto( unicap_handle_t handle, const char *identifier, int *enabled );



/** 
 * unicap_start_capture:
 * @handle: 
 * 
 * Start the capture device. After this call, unicap_wait_buffer calls are allowed
 * 
 * Returns:
 */
unicap_status_t unicap_start_capture( unicap_handle_t handle );

/**
 * unicap_stop_capture:
 * @handle:
 * 
 * Stop the capture device
 * 
 * Returns:
 */
unicap_status_t unicap_stop_capture( unicap_handle_t handle );

/** 
 * unicap_queue_buffer:
 * @handle:
 * @data_buffer:
 * 
 * Queue a buffer to be filled by the capture device. The queued
 * buffer must not be touched ( especially not be freed ) until it is
 * in the ready queue or dequeued. Supplied buffer must be at least of
 * the buffer size returned by get_format
 * 
 * Returns: 
 */
unicap_status_t unicap_queue_buffer( unicap_handle_t handle, unicap_data_buffer_t *data_buffer );

/** 
 * unicap_dequeue_buffer:
 * @handle:
 * @data_buffer: receives the dequeued buffer or NULL if no buffer was queued
 *
 * Removes the first buffer from the queue. Depending on the
 * device:Can only be called if the capture device is stopped.
 * 
 * Returns: status
 */
unicap_status_t unicap_dequeue_buffer( unicap_handle_t handle, unicap_data_buffer_t **data_buffer );

/**
 * unicap_wait_buffer
 * @handle:
 * @data_buffer:
 * 
 * Removes a buffer from the ready queue. If no buffer is available, this function blocks until a buffer got filled.
 * 
 * Returns: status
 */
unicap_status_t unicap_wait_buffer( unicap_handle_t handle, unicap_data_buffer_t **data_buffer );

/**
 * unicap_poll_buffer:
 * @handle: 
 * @count: 
 * 
 * Poll for buffers in the fill queue
 * 
 * Returns: status
 */
unicap_status_t unicap_poll_buffer( unicap_handle_t handle, int *count );


unicap_status_t unicap_describe_device( unicap_device_t *device, char *buffer, size_t *buffer_size );
unicap_status_t unicap_describe_format( unicap_format_t *format, char *buffer, size_t *buffer_size );
unicap_status_t unicap_describe_property( unicap_property_t *property, char *buffer, size_t *buffer_size );

unicap_status_t unicap_void_device( unicap_device_t *device );
unicap_status_t unicap_void_format( unicap_format_t *format );
unicap_status_t unicap_void_property( unicap_property_t *property );

unicap_format_t *unicap_copy_format( unicap_format_t *dest, unicap_format_t *src );
unicap_property_t *unicap_copy_property( unicap_property_t *dest, unicap_property_t *src );

/*
  Makes calling process the stream owner for this device
 */
unicap_status_t unicap_lock_stream( unicap_handle_t handle );
unicap_status_t unicap_unlock_stream( unicap_handle_t handle );
int unicap_is_stream_locked( unicap_device_t *device );
	
/*
  Makes calling process the properties owner
 */
unicap_status_t unicap_lock_properties( unicap_handle_t handle );
	
unicap_status_t unicap_unlock_properties( unicap_handle_t handle );
	
void unicap_cpi_register_event_notification( void *, 
					     void *data, unicap_handle_t handle );

/*
 */
void unicap_cache_init( void );



UNICAP_END_DECLS

#endif//__UNICAP_H__
