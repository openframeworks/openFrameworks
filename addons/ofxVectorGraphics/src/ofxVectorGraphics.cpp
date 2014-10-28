#include "ofxVectorGraphics.h"

//we need a point class that can actually be used
//scroll down for the vectorGraphics code
class ofPoint3 : public ofPoint{

	public:
		ofPoint3(float _x = 0, float _y = 0, float _z = 0){
			x = _x;
			y = _y;
			z = _z;
		}

		void set(ofPoint3 &p){
			x = p.x;
			y = p.y;
			z = p.z;
		}
		
		void set(float _x, float _y){
			x = _x;
			y = _y;
		}
		
		void set(float _x, float _y, float _z){
			x = _x;
			y = _y;
			z = _z;
		}		
				
		bool operator==(const ofPoint3 &p) { 
			return (x == p.x) && (y == p.y) && (z == p.z);
		}

		bool operator!=(const ofPoint3 &p) { 
			return (x != p.x) || (y != p.y) || (z != p.z);
		}
    
		ofPoint3 operator+(const ofPoint3 &p) const { 
			return ofPoint3( x+p.x, y+p.y, z+p.z); 
		} 
		
		ofPoint3 operator+(const float f) const { 
			return ofPoint3( x+f, y+f, z+f );
		} 
		
		ofPoint3& operator+=(const ofPoint3& p) { 
			x += p.x;
			y += p.y;
			z += p.z;
			return *this; 
		} 
    
		ofPoint3& operator+=(const float f) { 
			x += f;
			y += f;
			z += f;
			return *this; 
		} 	
	
		ofPoint3 operator-(const ofPoint3 &p) const { 
			return ofPoint3( x-p.x, y-p.y, z-p.z ); 
		} 
		
		ofPoint3 operator-(const float f) const { 
			return ofPoint3( x-f, y-f, z-f );
		} 
		
		ofPoint3& operator-=(const ofPoint3 &p) { 
			x -= p.x;
			y -= p.y;
			z -= p.z;
			return *this; 
		} 

		ofPoint3& operator-=(const float f) { 
			x -= f;
			y -= f;
			z -= f;
			return *this; 
		} 		

		ofPoint3 operator-() const { 
			return ofPoint3( -x, -y, -z ); 
		}
		
		ofPoint3 operator*(const ofPoint3 &p) const { 
			return ofPoint3( x*p.x, y*p.y, z*p.z ); 
		} 
			
		ofPoint3& operator*=(const ofPoint3 &p) { 
			x *= p.x;
			y *= p.y;
			z *= p.z;
			return *this; 
		} 

		ofPoint3 operator*(const float f) const { 
			return ofPoint3( x*f, y*f, z*f );
		} 
		
		ofPoint3& operator*=(const float f) { 
			x *= f;
			y *= f;
			z *= f;
			return *this; 
		} 
		
		ofPoint3 operator/(const ofPoint3 &p) const { 
			return ofPoint3( x/p.x, y/p.y, z/p.z );
		} 
		
		ofPoint3& operator/=(const ofPoint3 &p) { 
			x /= p.x;
			y /= p.y;
			z /= p.z;
			return *this; 
		} 

		ofPoint3 operator/(const float f) const { 
			return ofPoint3( x/f, y/f, z/f );
		} 

		ofPoint3& operator/=(const float f) { 
			x /= f;
			y /= f;
			z /= f;
			return *this; 
		}

	union{
		struct{
		float x; 
		float y;
		float z;};
		float v[3];
	};
	
};

//----------------------------------------------------------			
ofxVectorGraphics::ofxVectorGraphics(){
	bDraw		= true;
	bFill		= true;
	bCenter		= false;
	bRecord		= false;	
	bFirstPoint = false;	
	bShouldClose= false;
	whichShapeMode = 0;		
}

//----------------------------------------------------------			
void ofxVectorGraphics::beginEPS(string fileName, int x, int y, int w, int h){
	creeps.newFile(ofToDataPath(fileName).c_str(), x, y, x+w, y+h);
	bRecord = true;
	
	//CreEPS uses lower left as (0,0)
	//so we have to flip our coordinates
	//this works fine as long
	//as the window doesn't change size 
	//while we are capturing!!!				
	creeps.applyTranslation(0, h);
	creeps.applyScaling(1, -1);
}

