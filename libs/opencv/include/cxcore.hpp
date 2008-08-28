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


#ifndef _CXCORE_HPP_
#define _CXCORE_HPP_

class CV_EXPORTS CvImage
{
public:
    CvImage() : image(0), refcount(0) {}
    CvImage( CvSize size, int depth, int channels )
    {
        image = cvCreateImage( size, depth, channels );
        refcount = image ? new int(1) : 0;
    }

    CvImage( IplImage* img ) : image(img)
    {
        refcount = image ? new int(1) : 0;
    }

    CvImage( const CvImage& img ) : image(img.image), refcount(img.refcount)
    {
        if( refcount ) ++(*refcount);
    }

    CvImage( const char* filename, const char* imgname=0, int color=-1 ) : image(0), refcount(0)
    { load( filename, imgname, color ); }

    CvImage( CvFileStorage* fs, const char* mapname, const char* imgname ) : image(0), refcount(0)
    { read( fs, mapname, imgname ); }

    CvImage( CvFileStorage* fs, const char* seqname, int idx ) : image(0), refcount(0)
    { read( fs, seqname, idx ); }

    ~CvImage()
    {
        if( refcount && !(--*refcount) )
        {
            cvReleaseImage( &image );
            delete refcount;
        }
    }

    CvImage clone() { return CvImage(image ? cvCloneImage(image) : 0); }

    void create( CvSize size, int depth, int channels )
    {
        attach( cvCreateImage( size, depth, channels ));
    }

    void release() { detach(); }
    void clear() { detach(); }

    void attach( IplImage* img, bool use_refcount=true )
    {
        if( refcount )
        {
            if( --*refcount == 0 )
                cvReleaseImage( &image );
            delete refcount;
        }
        image = img;
        refcount = use_refcount && image ? new int(1) : 0;
    }

    void detach()
    {
        if( refcount )
        {
            if( --*refcount == 0 )
                cvReleaseImage( &image );
            delete refcount;
            refcount = 0;
        }
        image = 0;
    }

    bool load( const char* filename, const char* imgname=0, int color=-1 );
    bool read( CvFileStorage* fs, const char* mapname, const char* imgname );
    bool read( CvFileStorage* fs, const char* seqname, int idx );
    void save( const char* filename, const char* imgname );
    void write( CvFileStorage* fs, const char* imgname );

    void show( const char* window_name );
    bool is_valid() { return image != 0; }

    int width() const { return image ? image->width : 0; }
    int height() const { return image ? image->height : 0; }

    CvSize size() const { return image ? cvSize(image->width, image->height) : cvSize(0,0); }

    CvSize roi_size() const
    {
        return !image ? cvSize(0,0) :
            !image->roi ? cvSize(image->width,image->height) :
            cvSize(image->roi->width, image->roi->height);
    }

    CvRect roi() const
    {
        return !image ? cvRect(0,0,0,0) :
            !image->roi ? cvRect(0,0,image->width,image->height) :
            cvRect(image->roi->xOffset,image->roi->yOffset,
                   image->roi->width,image->roi->height);
    }

    int coi() const { return !image || !image->roi ? 0 : image->roi->coi; }

    void set_roi(CvRect roi) { cvSetImageROI(image,roi); }
    void reset_roi() { cvResetImageROI(image); }
    void set_coi(int coi) { cvSetImageCOI(image,coi); }
    int depth() const { return image ? image->depth : 0; }
    int channels() const { return image ? image->nChannels : 0; }
    int pix_size() const { return image ? ((image->depth & 255)>>3)*image->nChannels : 0; }

    uchar* data() { return image ? (uchar*)image->imageData : 0; }
    const uchar* data() const { return image ? (const uchar*)image->imageData : 0; }
    int step() const { return image ? image->widthStep : 0; }
    int origin() const { return image ? image->origin : 0; }

    uchar* roi_row(int y)
    {
        assert(0<=y);
        assert(!image ?
                1 : image->roi ?
                y<image->roi->height : y<image->height);
        
        return !image ? 0 :
            !image->roi ?
                (uchar*)(image->imageData + y*image->widthStep) :
                (uchar*)(image->imageData + (y+image->roi->yOffset)*image->widthStep +
                image->roi->xOffset*((image->depth & 255)>>3)*image->nChannels);
    }

    const uchar* roi_row(int y) const
    {
        assert(0<=y);
        assert(!image ?
                1 : image->roi ?
                y<image->roi->height : y<image->height); 

        return !image ? 0 :
            !image->roi ?
                (const uchar*)(image->imageData + y*image->widthStep) :
                (const uchar*)(image->imageData + (y+image->roi->yOffset)*image->widthStep +
                image->roi->xOffset*((image->depth & 255)>>3)*image->nChannels);
    }

    operator const IplImage* () const { return image; }
    operator IplImage* () { return image; }

    CvImage& operator = (const CvImage& img)
    {
        if( img.refcount )
            ++*img.refcount;
        if( refcount && !(--*refcount) )
            cvReleaseImage( &image );
        image=img.image;
        refcount=img.refcount;
        return *this;
    }

protected:
    IplImage* image;
    int* refcount;
};


class CV_EXPORTS CvMatrix
{
public:
    CvMatrix() : matrix(0) {}
    CvMatrix( int rows, int cols, int type )
    { matrix = cvCreateMat( rows, cols, type ); }

