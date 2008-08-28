/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef _CV_HPP_
#define _CV_HPP_

#ifdef __cplusplus

/****************************************************************************************\
*                    CvBaseImageFilter: Base class for filtering operations              *
\****************************************************************************************/

#define CV_WHOLE   0
#define CV_START   1
#define CV_END     2
#define CV_MIDDLE  4
#define CV_ISOLATED_ROI 8

typedef void (*CvRowFilterFunc)( const uchar* src, uchar* dst, void* params );
typedef void (*CvColumnFilterFunc)( uchar** src, uchar* dst, int dst_step, int count, void* params );

class CV_EXPORTS CvBaseImageFilter
{
public:
    CvBaseImageFilter();
    /* calls init() */
    CvBaseImageFilter( int _max_width, int _src_type, int _dst_type,
                       bool _is_separable, CvSize _ksize,
                       CvPoint _anchor=cvPoint(-1,-1),
                       int _border_mode=IPL_BORDER_REPLICATE,
                       CvScalar _border_value=cvScalarAll(0) );
    virtual ~CvBaseImageFilter();

    /* initializes the class for processing an image of maximal width _max_width,
       input image has data type _src_type, the output will have _dst_type.
       _is_separable != 0 if the filter is separable
       (specific behaviour is defined in a derived class), 0 otherwise.
       _ksize and _anchor specify the kernel size and the anchor point. _anchor=(-1,-1) means
       that the anchor is at the center.
       to get interpolate pixel values outside the image _border_mode=IPL_BORDER_*** is used,
       _border_value specify the pixel value in case of IPL_BORDER_CONSTANT border mode.
       before initialization clear() is called if necessary.
    */
    virtual void init( int _max_width, int _src_type, int _dst_type,
                       bool _is_separable, CvSize _ksize,
                       CvPoint _anchor=cvPoint(-1,-1),
                       int _border_mode=IPL_BORDER_REPLICATE,
                       CvScalar _border_value=cvScalarAll(0) );
    /* releases all the internal buffers.
       for the further use of the object, init() needs to be called. */
    virtual void clear();
    /* processes input image or a part of it.
       input is represented either as matrix (CvMat* src)
       or a list of row pointers (uchar** src2).
       in the later case width, _src_y1 and _src_y2 are used to specify the size.
       _dst is the output image/matrix.
       _src_roi specifies the roi inside the input image to process,
          (0,0,-1,-1) denotes the whole image.
       _dst_origin is the upper-left corner of the filtered roi within the output image.
       _phase is either CV_START, or CV_END, or CV_MIDDLE, or CV_START|CV_END, or CV_WHOLE,
          which is the same as CV_START|CV_END.
          CV_START means that the input is the first (top) stripe of the processed image [roi],
          CV_END - the input is the last (bottom) stripe of the processed image [roi],
          CV_MIDDLE - the input is neither first nor last stripe.
          CV_WHOLE - the input is the whole processed image [roi].
    */
    virtual int process( const CvMat* _src, CvMat* _dst,
                         CvRect _src_roi=cvRect(0,0,-1,-1),
                         CvPoint _dst_origin=cvPoint(0,0), int _flags=0 );
    /* retrieve various parameters of the filtering object */
    int get_src_type() const { return src_type; }
    int get_dst_type() const { return dst_type; }
    int get_work_type() const { return work_type; }
    CvSize get_kernel_size() const { return ksize; }
    CvPoint get_anchor() const { return anchor; }
    int get_width() const { return prev_x_range.end_index - prev_x_range.start_index; }
    CvRowFilterFunc get_x_filter_func() const { return x_func; }
    CvColumnFilterFunc get_y_filter_func() const { return y_func; }

protected:
    /* initializes work_type, buf_size and max_rows */ 
    virtual void get_work_params();
    /* it is called (not always) from process when _phase=CV_START or CV_WHOLE.
       the method initializes ring buffer (buf_end, buf_head, buf_tail, buf_count, rows),
       prev_width, prev_x_range, const_row, border_tab, border_tab_sz* */
    virtual void start_process( CvSlice x_range, int width );
    /* forms pointers to "virtual rows" above or below the processed roi using the specified
       border mode */
    virtual void make_y_border( int row_count, int top_rows, int bottom_rows );

    virtual int fill_cyclic_buffer( const uchar* src, int src_step,
                                    int y, int y1, int y2 );

    enum { ALIGN=32 };
    
    int max_width;
    /* currently, work_type must be the same as src_type in case of non-separable filters */
    int min_depth, src_type, dst_type, work_type;

    /* pointers to convolution functions, initialized by init method.
       for non-separable filters only y_conv should be set */
    CvRowFilterFunc x_func;
    CvColumnFilterFunc y_func;