//----------------------------------------------------------						
void ofxVectorGraphics::endEPS(){
	creeps.resetTransformations();
	creeps.finalize();
	bRecord = false;
}

//----------------------------------------------------------			
void ofxVectorGraphics::enableDraw(){ 
	bDraw = true; 
}

//----------------------------------------------------------			
void ofxVectorGraphics::disableDraw(){ 
	bDraw = false; 
}

//----------------------------------------------------------			
void ofxVectorGraphics::enableCenterRect(){ 
	bCenter = true; 
}

//----------------------------------------------------------			
void ofxVectorGraphics::disableCenterRect(){ 
	bCenter = false; 
}

//----------------------------------------------------------			
void ofxVectorGraphics::fill(){ 
	bFill = true; ofFill(); 
}

//----------------------------------------------------------			
void ofxVectorGraphics::noFill(){ 
	bFill = false; ofNoFill();
}

//----------------------------------------------------------			
void ofxVectorGraphics::setLineWidth(float lineWidth){
	ofSetLineWidth((int)lineWidth);
	if(bRecord) creeps.setAttributes( CAtLineThickness(lineWidth) );
}

//----------------------------------------------------------
void ofxVectorGraphics::setColor(int _r, int _g, int _b){
	float r = (float)_r / 255.0f; r = MAX(0,MIN(r,1.0f));
	float g = (float)_g / 255.0f; g = MAX(0,MIN(g,1.0f));
	float b = (float)_b / 255.0f; b = MAX(0,MIN(b,1.0f));
	changeColor(r, g, b);
}

//----------------------------------------------------------
void ofxVectorGraphics::setColor(int hexColor){
	int r = (hexColor >> 16) & 0xff;
	int g = (hexColor >> 8) & 0xff;
	int b = (hexColor >> 0) & 0xff;
	setColor(r,g,b);
}

//----------------------------------------------------------
void ofxVectorGraphics::changeColor(float r, float g, float b){
	ofSetColor(ofFloatColor(r,g,b));
	if(bRecord)creeps.setAttributes( CAtColor(r, g, b) );
}

//----------------------------------------------------------		
void ofxVectorGraphics::rect(float x1,float y1, float w, float h){
	if(bDraw){
		if(bCenter)ofSetRectMode(OF_RECTMODE_CENTER);
		else ofSetRectMode(OF_RECTMODE_CORNER);
		ofDrawRectangle(x1, y1, w, h);
	}
	if(bRecord){
		if(bCenter){
			x1 -= w * 0.5;
			y1 -= h * 0.5;
		}
		
		if(bFill) creeps.rectFill(x1,  y1,  w,  h); 
		else creeps.rectStroke(x1, y1, w, h);
	}
}

//----------------------------------------------------------
void ofxVectorGraphics::triangle(float x1,float y1, float x2, float y2, float x3, float y3){
	if(bDraw){
		ofDrawTriangle(x1, y1,  x2,  y2,  x3,  y3);
	}
	if(bRecord){
		creeps.startPath(x1, y1);
		creeps.addLine(x2,  y2);
		creeps.addLine(x3,  y3);						
		creeps.closeSubpath();
		if(bFill)creeps.endPath(CreEPS::FILL);
		else creeps.endPath(CreEPS::STROKE);
	}
}	

//----------------------------------------------------------
void ofxVectorGraphics::circle(float x1,float y1, float radius){
	if(bDraw){
		ofDrawCircle(x1, y1, radius);
	}
	if(bRecord){
		if(bFill) creeps.disk(x1, y1, radius); 
		else creeps.circle(x1, y1, radius);
	}
}	

//----------------------------------------------------------
void ofxVectorGraphics::ellipse(float x1,float y1, float w, float h){
	if(bDraw){
		ofDrawEllipse(x1, y1, w, h);
	}
	if(bRecord){
		if(bFill){
			//there is no filled ellipse in non-path mode
			creeps.startPath();
			creeps.addEllipse(x1, y1, w, h);
			creeps.endPath(CreEPS::FILL);
		} 
		else creeps.ellipse(x1, y1, w, h); 
	}
}	

