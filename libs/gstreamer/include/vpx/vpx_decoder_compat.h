/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */


/*!\defgroup decoder Common Decoder Algorithm Interface
 * This abstraction allows applications using this decoder to easily support
 * multiple video formats with minimal code duplication. This section describes
 * the interface common to all codecs.
 * @{
 */

/*!\file vpx_decoder_compat.h
 * \brief Provides a compatibility layer between version 1 and 2 of this API.
 *
 * This interface has been deprecated. Only existing code should make use
 * of this interface, and therefore, it is only thinly documented. Existing
 * code should be ported to the vpx_codec_* API.
 */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef VPX_DECODER_COMPAT_H
#define VPX_DECODER_COMPAT_H

    /*!\brief Decoder algorithm return codes */
    typedef enum {
        /*!\brief Operation completed without error */
        VPX_DEC_OK = VPX_CODEC_OK,

        /*!\brief Unspecified error */
        VPX_DEC_ERROR = VPX_CODEC_ERROR,

        /*!\brief Memory operation failed */
        VPX_DEC_MEM_ERROR = VPX_CODEC_MEM_ERROR,

        /*!\brief ABI version mismatch */
        VPX_DEC_ABI_MISMATCH = VPX_CODEC_ABI_MISMATCH,

        /*!\brief The given bitstream is not supported.
         *
         * The bitstream was unable to be parsed at the highest level. The decoder
         * is unable to proceed. This error \ref SHOULD be treated as fatal to the
         * stream. */
        VPX_DEC_UNSUP_BITSTREAM = VPX_CODEC_UNSUP_BITSTREAM,

        /*!\brief Encoded bitstream uses an unsupported feature
         *
         * The decoder does not implement a feature required by the encoder. This
         * return code should only be used for features that prevent future
         * pictures from being properly decoded. This error \ref MAY be treated as
         * fatal to the stream or \ref MAY be treated as fatal to the current GOP.
         */
        VPX_DEC_UNSUP_FEATURE = VPX_CODEC_UNSUP_FEATURE,

        /*!\brief The coded data for this stream is corrupt or incomplete
         *
         * There was a problem decoding the current frame.  This return code
         * should only be used for failures that prevent future pictures from
         * being properly decoded. This error \ref MAY be treated as fatal to the
         * stream or \ref MAY be treated as fatal to the current GOP. If decoding
         * is continued for the current GOP, artifacts may be present.
         */
        VPX_DEC_CORRUPT_FRAME = VPX_CODEC_CORRUPT_FRAME,

        /*!\brief An application-supplied parameter is not valid.
         *
         */
        VPX_DEC_INVALID_PARAM = VPX_CODEC_INVALID_PARAM,

        /*!\brief An iterator reached the end of list.
         *
         */
        VPX_DEC_LIST_END = VPX_CODEC_LIST_END

    }
    vpx_dec_err_t;

    /*! \brief Decoder capabilities bitfield
     *
     *  Each decoder advertises the capabilities it supports as part of its
     *  ::vpx_dec_iface_t interface structure. Capabilities are extra interfaces
     *  or functionality, and are not required to be supported by a decoder.
     *
     *  The available flags are specifiedby VPX_DEC_CAP_* defines.
     */
    typedef int vpx_dec_caps_t;
#define VPX_DEC_CAP_PUT_SLICE  0x0001 /**< Will issue put_slice callbacks */
#define VPX_DEC_CAP_PUT_FRAME  0x0002 /**< Will issue put_frame callbacks */
#define VPX_DEC_CAP_XMA        0x0004 /**< Supports e_xternal Memory Allocation */

    /*!\brief Stream properties
     *
     * This structure is used to query or set properties of the decoded
     * stream. Algorithms may extend this structure with data specific
     * to their bitstream by setting the sz member appropriately.
     */
#if 1
    typedef vpx_codec_stream_info_t vpx_dec_stream_info_t;
#else
    typedef struct
    {
        unsigned int sz;     /**< Size of this structure */
        unsigned int w;      /**< Width (or 0 for unknown/default) */
        unsigned int h;      /**< Height (or 0 for unknown/default) */
        unsigned int is_kf;  /**< Current frame is a keyframe */
    } vpx_dec_stream_info_t;