    CvMatrix( int rows, int cols, int type, CvMat* hdr,
              void* data=0, int step=CV_AUTOSTEP )
    { matrix = cvInitMatHeader( hdr, rows, cols, type, data, step ); }

    CvMatrix( int rows, int cols, int type, CvMemStorage* storage, bool alloc_data=true );

    CvMatrix( int rows, int cols, int type, void* data, int step=CV_AUTOSTEP )
    { matrix = cvCreateMatHeader( rows, cols, type );
      cvSetData( matrix, data, step ); }

    CvMatrix( CvMat* m )
    { matrix = m; }

    CvMatrix( const CvMatrix& m )
    {
        matrix = m.matrix;
        addref();
    }

    CvMatrix( const char* filename, const char* matname=0, int color=-1 ) : matrix(0)
    {  load( filename, matname, color ); }

    CvMatrix( CvFileStorage* fs, const char* mapname, const char* matname ) : matrix(0)
    {  read( fs, mapname, matname ); }

    CvMatrix( CvFileStorage* fs, const char* seqname, int idx ) : matrix(0)
    {  read( fs, seqname, idx ); }

    ~CvMatrix()
    {
        release();
    }

    CvMatrix clone() { return CvMatrix(matrix ? cvCloneMat(matrix) : 0); }

    void set( CvMat* m, bool add_ref )
    {
        release();
        matrix = m;
        if( add_ref )
            addref();
    }

    void create( int rows, int cols, int type )
    {
        set( cvCreateMat( rows, cols, type ), false );
    }

    void addref() const
    {
        if( matrix )
        {
            if( matrix->hdr_refcount )
                ++matrix->hdr_refcount;
            else if( matrix->refcount )
                ++*matrix->refcount;
        }
    }

    void release()
    {
        if( matrix )
        {
            if( matrix->hdr_refcount )
            {
                if( --matrix->hdr_refcount == 0 )
                    cvReleaseMat( &matrix );
            }
            else if( matrix->refcount )
            {
                if( --*matrix->refcount == 0 )
                    cvFree( &matrix->refcount );
            }
            matrix = 0;
        }
    }

    void clear()
    {
        release();
    }

    bool load( const char* filename, const char* matname=0, int color=-1 );
    bool read( CvFileStorage* fs, const char* mapname, const char* matname );
    bool read( CvFileStorage* fs, const char* seqname, int idx );
    void save( const char* filename, const char* matname );
    void write( CvFileStorage* fs, const char* matname );

    void show( const char* window_name );

    bool is_valid() { return matrix != 0; }

    int rows() const { return matrix ? matrix->rows : 0; }
    int cols() const { return matrix ? matrix->cols : 0; }

    CvSize size() const
    {
        return !matrix ? cvSize(0,0) : cvSize(matrix->rows,matrix->cols);
    }

    int type() const { return matrix ? CV_MAT_TYPE(matrix->type) : 0; }
    int depth() const { return matrix ? CV_MAT_DEPTH(matrix->type) : 0; }
    int channels() const { return matrix ? CV_MAT_CN(matrix->type) : 0; }
    int pix_size() const { return matrix ? CV_ELEM_SIZE(matrix->type) : 0; }

    uchar* data() { return matrix ? matrix->data.ptr : 0; }
    const uchar* data() const { return matrix ? matrix->data.ptr : 0; }
    int step() const { return matrix ? matrix->step : 0; }

    void set_data( void* data, int step=CV_AUTOSTEP )
    { cvSetData( matrix, data, step ); }

    uchar* row(int i) { return !matrix ? 0 : matrix->data.ptr + i*matrix->step; }
    const uchar* row(int i) const
    { return !matrix ? 0 : matrix->data.ptr + i*matrix->step; }

    operator const CvMat* () const { return matrix; }
    operator CvMat* () { return matrix; }

    CvMatrix& operator = (const CvMatrix& _m)
    {
        _m.addref();
        release();
        matrix = _m.matrix;
        return *this;
    }

protected:
    CvMat* matrix;
};


typedef IplImage* (CV_CDECL * CvLoadImageFunc)( const char* filename, int colorness );
typedef CvMat* (CV_CDECL * CvLoadImageMFunc)( const char* filename, int colorness );
typedef int (CV_CDECL * CvSaveImageFunc)( const char* filename, const CvArr* image );
typedef void (CV_CDECL * CvShowImageFunc)( const char* windowname, const CvArr* image );

CVAPI(int) cvSetImageIOFunctions( CvLoadImageFunc _load_image, CvLoadImageMFunc _load_image_m,
                            CvSaveImageFunc _save_image, CvShowImageFunc _show_image );

#define CV_SET_IMAGE_IO_FUNCTIONS() \
    cvSetImageIOFunctions( cvLoadImage, cvLoadImageM, cvSaveImage, cvShowImage )

// classes for automatic module/RTTI data registration/unregistration
struct CV_EXPORTS CvModule
{
    CvModule( CvModuleInfo* _info );
    ~CvModule();
    CvModuleInfo* info;

    static CvModuleInfo* first;
    static CvModuleInfo* last;
};

struct CV_EXPORTS CvType
{
    CvType( const char* type_name,
            CvIsInstanceFunc is_instance, CvReleaseFunc release=0,
            CvReadFunc read=0, CvWriteFunc write=0, CvCloneFunc clone=0 );
    ~CvType();
    CvTypeInfo* info;

    static CvTypeInfo* first;
    static CvTypeInfo* last;
};

#endif /*_CXCORE_HPP_*/