//----------------------------------------------------------
void ofxVectorGraphics::line(float x1,float y1, float x2, float y2){
	if(bDraw){
		ofDrawLine(x1, y1, x2, y2);
	}
	if(bRecord){
		creeps.line(x1, y1, x2, y2);
	}
}	

//-----------------------------------------------------------
void ofxVectorGraphics::arc(float x, float y, float radius, float offsetAngleDegrees, float internalAngleDegrees){
	if(bDraw){
		//no openFrameworks arc command so we make a ghetto gl one
		int		arcResolution = 20;
		float	step  = ( DEG_TO_RAD * internalAngleDegrees) / (arcResolution - 1);
		float	angle = DEG_TO_RAD * offsetAngleDegrees;
			
		float	xpos, ypos;

		if(!bFill){
	
			glBegin(GL_LINE_STRIP);
				for(int i = 0; i < arcResolution; i++){
					xpos	= cos(angle) * radius;
					ypos	= sin(angle) * radius; 
					glVertex2f(x + xpos, y + ypos);						
					angle += step;
				}
			glEnd();
		
		}else{
			glBegin(GL_TRIANGLE_STRIP);
				for(int i = 0; i < arcResolution; i++){
					xpos	= cos(angle) * radius;
					ypos	= sin(angle) * radius; 
					glVertex2f(x + xpos, y + ypos);						
					glVertex2f(x, y);														
					angle += step;
				}
			glEnd();
		}
	}
	if(bRecord){
		
		//creps works differently it likes to know absolute starting and ending angles
		float startAngle = offsetAngleDegrees;
		float endAngle   = startAngle + internalAngleDegrees;
		
		//it also only like to go from smaller to bigger in terms of rotation
		if(startAngle > endAngle){
			float tmp = endAngle;
			endAngle = startAngle;
			startAngle = tmp;
		}
		
		if(bFill){
			creeps.startPath(x, y);
			creeps.addArc(x , y, radius, startAngle,  endAngle);
			creeps.endPath(CreEPS::FILL);
		}else{
			creeps.arc(x , y, radius, startAngle,  endAngle);
		}
		
	}
}

//----------------------------------------------------------
void ofxVectorGraphics::bezier(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4){
	if(bDraw){
		ofDrawBezier(x1, y1, x2, y2, x3, y3, x4, y4);
	}
	if(bRecord){
		if(bFill){
			creeps.startPath(x1, y1);
			creeps.addCurve(x2, y2, x3, y3, x4, y4);
			creeps.addLine(x1, y1);	
			creeps.endPath(CreEPS::FILL);
		}
		else creeps.curve(x1, y1, x2, y2, x3, y3, x4, y4);
	}
}				

//----------------------------------------------------------
void ofxVectorGraphics::curve(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4){
	if(bDraw){
		ofDrawCurve(x1, y1, x2, y2, x3, y3, x4, y4);
	}
	if(bRecord){
	
		//we treat the catmull rom curve as a bezier 
		//by finding the approrpriate control points 
		//to make the same curve
													
		//x1 y1 = p1
		//x2 y2 = p2
		//x3 y3 = p3
		//x4 y4 = p4
		
		//control point at p2 is parallel 
		//to the line defined by p3 - p1
		
		//control point at p3 is parallel
		//to the line defined by p4 - p2
	
		ofPoint3 p1(x1, y1);
		ofPoint3 p2(x2, y2);
		ofPoint3 p3(x3, y3);
		ofPoint3 p4(x4, y4);
		
		//SUPER WEIRD MAGIC CONSTANT = 1/6 (this works 100% can someone explain it?)
		ofPoint3 cp1 = p2 + ( p3 - p1 ) * (1.0/6);
		ofPoint3 cp2 = p3 + ( p2 - p4 ) * (1.0/6);						
				
		creeps.startPath(x2, y2);
		creeps.addCurve( cp1.x, cp1.y, cp2.x, cp2.y, x3, y3);
		
		if(bFill){
			creeps.addLine(x2, y2);	
			creeps.endPath(CreEPS::FILL);
		}else{
			creeps.endPath(CreEPS::STROKE);
		}
	}
}	

//----------------------------------------------------------						
void ofxVectorGraphics::beginShape(){
	if(bDraw){
		ofBeginShape();
	}
	if(bRecord){
		bFirstPoint		= true;
		bShouldClose	= false;
	}
}

