#ifndef _OF_TYPES
#define _OF_TYPES

#include "ofConstants.h"
#include "ofUtils.h"

//----------------------------------------------------------
// ofPoint
//----------------------------------------------------------
class ofPoint {
  public:
	virtual ~ofPoint(){}

    ofPoint( float _x=0.0f, float _y=0.0f, float _z=0.0f ) {
        x = _x;
        y = _y;
        z = _z;
    }

    ofPoint( const ofPoint & pnt){
        x = pnt.x;
        y = pnt.y;
        z = pnt.z;
    }

    void set(float _x, float _y, float _z = 0){
        x = _x;
        y = _y;
        z = _z;
    }


	//------ Operators:

  	//Negative
    ofPoint operator-() const {
        return ofPoint( -x, -y, -z );
    }

    //equality
    bool operator==( const ofPoint& pnt ) {
        return (x == pnt.x) && (y == pnt.y) && (z == pnt.z);
    }

	//inequality
    bool operator!=( const ofPoint& pnt ) {
        return (x != pnt.x) || (y != pnt.y) || (z != pnt.z);
    }

	//Set
	ofPoint & operator=( const ofPoint& pnt ){
		x = pnt.x;
		y = pnt.y;
		z = pnt.z;
		return *this;
	}

	ofPoint & operator=( const float& val ){
		x = val;
		y = val;
		z = val;
		return *this;
	}

	// Add
    ofPoint operator+( const ofPoint& pnt ) const {
        return ofPoint( x+pnt.x, y+pnt.y, z+pnt.z );
    }

    ofPoint operator+( const float& val ) const {
        return ofPoint( x+val, y+val, z+val );
    }

	ofPoint & operator+=( const ofPoint& pnt ) {
        x+=pnt.x;
        y+=pnt.y;
        z+=pnt.z;
        return *this;
    }

	ofPoint & operator+=( const float & val ) {
        x+=val;
        y+=val;
        z+=val;
        return *this;
    }

	// Subtract
    ofPoint operator-(const ofPoint& pnt) const {
        return ofPoint( x-pnt.x, y-pnt.y, z-pnt.z );
    }

    ofPoint operator-(const float& val) const {
        return ofPoint( x-val, y-val, z-val);
    }

    ofPoint & operator-=( const ofPoint& pnt ) {
        x -= pnt.x;
        y -= pnt.y;
        z -= pnt.z;
        return *this;
    }

    ofPoint & operator-=( const float & val ) {
        x -= val;
        y -= val;
        z -= val;
        return *this;
    }

	// Multiply
    ofPoint operator*( const ofPoint& pnt ) const {
        return ofPoint( x*pnt.x, y*pnt.y, z*pnt.z );
    }

    ofPoint operator*(const float& val) const {
        return ofPoint( x*val, y*val, z*val);
    }

    ofPoint & operator*=( const ofPoint& pnt ) {
        x*=pnt.x;
        y*=pnt.y;
        z*=pnt.z;
        return *this;
    }

    ofPoint & operator*=( const float & val ) {
        x*=val;
        y*=val;
        z*=val;
        return *this;
    }

	// Divide
    ofPoint operator/( const ofPoint& pnt ) const {
        return ofPoint( pnt.x!=0 ? x/pnt.x : x , pnt.y!=0 ? y/pnt.y : y, pnt.z!=0 ? z/pnt.z : z );
    }

    ofPoint operator/( const float &val ) const {
		if( val != 0){
			return ofPoint( x/val, y/val, z/val );
		}
        return ofPoint(x, y, z );
    }

    ofPoint& operator/=( const ofPoint& pnt ) {
        pnt.x!=0 ? x/=pnt.x : x;
        pnt.y!=0 ? y/=pnt.y : y;
        pnt.z!=0 ? z/=pnt.z : z;

        return *this;
    }

    ofPoint& operator/=( const float &val ) {
		if( val != 0 ){
			x /= val;
			y /= val;
			z /= val;
		}

		return *this;
    }

	// union allows us to access the coordinates through
    // both an array 'v' and 'x', 'y', 'z' member varibles
    union  {
        struct {
            float x;
            float y;
            float z;
        };
        float v[3];
    };

	float& operator[]( int n ){
		return v[n];
	}
};

//----------------------------------------------------------
// ofRectangle
//----------------------------------------------------------

class ofRectangle {
  public:
	ofRectangle(){ x = y = width = height = 0; };
	ofRectangle(float _x, float _y, float _w, float _h){
		x = _x;
		y = _y;
		width = _w;
		height = _h;
	}
	virtual ~ofRectangle(){}

	void set(float px, float py, float w, float h){
		x		= px;
		y		= py;
		width	= w;
		height	= h;
	}

	void set(ofPoint pos, float w, float h){
		x		= pos.x;
		y		= pos.y;
		width	= w;
		height	= h;
	}

