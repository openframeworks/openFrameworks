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

#ifndef __CVAUX_HPP__
#define __CVAUX_HPP__

#ifdef __cplusplus

/****************************************************************************************\
*                                      Image class                                       *
\****************************************************************************************/

class CV_EXPORTS CvCamShiftTracker
{
public:
    
    CvCamShiftTracker();
    virtual ~CvCamShiftTracker();
    
    /**** Characteristics of the object that are calculated by track_object method *****/
    float   get_orientation() const // orientation of the object in degrees 
    { return m_box.angle; }
    float   get_length() const // the larger linear size of the object
    { return m_box.size.height; }
    float   get_width() const // the smaller linear size of the object
    { return m_box.size.width; }
    CvPoint2D32f get_center() const // center of the object
    { return m_box.center; }
    CvRect get_window() const // bounding rectangle for the object
    { return m_comp.rect; }
    
    /*********************** Tracking parameters ************************/
    int     get_threshold() const // thresholding value that applied to back project
    { return m_threshold; }

    int     get_hist_dims( int* dims = 0 ) const // returns number of histogram dimensions and sets
    { return m_hist ? cvGetDims( m_hist->bins, dims ) : 0; } 
    
    int     get_min_ch_val( int channel ) const // get the minimum allowed value of the specified channel
    { return m_min_ch_val[channel]; }
    
    int     get_max_ch_val( int channel ) const // get the maximum allowed value of the specified channel
    { return m_max_ch_val[channel]; }
    
    // set initial object rectangle (must be called before initial calculation of the histogram)
    bool    set_window( CvRect window)
    { m_comp.rect = window; return true; }
    
    bool    set_threshold( int threshold ) // threshold applied to the histogram bins
    { m_threshold = threshold; return true; }

    bool    set_hist_bin_range( int dim, int min_val, int max_val );

    bool    set_hist_dims( int c_dims, int* dims );// set the histogram parameters
    
    bool    set_min_ch_val( int channel, int val ) // set the minimum allowed value of the specified channel
    { m_min_ch_val[channel] = val; return true; }
    bool    set_max_ch_val( int channel, int val ) // set the maximum allowed value of the specified channel
    { m_max_ch_val[channel] = val; return true; }

    /************************ The processing methods *********************************/
    // update object position
    virtual bool  track_object( const IplImage* cur_frame );
    
    // update object histogram
    virtual bool  update_histogram( const IplImage* cur_frame );

    // reset histogram
    virtual void  reset_histogram();

    /************************ Retrieving internal data *******************************/
    // get back project image
    virtual IplImage* get_back_project()
    { return m_back_project; }

    float query( int* bin ) const
    { return m_hist ? cvQueryHistValue_nD( m_hist, bin ) : 0.f; }
    
protected:

    // internal method for color conversion: fills m_color_planes group
    virtual void color_transform( const IplImage* img ); 

    CvHistogram* m_hist;

    CvBox2D    m_box;
    CvConnectedComp m_comp;

    float      m_hist_ranges_data[CV_MAX_DIM][2];
    float*     m_hist_ranges[CV_MAX_DIM];

    int        m_min_ch_val[CV_MAX_DIM];
    int        m_max_ch_val[CV_MAX_DIM];
    int        m_threshold;

    IplImage*  m_color_planes[CV_MAX_DIM];
    IplImage*  m_back_project;
    IplImage*  m_temp;
    IplImage*  m_mask;
};

#endif /* __cplusplus */

#endif /* __CVAUX_HPP__ */

/* End of file. */