    uchar* buffer;
    uchar** rows;
    int top_rows, bottom_rows, max_rows;
    uchar *buf_start, *buf_end, *buf_head, *buf_tail;
    int buf_size, buf_step, buf_count, buf_max_count;

    bool is_separable;
    CvSize ksize;
    CvPoint anchor;
    int max_ky, border_mode;
    CvScalar border_value;
    uchar* const_row;
    int* border_tab;
    int border_tab_sz1, border_tab_sz;

    CvSlice prev_x_range;
    int prev_width;
};


/* Derived class, for linear separable filtering. */
class CV_EXPORTS CvSepFilter : public CvBaseImageFilter
{
public:
    CvSepFilter();
    CvSepFilter( int _max_width, int _src_type, int _dst_type,
                 const CvMat* _kx, const CvMat* _ky,
                 CvPoint _anchor=cvPoint(-1,-1),
                 int _border_mode=IPL_BORDER_REPLICATE,
                 CvScalar _border_value=cvScalarAll(0) );
    virtual ~CvSepFilter();

    virtual void init( int _max_width, int _src_type, int _dst_type,
                       const CvMat* _kx, const CvMat* _ky,
                       CvPoint _anchor=cvPoint(-1,-1),
                       int _border_mode=IPL_BORDER_REPLICATE,
                       CvScalar _border_value=cvScalarAll(0) );
    virtual void init_deriv( int _max_width, int _src_type, int _dst_type,
                             int dx, int dy, int aperture_size, int flags=0 );
    virtual void init_gaussian( int _max_width, int _src_type, int _dst_type,
                                int gaussian_size, double sigma );

    /* dummy method to avoid compiler warnings */
    virtual void init( int _max_width, int _src_type, int _dst_type,
                       bool _is_separable, CvSize _ksize,
                       CvPoint _anchor=cvPoint(-1,-1),
                       int _border_mode=IPL_BORDER_REPLICATE,
                       CvScalar _border_value=cvScalarAll(0) );

    virtual void clear();
    const CvMat* get_x_kernel() const { return kx; }
    const CvMat* get_y_kernel() const { return ky; }
    int get_x_kernel_flags() const { return kx_flags; }
    int get_y_kernel_flags() const { return ky_flags; }

    enum { GENERIC=0, ASYMMETRICAL=1, SYMMETRICAL=2, POSITIVE=4, SUM_TO_1=8, INTEGER=16 };
    enum { NORMALIZE_KERNEL=1, FLIP_KERNEL=2 };

    static void init_gaussian_kernel( CvMat* kernel, double sigma=-1 );
    static void init_sobel_kernel( CvMat* _kx, CvMat* _ky, int dx, int dy, int flags=0 );
    static void init_scharr_kernel( CvMat* _kx, CvMat* _ky, int dx, int dy, int flags=0 );

protected:
    CvMat *kx, *ky;
    int kx_flags, ky_flags;
};


/* Derived class, for linear non-separable filtering. */
class CV_EXPORTS CvLinearFilter : public CvBaseImageFilter
{
public:
    CvLinearFilter();
    CvLinearFilter( int _max_width, int _src_type, int _dst_type,
                    const CvMat* _kernel,
                    CvPoint _anchor=cvPoint(-1,-1),
                    int _border_mode=IPL_BORDER_REPLICATE,
                    CvScalar _border_value=cvScalarAll(0) );
    virtual ~CvLinearFilter();

    virtual void init( int _max_width, int _src_type, int _dst_type,
                       const CvMat* _kernel,
                       CvPoint _anchor=cvPoint(-1,-1),
                       int _border_mode=IPL_BORDER_REPLICATE,
                       CvScalar _border_value=cvScalarAll(0) );

    /* dummy method to avoid compiler warnings */
    virtual void init( int _max_width, int _src_type, int _dst_type,
                       bool _is_separable, CvSize _ksize,
                       CvPoint _anchor=cvPoint(-1,-1),
                       int _border_mode=IPL_BORDER_REPLICATE,
                       CvScalar _border_value=cvScalarAll(0) );

    virtual void clear();
    const CvMat* get_kernel() const { return kernel; }
    uchar* get_kernel_sparse_buf() { return k_sparse; }
    int get_kernel_sparse_count() const { return k_sparse_count; }

protected:
    CvMat *kernel;
    uchar* k_sparse;
    int k_sparse_count;
};