#endif


    /*!\brief Decoder interface structure.
     *
     * Contains function pointers and other data private to the decoder
     * implementation. This structure is opaque to the application.
     */
    typedef const struct vpx_codec_iface vpx_dec_iface_t;
    typedef       struct vpx_codec_priv  vpx_dec_priv_t;

    /*!\brief Iterator
     *
     * Opaque storage used for iterating over lists.
     */
    typedef vpx_codec_iter_t vpx_dec_iter_t;

    /*!\brief Decoder context structure
     *
     * All decoders \ref MUST support this context structure fully. In general,
     * this data should be considered private to the decoder algorithm, and
     * not be manipulated or examined by the calling application. Applications
     * may reference the 'name' member to get a printable description of the
     * algorithm.
     */
#if 1
    typedef vpx_codec_ctx_t vpx_dec_ctx_t;
#else
    typedef struct
    {
        const char            *name;        /**< Printable interface name */
        vpx_dec_iface_t       *iface;       /**< Interface pointers */
        vpx_dec_err_t          err;         /**< Last returned error */
        vpx_dec_priv_t        *priv;        /**< Algorithm private storage */
    } vpx_dec_ctx_t;
#endif


    /*!\brief Return the build configuration
     *
     * Returns a printable string containing an encoded version of the build
     * configuration. This may be useful to vpx support.
     *
     */
    const char *vpx_dec_build_config(void) DEPRECATED;

    /*!\brief Return the name for a given interface
     *
     * Returns a human readable string for name of the given decoder interface.
     *
     * \param[in]    iface     Interface pointer
     *
     */
    const char *vpx_dec_iface_name(vpx_dec_iface_t *iface) DEPRECATED;


    /*!\brief Convert error number to printable string
     *
     * Returns a human readable string for the last error returned by the
     * algorithm. The returned error will be one line and will not contain
     * any newline characters.
     *
     *
     * \param[in]    err     Error number.
     *
     */
    const char *vpx_dec_err_to_string(vpx_dec_err_t  err) DEPRECATED;


    /*!\brief Retrieve error synopsis for decoder context
     *
     * Returns a human readable string for the last error returned by the
     * algorithm. The returned error will be one line and will not contain
     * any newline characters.
     *
     *
     * \param[in]    ctx     Pointer to this instance's context.
     *
     */
    const char *vpx_dec_error(vpx_dec_ctx_t  *ctx) DEPRECATED;


    /*!\brief Retrieve detailed error information for decoder context
     *
     * Returns a human readable string providing detailed information about
     * the last error.
     *
     * \param[in]    ctx     Pointer to this instance's context.
     *
     * \retval NULL
     *     No detailed information is available.
     */
    const char *vpx_dec_error_detail(vpx_dec_ctx_t  *ctx) DEPRECATED;


    /* REQUIRED FUNCTIONS
     *
     * The following functions are required to be implemented for all decoders.
     * They represent the base case functionality expected of all decoders.
     */


    /*!\brief Initialize a decoder instance
     *
     * Initializes a decoder context using the given interface. Applications
     * should call the vpx_dec_init convenience macro instead of this
     * function directly, to ensure that the ABI version number parameter
     * is properly initialized.
     *
     * \param[in]    ctx     Pointer to this instance's context.
     * \param[in]    iface   Pointer to the alogrithm interface to use.
     * \param[in]    ver     ABI version number. Must be set to
     *                       VPX_DECODER_ABI_VERSION
     * \retval #VPX_DEC_OK
     *     The decoder algorithm initialized.
     * \retval #VPX_DEC_MEM_ERROR
     *     Memory allocation failed.
     */
    vpx_dec_err_t vpx_dec_init_ver(vpx_dec_ctx_t    *ctx,
                                   vpx_dec_iface_t  *iface,
                                   int               ver) DEPRECATED;
