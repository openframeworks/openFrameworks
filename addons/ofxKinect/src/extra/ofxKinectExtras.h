#pragma once
#include <iostream>

#define OFX_KINECT_EXTRA_FW 

class ofxKinectExtras{

    public:
        static int getFWSize1473();
        static unsigned char * getFWData1473();
    
        static int getFWSizek4w();
        static unsigned char * getFWDatak4w();
};
