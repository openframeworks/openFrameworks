/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */


/*!\file vpx_image.h
 * \brief Describes the vpx image descriptor and associated operations
 *
 */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef VPX_IMAGE_H
#define VPX_IMAGE_H

    /*!\brief Current ABI version number
     *
     * \internal
     * If this file is altered in any way that changes the ABI, this value
     * must be bumped.  Examples include, but are not limited to, changing
     * types, removing or reassigning enums, adding/removing/rearranging
     * fields to structures
     */
#define VPX_IMAGE_ABI_VERSION (1) /**<\hideinitializer*/


#define VPX_IMG_FMT_PLANAR     0x100  /**< Image is a planar format */
#define VPX_IMG_FMT_UV_FLIP    0x200  /**< V plane precedes U plane in memory */
#define VPX_IMG_FMT_HAS_ALPHA  0x400  /**< Image has an alpha channel componnent */


    /*!\brief List of supported image formats */
    typedef enum vpx_img_fmt {
        VPX_IMG_FMT_NONE,
        VPX_IMG_FMT_RGB24,   /**< 24 bit per pixel packed RGB */
        VPX_IMG_FMT_RGB32,   /**< 32 bit per pixel packed 0RGB */
        VPX_IMG_FMT_RGB565,  /**< 16 bit per pixel, 565 */
        VPX_IMG_FMT_RGB555,  /**< 16 bit per pixel, 555 */
        VPX_IMG_FMT_UYVY,    /**< UYVY packed YUV */
        VPX_IMG_FMT_YUY2,    /**< YUYV packed YUV */
        VPX_IMG_FMT_YVYU,    /**< YVYU packed YUV */
        VPX_IMG_FMT_BGR24,   /**< 24 bit per pixel packed BGR */
        VPX_IMG_FMT_RGB32_LE, /**< 32 bit packed BGR0 */
        VPX_IMG_FMT_ARGB,     /**< 32 bit packed ARGB, alpha=255 */
        VPX_IMG_FMT_ARGB_LE,  /**< 32 bit packed BGRA, alpha=255 */
        VPX_IMG_FMT_RGB565_LE,  /**< 16 bit per pixel, gggbbbbb rrrrrggg */
        VPX_IMG_FMT_RGB555_LE,  /**< 16 bit per pixel, gggbbbbb 0rrrrrgg */
        VPX_IMG_FMT_YV12    = VPX_IMG_FMT_PLANAR | VPX_IMG_FMT_UV_FLIP | 1, /**< planar YVU */
        VPX_IMG_FMT_I420    = VPX_IMG_FMT_PLANAR | 2,
        VPX_IMG_FMT_VPXYV12 = VPX_IMG_FMT_PLANAR | VPX_IMG_FMT_UV_FLIP | 3, /** < planar 4:2:0 format with vpx color space */
        VPX_IMG_FMT_VPXI420 = VPX_IMG_FMT_PLANAR | 4   /** < planar 4:2:0 format with vpx color space */
    }
    vpx_img_fmt_t; /**< alias for enum vpx_img_fmt */

#if !defined(VPX_CODEC_DISABLE_COMPAT) || !VPX_CODEC_DISABLE_COMPAT
#define IMG_FMT_PLANAR         VPX_IMG_FMT_PLANAR     /**< \deprecated Use #VPX_IMG_FMT_PLANAR */
#define IMG_FMT_UV_FLIP        VPX_IMG_FMT_UV_FLIP    /**< \deprecated Use #VPX_IMG_FMT_UV_FLIP */
#define IMG_FMT_HAS_ALPHA      VPX_IMG_FMT_HAS_ALPHA  /**< \deprecated Use #VPX_IMG_FMT_HAS_ALPHA */

    /*!\brief Deprecated list of supported image formats
     * \deprecated New code should use #vpx_img_fmt
     */
#define img_fmt   vpx_img_fmt
    /*!\brief alias for enum img_fmt.
     * \deprecated New code should use #vpx_img_fmt_t
     */
#define img_fmt_t vpx_img_fmt_t