#define vpx_dec_init(ctx, iface) \
    vpx_dec_init_ver(ctx, iface, VPX_DECODER_ABI_VERSION)


    /*!\brief Destroy a decoder instance
     *
     * Destroys a decoder context, freeing any associated memory buffers.
     *
     * \param[in] ctx   Pointer to this instance's context
     *
     * \retval #VPX_DEC_OK
     *     The decoder algorithm initialized.
     * \retval #VPX_DEC_MEM_ERROR
     *     Memory allocation failed.
     */
    vpx_dec_err_t vpx_dec_destroy(vpx_dec_ctx_t *ctx) DEPRECATED;


    /*!\brief Get the capabilities of an algorithm.
     *
     * Retrieves the capabliities bitfield from the algorithm's interface.
     *
     * \param[in] iface   Pointer to the alogrithm interface
     *
     */
    vpx_dec_caps_t vpx_dec_get_caps(vpx_dec_iface_t *iface) DEPRECATED;


    /*!\brief Parse stream info from a buffer
     *
     * Performs high level parsing of the bitstream. Construction of a decoder
     * context is not necessary. Can be used to determine if the bitstream is
     * of the proper format, and to extract information from the stream.
     *
     * \param[in]      iface   Pointer to the alogrithm interface
     * \param[in]      data    Pointer to a block of data to parse
     * \param[in]      data_sz Size of the data buffer
     * \param[in,out]  si      Pointer to stream info to update. The size member
     *                         \ref MUST be properly initialized, but \ref MAY be
     *                         clobbered by the algorithm. This parameter \ref MAY
     *                         be NULL.
     *
     * \retval #VPX_DEC_OK
     *     Bitstream is parsable and stream information updated
     */
    vpx_dec_err_t vpx_dec_peek_stream_info(vpx_dec_iface_t       *iface,
                                           const uint8_t         *data,
                                           unsigned int           data_sz,
                                           vpx_dec_stream_info_t *si) DEPRECATED;


    /*!\brief Return information about the current stream.
     *
     * Returns information about the stream that has been parsed during decoding.
     *
     * \param[in]      ctx     Pointer to this instance's context
     * \param[in,out]  si      Pointer to stream info to update. The size member
     *                         \ref MUST be properly initialized, but \ref MAY be
     *                         clobbered by the algorithm. This parameter \ref MAY
     *                         be NULL.
     *
     * \retval #VPX_DEC_OK
     *     Bitstream is parsable and stream information updated
     */
    vpx_dec_err_t vpx_dec_get_stream_info(vpx_dec_ctx_t         *ctx,
                                          vpx_dec_stream_info_t *si) DEPRECATED;


    /*!\brief Control algorithm
     *
     * This function is used to exchange algorithm specific data with the decoder
     * instance. This can be used to implement features specific to a particular
     * algorithm.
     *
     * This wrapper function dispatches the request to the helper function
     * associated with the given ctrl_id. It tries to call this function
     * transparantly, but will return #VPX_DEC_ERROR if the request could not
     * be dispatched.
     *
     * \param[in]     ctx              Pointer to this instance's context
     * \param[in]     ctrl_id          Algorithm specific control identifier
     * \param[in,out] data             Data to exchange with algorithm instance.
     *
     * \retval #VPX_DEC_OK
     *     The control request was processed.
     * \retval #VPX_DEC_ERROR
     *     The control request was not processed.
     * \retval #VPX_DEC_INVALID_PARAM
     *     The data was not valid.
     */
    vpx_dec_err_t vpx_dec_control(vpx_dec_ctx_t  *ctx,
                                  int             ctrl_id,
                                  void           *data) DEPRECATED;

    /*!\brief Decode data
     *
     * Processes a buffer of coded data. If the processing results in a new
     * decoded frame becoming available, #VPX_DEC_CB_PUT_SLICE and
     * #VPX_DEC_CB_PUT_FRAME events may be generated, as appropriate. Encoded data
     * \ref MUST be passed in DTS (decode time stamp) order. Frames produced will
     * always be in PTS (presentation time stamp) order.
     *
     * \param[in] ctx          Pointer to this instance's context
     * \param[in] data         Pointer to this block of new coded data. If
     *                         NULL, a VPX_DEC_CB_PUT_FRAME event is posted
     *                         for the previously decoded frame.
     * \param[in] data_sz      Size of the coded data, in bytes.
     * \param[in] user_priv    Application specific data to associate with
     *                         this frame.
     * \param[in] rel_pts      PTS relative to the previous frame, in us. If
     *                         unknown or unavailable, set to zero.
     *
     * \return Returns #VPX_DEC_OK if the coded data was processed completely
     *         and future pictures can be decoded without error. Otherwise,
     *         see the descriptions of the other error codes in ::vpx_dec_err_t
     *         for recoverability capabilities.
     */
    vpx_dec_err_t vpx_dec_decode(vpx_dec_ctx_t  *ctx,
                                 uint8_t        *data,
                                 unsigned int    data_sz,
                                 void       *user_priv,
                                 int         rel_pts) DEPRECATED;


    /*!\brief Decoded frames iterator
     *
     * Iterates over a list of the frames available for display. The iterator
     * storage should be initialized to NULL to start the iteration. Iteration is
     * complete when this function returns NULL.
     *
     * The list of available frames becomes valid upon completion of the
     * vpx_dec_decode call, and remains valid until the next call to vpx_dec_decode.
     *
     * \param[in]     ctx      Pointer to this instance's context
     * \param[in out] iter     Iterator storage, initialized to NULL
     *
     * \return Returns a pointer to an image, if one is ready for display. Frames
     *         produced will always be in PTS (presentation time stamp) order.
     */
    vpx_image_t *vpx_dec_get_frame(vpx_dec_ctx_t  *ctx,
                                   vpx_dec_iter_t *iter) DEPRECATED;


    /*!\defgroup cap_put_frame Frame-Based Decoding Functions
     *
     * The following functions are required to be implemented for all decoders
     * that advertise the VPX_DEC_CAP_PUT_FRAME capability. Calling these functions
     * for codecs that don't advertise this capability will result in an error
     * code being returned, usually VPX_DEC_ERROR
     * @{
     */

    /*!\brief put frame callback prototype
     *
     * This callback is invoked by the decoder to notify the application of
     * the availability of decoded image data.
     */
    typedef void (*vpx_dec_put_frame_cb_fn_t)(void          *user_priv,
            const vpx_image_t *img);


    /*!\brief Register for notification of frame completion.
     *
     * Registers a given function to be called when a decoded frame is
     * available.
     *
     * \param[in] ctx          Pointer to this instance's context
     * \param[in] cb           Pointer to the callback function
     * \param[in] user_priv    User's private data
     *
     * \retval #VPX_DEC_OK
     *     Callback successfully registered.
     * \retval #VPX_DEC_ERROR
     *     Decoder context not initialized, or algorithm not capable of
     *     posting slice completion.
     */
    vpx_dec_err_t vpx_dec_register_put_frame_cb(vpx_dec_ctx_t             *ctx,
            vpx_dec_put_frame_cb_fn_t  cb,
            void                      *user_priv) DEPRECATED;


    /*!@} - end defgroup cap_put_frame */

    /*!\defgroup cap_put_slice Slice-Based Decoding Functions
     *
     * The following functions are required to be implemented for all decoders
     * that advertise the VPX_DEC_CAP_PUT_SLICE capability. Calling these functions
     * for codecs that don't advertise this capability will result in an error
     * code being returned, usually VPX_DEC_ERROR
     * @{
     */

    /*!\brief put slice callback prototype
     *
     * This callback is invoked by the decoder to notify the application of
     * the availability of partially decoded image data. The
     */
    typedef void (*vpx_dec_put_slice_cb_fn_t)(void           *user_priv,
            const vpx_image_t      *img,
            const vpx_image_rect_t *valid,
            const vpx_image_rect_t *update);


    /*!\brief Register for notification of slice completion.
     *
     * Registers a given function to be called when a decoded slice is
     * available.
     *
     * \param[in] ctx          Pointer to this instance's context
     * \param[in] cb           Pointer to the callback function
     * \param[in] user_priv    User's private data
     *
     * \retval #VPX_DEC_OK
     *     Callback successfully registered.
     * \retval #VPX_DEC_ERROR
     *     Decoder context not initialized, or algorithm not capable of
     *     posting slice completion.
     */
    vpx_dec_err_t vpx_dec_register_put_slice_cb(vpx_dec_ctx_t             *ctx,
            vpx_dec_put_slice_cb_fn_t  cb,
            void                      *user_priv) DEPRECATED;


    /*!@} - end defgroup cap_put_slice*/

    /*!\defgroup cap_xma External Memory Allocation Functions
     *
     * The following functions are required to be implemented for all decoders
     * that advertise the VPX_DEC_CAP_XMA capability. Calling these functions
     * for codecs that don't advertise this capability will result in an error
     * code being returned, usually VPX_DEC_ERROR
     * @{
     */

    /*!\brief Memory Map Entry
     *
     * This structure is used to contain the properties of a memory segment. It
     * is populated by the decoder in the request phase, and by the calling
     * application once the requested allocation has been performed.
     */