	void setFromCenter(float px, float py, float w, float h){
		x		= px - w*0.5f;
		y		= py - h*0.5f;
		width	= w;
		height	= h;
	}

	void setFromCenter(ofPoint pos, float w, float h){
		x		= pos.x - w*0.5f;
		y		= pos.y - h*0.5f;
		width	= w;
		height	= h;
	}
	
	ofPoint getCenter(){
		return ofPoint(x + width * 0.5f, y + height * 0.5f, 0);
	}

	bool inside (ofPoint p){
		return inside(p.x, p.y);
	}

	bool inside(float px, float py){
		if( px < x && py < y && px > x + width && py > y + height ){
		    return false;
		}
		return true;
	}

   float x;
   float y;
   float width;
   float height;

};



//----------------------------------------------------------
// ofColor
//----------------------------------------------------------

class ofColor{
	public:
		ofColor(){
			r = 255;
			g = 255;
			b = 255;
			a = 255;
		}
		virtual ~ofColor(){}
		float r, g, b, a;
	
		float & operator[]( int n ){
			switch( n ){
				case 0:
					return r;
				case 1:
					return g;
				case 2:
					return b;
				case 3: 
					return a;
				default:
					return r;
				break;
			}
		}
};

//----------------------------------------------------------
// ofStyle
//----------------------------------------------------------

class ofStyle{
	public:
		ofStyle(){
			bFill				= true;
			blending			= false;
			smoothing			= false;
			circleResolution	= 20;
			lineWidth			= 1.0;
			polyMode			= OF_POLY_WINDING_ODD;
			rectMode			= OF_RECTMODE_CORNER;
		}

		virtual ~ofStyle(){}

		ofColor color;
		int polyMode;
		int rectMode;
		bool bFill;
		bool blending;
		bool smoothing;
		int circleResolution;
		float lineWidth;
};

//----------------------------------------------------------
// ofBuffer
//----------------------------------------------------------

class ofBuffer{
public:
	long 	size;
	char * 	buffer;

	ofBuffer(){
		size 	= 0;
		buffer 	= NULL;
	}

	ofBuffer(const string & path){
		readFile(path);
	}

	ofBuffer(int _size, char * _buffer){
		size 	= _size;
		buffer 	= _buffer;
	}

	bool readFile(const string & path){
		ifstream * file = new ifstream(ofToDataPath(path,true).c_str());

		if(!file || !file->is_open()){
			size   = 0;
			buffer = NULL;
			ofLog(OF_LOG_ERROR, "couldn't open " + path);
			return false;
		}

		filebuf *pbuf=file->rdbuf();

		// get file size using buffer's members
		size = (long)pbuf->pubseekoff (0,ios::end,ios::in);
		pbuf->pubseekpos (0,ios::in);

		// get file data
		buffer = new char[size];
		pbuf->sgetn (buffer,size);
		return true;
	}

	~ofBuffer(){
		if(buffer) delete[] buffer;
	}
};


//----------------------------------------------------------
// ofBaseDraws
//----------------------------------------------------------

class ofBaseDraws{
public:
	virtual ~ofBaseDraws(){}
	virtual void draw(float x,float y)=0;
	virtual void draw(float x,float y,float w, float h)=0;
	virtual float getHeight()=0;
	virtual float getWidth()=0;

	virtual void setAnchorPercent(float xPct, float yPct){};
    virtual void setAnchorPoint(float x, float y){};
	virtual void resetAnchor(){};

};

//----------------------------------------------------------
// ofBaseUpdates
//----------------------------------------------------------

class ofBaseUpdates{
public:
	virtual ~ofBaseUpdates(){}
	virtual void update()=0;
};


//----------------------------------------------------------
// ofBaseHasTexture
//----------------------------------------------------------
class ofTexture;

class ofBaseHasTexture{
public:
	virtual ~ofBaseHasTexture(){}
	virtual ofTexture & getTextureReference()=0;
	virtual void setUseTexture(bool bUseTex)=0;
};

//----------------------------------------------------------
// ofBaseHasPixels
//----------------------------------------------------------
class ofBaseHasPixels{
public:
	virtual ~ofBaseHasPixels(){}
	virtual unsigned char * getPixels()=0;
};

//----------------------------------------------------------
// ofBaseImage
//----------------------------------------------------------
class ofBaseImage: public ofBaseDraws, public ofBaseHasTexture, public ofBaseHasPixels{
public:

};

//----------------------------------------------------------
// ofBaseVideo
//----------------------------------------------------------
class ofBaseVideo: public ofBaseImage, public ofBaseUpdates{
public:
	virtual ~ofBaseVideo(){}
	virtual unsigned char * getPixels()=0;
	virtual bool isFrameNew()=0;
	virtual void close()=0;
};
#endif