#define IMG_FMT_NONE       VPX_IMG_FMT_NONE       /**< \deprecated Use #VPX_IMG_FMT_NONE */
#define IMG_FMT_RGB24      VPX_IMG_FMT_RGB24      /**< \deprecated Use #VPX_IMG_FMT_RGB24 */
#define IMG_FMT_RGB32      VPX_IMG_FMT_RGB32      /**< \deprecated Use #VPX_IMG_FMT_RGB32 */
#define IMG_FMT_RGB565     VPX_IMG_FMT_RGB565     /**< \deprecated Use #VPX_IMG_FMT_RGB565 */
#define IMG_FMT_RGB555     VPX_IMG_FMT_RGB555     /**< \deprecated Use #VPX_IMG_FMT_RGB555 */
#define IMG_FMT_UYVY       VPX_IMG_FMT_UYVY       /**< \deprecated Use #VPX_IMG_FMT_UYVY */
#define IMG_FMT_YUY2       VPX_IMG_FMT_YUY2       /**< \deprecated Use #VPX_IMG_FMT_YUY2 */
#define IMG_FMT_YVYU       VPX_IMG_FMT_YVYU       /**< \deprecated Use #VPX_IMG_FMT_YVYU */
#define IMG_FMT_BGR24      VPX_IMG_FMT_BGR24      /**< \deprecated Use #VPX_IMG_FMT_BGR24 */
#define IMG_FMT_RGB32_LE   VPX_IMG_FMT_RGB32_LE   /**< \deprecated Use #VPX_IMG_FMT_RGB32_LE */
#define IMG_FMT_ARGB       VPX_IMG_FMT_ARGB       /**< \deprecated Use #VPX_IMG_FMT_ARGB */
#define IMG_FMT_ARGB_LE    VPX_IMG_FMT_ARGB_LE    /**< \deprecated Use #VPX_IMG_FMT_ARGB_LE */
#define IMG_FMT_RGB565_LE  VPX_IMG_FMT_RGB565_LE  /**< \deprecated Use #VPX_IMG_FMT_RGB565_LE */
#define IMG_FMT_RGB555_LE  VPX_IMG_FMT_RGB555_LE  /**< \deprecated Use #VPX_IMG_FMT_RGB555_LE */
#define IMG_FMT_YV12       VPX_IMG_FMT_YV12       /**< \deprecated Use #VPX_IMG_FMT_YV12 */
#define IMG_FMT_I420       VPX_IMG_FMT_I420       /**< \deprecated Use #VPX_IMG_FMT_I420 */
#define IMG_FMT_VPXYV12    VPX_IMG_FMT_VPXYV12    /**< \deprecated Use #VPX_IMG_FMT_VPXYV12 */
#define IMG_FMT_VPXI420    VPX_IMG_FMT_VPXI420    /**< \deprecated Use #VPX_IMG_FMT_VPXI420 */
#endif /* VPX_CODEC_DISABLE_COMPAT */

    /**\brief Image Descriptor */
    typedef struct vpx_image
    {
        vpx_img_fmt_t fmt; /**< Image Format */

        /* Image storage dimensions */
        unsigned int  w;   /**< Stored image width */
        unsigned int  h;   /**< Stored image height */

        /* Image display dimensions */
        unsigned int  d_w;   /**< Displayed image width */
        unsigned int  d_h;   /**< Displayed image height */

        /* Chroma subsampling info */
        unsigned int  x_chroma_shift;   /**< subsampling order, X */
        unsigned int  y_chroma_shift;   /**< subsampling order, Y */

        /* Image data pointers. */
#define VPX_PLANE_PACKED 0   /**< To be used for all packed formats */
#define VPX_PLANE_Y      0   /**< Y (Luminance) plane */
#define VPX_PLANE_U      1   /**< U (Chroma) plane */
#define VPX_PLANE_V      2   /**< V (Chroma) plane */
#define VPX_PLANE_ALPHA  3   /**< A (Transparancy) plane */
#if !defined(VPX_CODEC_DISABLE_COMPAT) || !VPX_CODEC_DISABLE_COMPAT
#define PLANE_PACKED     VPX_PLANE_PACKED
#define PLANE_Y          VPX_PLANE_Y
#define PLANE_U          VPX_PLANE_U
#define PLANE_V          VPX_PLANE_V
#define PLANE_ALPHA      VPX_PLANE_ALPHA
#endif
        unsigned char *planes[4];  /**< pointer to the top left pixel for each plane */
        int      stride[4];  /**< stride between rows for each plane */

        int     bps; /**< bits per sample (for packed formats) */

        /* The following member may be set by the application to associate data
         * with this image.
         */
        void    *user_priv; /**< may be set by the application to associate data
                         *   with this image. */

        /* The following members should be treated as private. */
        unsigned char *img_data;       /**< private */
        int      img_data_owner; /**< private */
        int      self_allocd;    /**< private */
    } vpx_image_t; /**< alias for struct vpx_image */

    /**\brief Representation of a rectangle on a surface */
    typedef struct vpx_image_rect
    {
        unsigned int x; /**< leftmost column */
        unsigned int y; /**< topmost row */
        unsigned int w; /**< width */
        unsigned int h; /**< height */
    } vpx_image_rect_t; /**< alias for struct vpx_image_rect */

    /*!\brief Open a descriptor, allocating storage for the underlying image
     *
     * Returns a descriptor for storing an image of the given format. The
     * storage for the descriptor is allocated on the heap.
     *
     * \param[in]    img       Pointer to storage for descriptor. If this parameter
     *                         is NULL, the storage for the descriptor will be
     *                         allocated on the heap.
     * \param[in]    fmt       Format for the image
     * \param[in]    d_w       Width of the image
     * \param[in]    d_h       Height of the image
     * \param[in]    align     Alignment, in bytes, of each row in the image.
     *
     * \return Returns a pointer to the initialized image descriptor. If the img
     *         parameter is non-null, the value of the img parameter will be
     *         returned.
     */
    vpx_image_t *vpx_img_alloc(vpx_image_t  *img,
                               vpx_img_fmt_t fmt,
                               unsigned int d_w,
                               unsigned int d_h,
                               unsigned int align);

    /*!\brief Open a descriptor, using existing storage for the underlying image
     *
     * Returns a descriptor for storing an image of the given format. The
     * storage for descriptor has been allocated elsewhere, and a descriptor is
     * desired to "wrap" that storage.
     *
     * \param[in]    img       Pointer to storage for descriptor. If this parameter
     *                         is NULL, the storage for the descriptor will be
     *                         allocated on the heap.
     * \param[in]    fmt       Format for the image
     * \param[in]    d_w       Width of the image
     * \param[in]    d_h       Height of the image
     * \param[in]    align     Alignment, in bytes, of each row in the image.
     * \param[in]    img_data  Storage to use for the image
     *
     * \return Returns a pointer to the initialized image descriptor. If the img
     *         parameter is non-null, the value of the img parameter will be
     *         returned.
     */
    vpx_image_t *vpx_img_wrap(vpx_image_t  *img,
                              vpx_img_fmt_t fmt,
                              unsigned int d_w,
                              unsigned int d_h,
                              unsigned int align,
                              unsigned char      *img_data);


    /*!\brief Set the rectangle identifying the displayed portion of the image
     *
     * Updates the displayed rectangle (aka viewport) on the image surface to
     * match the specified coordinates and size.
     *
     * \param[in]    img       Image descriptor
     * \param[in]    x         leftmost column
     * \param[in]    y         topmost row
     * \param[in]    w         width
     * \param[in]    h         height
     *
     * \return 0 if the requested rectangle is valid, nonzero otherwise.
     */
    int vpx_img_set_rect(vpx_image_t  *img,
                         unsigned int  x,
                         unsigned int  y,
                         unsigned int  w,
                         unsigned int  h);


    /*!\brief Flip the image vertically (top for bottom)
     *
     * Adjusts the image descriptor's pointers and strides to make the image
     * be referenced upside-down.
     *
     * \param[in]    img       Image descriptor
     */
    void vpx_img_flip(vpx_image_t *img);

    /*!\brief Close an image descriptor
     *
     * Frees all allocated storage associated with an image descriptor.
     *
     * \param[in]    img       Image descriptor
     */
    void vpx_img_free(vpx_image_t *img);

#endif
#ifdef __cplusplus
}
#endif