#if 1
#define VPX_DEC_MEM_ZERO     0x1  /**< Segment must be zeroed by allocation */
#define VPX_DEC_MEM_WRONLY   0x2  /**< Segment need not be readable */
#define VPX_DEC_MEM_FAST     0x4  /**< Place in fast memory, if available */
    typedef struct vpx_codec_mmap vpx_dec_mmap_t;
#else
    typedef struct vpx_dec_mmap
    {
        /*
         * The following members are set by the codec when requesting a segment
         */
        unsigned int   id;     /**< identifier for the segment's contents */
        unsigned long  sz;     /**< size of the segment, in bytes */
        unsigned int   align;  /**< required alignment of the segment, in bytes */
        unsigned int   flags;  /**< bitfield containing segment properties */
#define VPX_DEC_MEM_ZERO     0x1  /**< Segment must be zeroed by allocation */
#define VPX_DEC_MEM_WRONLY   0x2  /**< Segment need not be readable */
#define VPX_DEC_MEM_FAST     0x4  /**< Place in fast memory, if available */

        /* The following members are to be filled in by the allocation function */
        void          *base;   /**< pointer to the allocated segment */
        void (*dtor)(struct vpx_dec_mmap *map);         /**< destructor to call */
        void          *priv;   /**< allocator private storage */
    } vpx_dec_mmap_t;