//----------------------------------------------------------						
void ofxVectorGraphics::polyVertex(float x, float y){
	if(bDraw){
		ofVertex(x, y);
	}
	
	if(bRecord){
	
		//clear curve vertices
		if(whichShapeMode == 1){
			clearAllVertices();	
		}

		whichShapeMode = 0;
		
		if(bFirstPoint){
			creeps.startPath(x, y);
			bFirstPoint = false;
			bShouldClose = true;
		}else{
			creeps.addLine(x,y);
		}
	}
}

//----------------------------------------------------------	
//this takes three arguments as it is based off the idea that at least one 
//inital polyVertex has already been set. 					
void ofxVectorGraphics::bezierVertex(float x1, float y1, float x2, float y2, float x3, float y3){
	if(bDraw){
		ofBezierVertex(x1, y1, x2, y2, x3, y3);
	}
	if(bRecord){
		
		//clear curve vertices
		if(whichShapeMode == 1){
			clearAllVertices();	
		}
		
		whichShapeMode = 2;					
		
		//we can only add a bezier curve if the curve
		//has started with a polyVertex
		//so as long as this is not the first point we add it
		if(!bFirstPoint){
			creeps.addCurve(x1, y1, x2, y2, x3, y3);
		}
		
	}
}			

//----------------------------------------------------------						
void ofxVectorGraphics::curveVertex(float x, float y){
	if(bDraw){
		ofCurveVertex(x, y);
	}
	if(bRecord){
	
		//if some points have already been created
		//this could crash the app 
		//so we close the current path to be safe
		if(bShouldClose){
			if(bFill) creeps.endPath(CreEPS::FILL);
			else creeps.endPath(CreEPS::STROKE);
		}
	
		whichShapeMode = 1;
	
		double* point = new double[2];
		point[0] = x;
		point[1] = y;
		curvePts.push_back(point);
	}
}


//----------------------------------------------------------						
void ofxVectorGraphics::endShape(bool bClose){
	if(bDraw){
		ofEndShape(bClose);
	}
	if(bRecord){
											
		//catmull roms - we need at least 4 points to draw
		if( whichShapeMode == 1 && curvePts.size() > 3){												
			
			//we go through and we calculate the bezier of each 
			//catmull rom curve - smart right? :)
			for (unsigned int i = 1; i< curvePts.size()-2; i++) {
				
				ofPoint3 prevPt(	curvePts[i-1][0],	curvePts[i-1][1]);
				ofPoint3 startPt(curvePts[i][0],		curvePts[i][1]);							
				ofPoint3 endPt(	curvePts[i+1][0],	curvePts[i+1][1]);
				ofPoint3 nextPt(	curvePts[i+2][0],	curvePts[i+2][1]);
				
				//SUPER WEIRD MAGIC CONSTANT = 1/6
				//Someone please explain this!!! 
				//It works and is 100% accurate but wtf!
				ofPoint3 cp1 = startPt + ( endPt - prevPt ) * (1.0/6);
				ofPoint3 cp2 = endPt + ( startPt - nextPt ) * (1.0/6);						
				
				//if this is the first line we are drawing 
				//we have to start the path at a location
				if( i == 1 ){
					creeps.startPath(startPt.x, startPt.y);
					bShouldClose = true;
				}
				
				creeps.addCurve( cp1.x, cp1.y, cp2.x, cp2.y, endPt.x, endPt.y);
			}						
		}
		
		if(bShouldClose){														
			//we close the path if requested
			if(bClose)creeps.closeSubpath();
		
			//render the stroke as either a fill or a stroke.
			if(bFill){
				creeps.endPath(CreEPS::FILL);
			}else{
				creeps.endPath(CreEPS::STROKE);
			}
			bShouldClose = false;
		}
		
		//we want to clear all the vertices
		//otherwise we keep adding points from
		//the previous file - cool but not what we want!
		clearAllVertices();
		
	}
}			


//----------------------------------------------------------
void ofxVectorGraphics::clearAllVertices(){
	for(vector<double*>::iterator itr=curvePts.begin();
		itr!=curvePts.end();
		++itr){
		delete [] (*itr);
	}
	curvePts.clear();
}			
						
