//
//  ofSize.h
//  openFrameworksLib
//
//  Created by ISHII 2bit on 2013/03/11.
//
//

#ifndef __openFrameworksLib__ofSize__
#define __openFrameworksLib__ofSize__

#include "ofConstants.h"
#include "ofLog.h"

class ofSize {
public:
    ofSize();
    virtual ~ofSize();
    ofSize(float w, float h);
    
    void set(float w, float h);
    void set(const ofSize& size);
    
    void setWidth(float w);
    void setHeight(float h);
    
    void scale(float s);
    void scale(float sX, float sY);
    void scaleWidth(float sX);
    void scaleHeight(float sY);
    
    ofSize getScaled(float s) const;
    ofSize getScaled(float sX, float sY) const;
    ofSize getScaleWidth(float sX) const;
    ofSize getScaleHeight(float sY) const;
    
    float getArea() const;
    float getPerimeter() const;
    float getAspectRatio() const;
    bool isEmpty() const;
    
    float getWidth() const;
    float getHeight() const;
    
    bool operator != (const ofSize& size) const;
    bool operator == (const ofSize& size) const;
    ofSize& operator = (const ofSize& size);
    ofSize& operator * (float s);
    
    float width;
    float height;
};

#endif /* defined(__openFrameworksLib__ofSize__) */