#endif

    /*!\brief Initialize a decoder instance in external allocation mode
     *
     * Initializes a decoder context using the given interface. Applications
     * should call the vpx_dec_xma_init convenience macro instead of this
     * function directly, to ensure that the ABI version number parameter
     * is properly initialized.
     *
     * \param[in]    ctx     Pointer to this instance's context.
     * \param[in]    iface   Pointer to the alogrithm interface to use.
     * \param[in]    ver     ABI version number. Must be set to
     *                       VPX_DECODER_ABI_VERSION
     * \retval #VPX_DEC_OK
     *     The decoder algorithm initialized.
     * \retval #VPX_DEC_ERROR
     *     Decoder does not support XMA mode.
     */
    vpx_dec_err_t vpx_dec_xma_init_ver(vpx_dec_ctx_t    *ctx,
                                       vpx_dec_iface_t  *iface,
                                       int               ver) DEPRECATED;
#define vpx_dec_xma_init(ctx, iface) \
    vpx_dec_xma_init_ver(ctx, iface, VPX_DECODER_ABI_VERSION)


    /*!\brief Iterate over the list of segments to allocate.
     *
     * Iterates over a list of the segments to allocate. The iterator storage
     * should be initialized to NULL to start the iteration. Iteration is complete
     * when this function returns VPX_DEC_LIST_END. The amount of memory needed to
     * allocate is dependant upon the size of the encoded stream. This means that
     * the stream info structure must be known at allocation time. It can be
     * populated with the vpx_dec_peek_stream_info() function. In cases where the
     * stream to be decoded is not available at allocation time, a fixed size must
     * be requested. The decoder will not be able to decode streams larger than
     * the size used at allocation time.
     *
     * \param[in]      ctx     Pointer to this instance's context.
     * \param[out]     mmap    Pointer to the memory map entry to populate.
     * \param[in]      si      Pointer to the stream info.
     * \param[in out]  iter    Iterator storage, initialized to NULL
     *
     * \retval #VPX_DEC_OK
     *     The memory map entry was populated.
     * \retval #VPX_DEC_ERROR
     *     Decoder does not support XMA mode.
     * \retval #VPX_DEC_MEM_ERROR
     *     Unable to determine segment size from stream info.
     */
    vpx_dec_err_t vpx_dec_get_mem_map(vpx_dec_ctx_t                *ctx,
                                      vpx_dec_mmap_t               *mmap,
                                      const vpx_dec_stream_info_t  *si,
                                      vpx_dec_iter_t               *iter) DEPRECATED;


    /*!\brief Identify allocated segments to decoder instance
     *
     * Stores a list of allocated segments in the decoder. Segments \ref MUST be
     * passed in the order they are read from vpx_dec_get_mem_map(), but may be
     * passed in groups of any size. Segments \ref MUST be set only once. The
     * allocation function \ref MUST ensure that the vpx_dec_mmap_t::base member
     * is non-NULL. If the segment requires cleanup handling (eg, calling free()
     * or close()) then the vpx_dec_mmap_t::dtor member \ref MUST be populated.
     *
     * \param[in]      ctx     Pointer to this instance's context.
     * \param[in]      mmaps   Pointer to the first memory map entry in the list.
     * \param[in]      num_maps  Number of entries being set at this time
     *
     * \retval #VPX_DEC_OK
     *     The segment was stored in the decoder context.
     * \retval #VPX_DEC_ERROR
     *     Decoder does not support XMA mode.
     * \retval #VPX_DEC_MEM_ERROR
     *     Segment base address was not set, or segment was already stored.

     */
    vpx_dec_err_t  vpx_dec_set_mem_map(vpx_dec_ctx_t   *ctx,
                                       vpx_dec_mmap_t  *mmaps,
                                       unsigned int     num_maps) DEPRECATED;

    /*!@} - end defgroup cap_xma*/
    /*!@} - end defgroup decoder*/


#endif
#ifdef __cplusplus
}
#endif