/* Box filter ("all 1's", optionally normalized) filter. */
class CV_EXPORTS CvBoxFilter : public CvBaseImageFilter
{
public:
    CvBoxFilter();
    CvBoxFilter( int _max_width, int _src_type, int _dst_type,
                 bool _normalized, CvSize _ksize,
                 CvPoint _anchor=cvPoint(-1,-1),
                 int _border_mode=IPL_BORDER_REPLICATE,
                 CvScalar _border_value=cvScalarAll(0) );
    virtual void init( int _max_width, int _src_type, int _dst_type,
                       bool _normalized, CvSize _ksize,
                       CvPoint _anchor=cvPoint(-1,-1),
                       int _border_mode=IPL_BORDER_REPLICATE,
                       CvScalar _border_value=cvScalarAll(0) );

    virtual ~CvBoxFilter();
    bool is_normalized() const { return normalized; }
    double get_scale() const { return scale; }
    uchar* get_sum_buf() { return sum; }
    int* get_sum_count_ptr() { return &sum_count; }

protected:
    virtual void start_process( CvSlice x_range, int width );

    uchar* sum;
    int sum_count;
    bool normalized;
    double scale;
};


/* Laplacian operator: (d2/dx + d2/dy)I. */
class CV_EXPORTS CvLaplaceFilter : public CvSepFilter
{
public:
    CvLaplaceFilter();
    CvLaplaceFilter( int _max_width, int _src_type, int _dst_type,
                     bool _normalized, int _ksize,
                     int _border_mode=IPL_BORDER_REPLICATE,
                     CvScalar _border_value=cvScalarAll(0) );
    virtual ~CvLaplaceFilter();
    virtual void init( int _max_width, int _src_type, int _dst_type,
                       bool _normalized, int _ksize,
                       int _border_mode=IPL_BORDER_REPLICATE,
                       CvScalar _border_value=cvScalarAll(0) );

    /* dummy methods to avoid compiler warnings */
    virtual void init( int _max_width, int _src_type, int _dst_type,
                       bool _is_separable, CvSize _ksize,
                       CvPoint _anchor=cvPoint(-1,-1),
                       int _border_mode=IPL_BORDER_REPLICATE,
                       CvScalar _border_value=cvScalarAll(0) );

    virtual void init( int _max_width, int _src_type, int _dst_type,
                       const CvMat* _kx, const CvMat* _ky,
                       CvPoint _anchor=cvPoint(-1,-1),
                       int _border_mode=IPL_BORDER_REPLICATE,
                       CvScalar _border_value=cvScalarAll(0) );

    bool is_normalized() const { return normalized; }
    bool is_basic_laplacian() const { return basic_laplacian; }
protected:
    void get_work_params();

    bool basic_laplacian;
    bool normalized;
};


/* basic morphological operations: erosion & dilation */
class CV_EXPORTS CvMorphology : public CvBaseImageFilter
{
public:
    CvMorphology();
    CvMorphology( int _operation, int _max_width, int _src_dst_type,
                  int _element_shape, CvMat* _element,
                  CvSize _ksize=cvSize(0,0), CvPoint _anchor=cvPoint(-1,-1),
                  int _border_mode=IPL_BORDER_REPLICATE,
                  CvScalar _border_value=cvScalarAll(0) );
    virtual ~CvMorphology();
    virtual void init( int _operation, int _max_width, int _src_dst_type,
                       int _element_shape, CvMat* _element,
                       CvSize _ksize=cvSize(0,0), CvPoint _anchor=cvPoint(-1,-1),
                       int _border_mode=IPL_BORDER_REPLICATE,
                       CvScalar _border_value=cvScalarAll(0) );

    /* dummy method to avoid compiler warnings */
    virtual void init( int _max_width, int _src_type, int _dst_type,
                       bool _is_separable, CvSize _ksize,
                       CvPoint _anchor=cvPoint(-1,-1),
                       int _border_mode=IPL_BORDER_REPLICATE,
                       CvScalar _border_value=cvScalarAll(0) );

    virtual void clear();
    const CvMat* get_element() const { return element; }
    int get_element_shape() const { return el_shape; }
    int get_operation() const { return operation; }
    uchar* get_element_sparse_buf() { return el_sparse; }
    int get_element_sparse_count() const { return el_sparse_count; }

    enum { RECT=0, CROSS=1, ELLIPSE=2, CUSTOM=100, BINARY = 0, GRAYSCALE=256 };
    enum { ERODE=0, DILATE=1 };

    static void init_binary_element( CvMat* _element, int _element_shape,
                                     CvPoint _anchor=cvPoint(-1,-1) );
protected:

    void start_process( CvSlice x_range, int width );
    int fill_cyclic_buffer( const uchar* src, int src_step,
                            int y0, int y1, int y2 );
    uchar* el_sparse;
    int el_sparse_count;

    CvMat *element;
    int el_shape;
    int operation;
};


#endif /* __cplusplus */

#endif /* _CV_HPP_ */

/* End of file. */
