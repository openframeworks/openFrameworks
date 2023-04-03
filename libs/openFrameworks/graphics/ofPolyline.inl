#ifndef OF_POLYLINE_H
#include "ofPolyline.h"
#endif

#include "ofRectangle.h"
#include "ofGraphicsBaseTypes.h"
#include "ofVectorMath.h"
#include "ofAppRunner.h"
#include "ofMath.h"
#include "ofLog.h"
#include "ofConstants.h"

//----------------------------------------------------------
template<class T>
ofPolyline_<T>::ofPolyline_(){
    setRightVector();
	clear();
}

//----------------------------------------------------------
template<class T>
ofPolyline_<T>::ofPolyline_(const std::vector<T>& verts){
    setRightVector();
	clear();
	addVertices(verts);
}

//----------------------------------------------------------
template<class T>
ofPolyline_<T> ofPolyline_<T>::fromRectangle(const ofRectangle& rect) {
	ofPolyline_ polyline;
    polyline.addVertex(rect.getMin());
    polyline.addVertex(rect.getMaxX(),rect.getMinY());
    polyline.addVertex(rect.getMax());
    polyline.addVertex(rect.getMinX(),rect.getMaxY());
    polyline.close();
    return polyline;
}

//----------------------------------------------------------
template<class T>
void ofPolyline_<T>::clear() {
	setClosed(false);
	points.clear();
	curveVertices.clear();
    flagHasChanged();
}

//----------------------------------------------------------
template<class T>
void ofPolyline_<T>::addVertex(const T& p) {
	curveVertices.clear();
	points.push_back(p);
    flagHasChanged();
}

//----------------------------------------------------------
template<class T>
void ofPolyline_<T>::addVertex(float x, float y, float z) {
	curveVertices.clear();
	addVertex(T(x,y,z));
    flagHasChanged();
}

//----------------------------------------------------------
template<class T>
void ofPolyline_<T>::addVertices(const std::vector<T>& verts) {
	curveVertices.clear();
	points.insert( points.end(), verts.begin(), verts.end() );
    flagHasChanged();
}

//----------------------------------------------------------
template<class T>
void ofPolyline_<T>::addVertices(const T* verts, int numverts) {
	curveVertices.clear();
	points.insert( points.end(), verts, verts + numverts );
    flagHasChanged();
}

//----------------------------------------------------------
template<class T>
void ofPolyline_<T>::insertVertex(const T &p, int index) {
    curveVertices.clear();
    points.insert(points.begin()+index, p);
    flagHasChanged();
}

//----------------------------------------------------------
template<class T>
void ofPolyline_<T>::insertVertex(float x, float y, float z, int index) {
	insertVertex(T(x, y, z), index);
}


//----------------------------------------------------------
template<class T>
void ofPolyline_<T>::removeVertex(int index) {
	if(index >= points.size()){
		ofLogError("ofPolyline") << "removeVertex(): ignoring out of range index " << index << ", number of vertices is" << points.size();
	}else{
		curveVertices.clear();
		points.erase(points.begin()+index);
		flagHasChanged();
	}
}


//----------------------------------------------------------
template<class T>
size_t ofPolyline_<T>::size() const {
	return points.size();
}

//----------------------------------------------------------
template<class T>
const T& ofPolyline_<T>::operator[] (int index) const {
	return points[index];
}

//----------------------------------------------------------
template<class T>
T& ofPolyline_<T>::operator[] (int index) {
    flagHasChanged();
	return points[index];
}

//----------------------------------------------------------
template<class T>
void ofPolyline_<T>::resize(size_t size){
	points.resize(size);
    flagHasChanged();
}

//----------------------------------------------------------
template<class T>
void ofPolyline_<T>::setClosed( bool tf ) {
	bClosed = tf;
    flagHasChanged();
}

//----------------------------------------------------------
template<class T>
bool ofPolyline_<T>::isClosed() const {
	return bClosed;
}

//----------------------------------------------------------
template<class T>
void ofPolyline_<T>::close(){
    setClosed(true);
}

//----------------------------------------------------------
template<class T>
bool ofPolyline_<T>::hasChanged(){
    if(bHasChanged){
        bHasChanged=false;
        return true;
    }else{
        return false;
    }
}

//----------------------------------------------------------
template<class T>
void ofPolyline_<T>::flagHasChanged() {
    bHasChanged = true;
    bCacheIsDirty = true;
}

//----------------------------------------------------------
template<class T>
std::vector<T> & ofPolyline_<T>::getVertices(){
    flagHasChanged();
	return points;
}

//----------------------------------------------------------
template<class T>
const std::vector<T> & ofPolyline_<T>::getVertices() const {
	return points;
}


//----------------------------------------------------------
template<class T>
void ofPolyline_<T>::setCircleResolution(int res){
	if (res > 1 && res != (int)circlePoints.size()){
		circlePoints.resize(res);
        
		float angle = 0.0f;
		const float angleAdder = glm::two_pi<float>() / (float)res;
		for (int i = 0; i < res; i++){
			circlePoints[i].x = cos(angle);
			circlePoints[i].y = sin(angle);
			circlePoints[i].z = 0.0f;
			angle += angleAdder;
		}
	}
}

//----------------------------------------------------------
template<class T>
// wraps any radian angle -FLT_MAX to +FLT_MAX into 0->2PI range.
// TODO, make angle treatment consistent across all functions
// should always be radians?  or should this take degrees?
// used internally, so perhaps not as important
float ofPolyline_<T>::wrapAngle(float angleRadians) {
	return ofWrap(angleRadians, 0.0f, glm::two_pi<float>());
}

//----------------------------------------------------------
template<class T>
void ofPolyline_<T>::bezierTo( const T & cp1, const T & cp2, const T & to, int curveResolution ){
	// if, and only if poly vertices has points, we can make a bezier
	// from the last point
	curveVertices.clear();
    
	// the resolultion with which we computer this bezier
	// is arbitrary, can we possibly make it dynamic?
    
	if (size() > 0){
		float x0 = points[size()-1].x;
		float y0 = points[size()-1].y;
		float z0 = points[size()-1].z;
        
		float   ax, bx, cx;
		float   ay, by, cy;
		float   az, bz, cz;
		float   t, t2, t3;
		float   x, y, z;
        
		// polynomial coefficients
		cx = 3.0f * (cp1.x - x0);
		bx = 3.0f * (cp2.x - cp1.x) - cx;
		ax = to.x - x0 - cx - bx;
        
		cy = 3.0f * (cp1.y - y0);
		by = 3.0f * (cp2.y - cp1.y) - cy;
		ay = to.y - y0 - cy - by;
        
		cz = 3.0f * (cp1.z - z0);
		bz = 3.0f * (cp2.z - cp1.z) - cz;
		az = to.z - z0 - cz - bz;
        
		for (int i = 1; i <= curveResolution; i++){
			t 	=  (float)i / (float)(curveResolution);
			t2 = t * t;
			t3 = t2 * t;
			x = (ax * t3) + (bx * t2) + (cx * t) + x0;
			y = (ay * t3) + (by * t2) + (cy * t) + y0;
			z = (az * t3) + (bz * t2) + (cz * t) + z0;
			points.emplace_back(x,y,z);
		}
	}
    flagHasChanged();
}

//----------------------------------------------------------
template<class T>
void ofPolyline_<T>::quadBezierTo(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int curveResolution){
	curveVertices.clear();
	for(int i=0; i <= curveResolution; i++){
		double t = (double)i / (double)(curveResolution);
		double a = (1.0 - t)*(1.0 - t);
		double b = 2.0 * t * (1.0 - t);
		double c = t*t;
		double x = a * x1 + b * x2 + c * x3;
		double y = a * y1 + b * y2 + c * y3;
		double z = a * z1 + b * z2 + c * z3;
		points.emplace_back(x, y, z);
	}
    flagHasChanged();
}

//----------------------------------------------------------
template<class T>
void ofPolyline_<T>::curveTo( const T & to, int curveResolution ){
    
	curveVertices.push_back(to);
    
	if (curveVertices.size() == 4){
        
		float x0 = curveVertices[0].x;
		float y0 = curveVertices[0].y;
		float z0 = curveVertices[0].z;
		float x1 = curveVertices[1].x;
		float y1 = curveVertices[1].y;
		float z1 = curveVertices[1].z;
		float x2 = curveVertices[2].x;
		float y2 = curveVertices[2].y;
		float z2 = curveVertices[2].z;
		float x3 = curveVertices[3].x;
		float y3 = curveVertices[3].y;
		float z3 = curveVertices[3].z;
        
		float t,t2,t3;
		float x,y,z;
        
		for (int i = 1; i <= curveResolution; i++){
            
			t 	=  (float)i / (float)(curveResolution);
			t2 	= t * t;
			t3 	= t2 * t;
            
			x = 0.5f * ( ( 2.0f * x1 ) +
                        ( -x0 + x2 ) * t +
                        ( 2.0f * x0 - 5.0f * x1 + 4 * x2 - x3 ) * t2 +
                        ( -x0 + 3.0f * x1 - 3.0f * x2 + x3 ) * t3 );
            
			y = 0.5f * ( ( 2.0f * y1 ) +
                        ( -y0 + y2 ) * t +
                        ( 2.0f * y0 - 5.0f * y1 + 4 * y2 - y3 ) * t2 +
                        ( -y0 + 3.0f * y1 - 3.0f * y2 + y3 ) * t3 );
            
			z = 0.5f * ( ( 2.0f * z1 ) +
                        ( -z0 + z2 ) * t +
                        ( 2.0f * z0 - 5.0f * z1 + 4 * z2 - z3 ) * t2 +
                        ( -z0 + 3.0f * z1 - 3.0f * z2 + z3 ) * t3 );
            
			points.emplace_back(x,y,z);
		}
		curveVertices.pop_front();
	}
    flagHasChanged();
}

//----------------------------------------------------------
template<class T>
void ofPolyline_<T>::arc(const T & center, float radiusX, float radiusY, float angleBegin, float angleEnd, bool clockwise, int circleResolution){
    
    if(circleResolution<=1) circleResolution=2;
    setCircleResolution(circleResolution);
    points.reserve(points.size()+circleResolution);

    const float epsilon = 0.0001f;
    
    const size_t nCirclePoints = circlePoints.size();
    float segmentArcSize  = glm::two_pi<float>() / (float)nCirclePoints;
    
    // convert angles to radians and wrap them into the range 0-M_TWO_PI and
    float angleBeginRad = wrapAngle(ofDegToRad(angleBegin));
    float angleEndRad =   wrapAngle(ofDegToRad(angleEnd));
    
    while(angleBeginRad >= angleEndRad) angleEndRad += glm::two_pi<float>();
    
    // determine the directional angle delta
    float d = clockwise ? angleEndRad - angleBeginRad : angleBeginRad - angleEndRad;
    // find the shortest angle delta, clockwise delta direction yeilds POSITIVE values
    float deltaAngle = atan2(sin(d),cos(d));
    
    // establish the remaining angle that we have to work through
    float remainingAngle = deltaAngle;
    
    // if the delta angle is in the CCW direction OR the start and stop angles are
    // effectively the same adjust the remaining angle to be a be a full rotation
    if(deltaAngle < 0 || std::abs(deltaAngle) < epsilon) remainingAngle += glm::two_pi<float>();
    
	T radii(radiusX, radiusY, 0.f);
	T point(0);
    
    int currentLUTIndex = 0;
    bool isFirstPoint = true; // special case for the first point
    
    while(remainingAngle > 0) {
        if(isFirstPoint) {
            // TODO: should this be the exact point on the circle or
            // should it be an intersecting point on the line that connects two
            // surrounding LUT points?
            //
            // get the EXACT first point requested (for points that
            // don't fall precisely on a LUT entry)
			point = T(cos(angleBeginRad), sin(angleBeginRad), 0.f);
            // set up the get any in between points from the LUT
            float ratio = angleBeginRad / glm::two_pi<float>() * (float)nCirclePoints;
            currentLUTIndex = clockwise ? (int)ceil(ratio) : (int)floor(ratio);
            float lutAngleAtIndex = currentLUTIndex * segmentArcSize;
            // the angle between the beginning angle and the next angle in the LUT table
            float d = clockwise ? (lutAngleAtIndex - angleBeginRad) : (angleBeginRad - lutAngleAtIndex);
            float firstPointDelta = atan2(sin(d),cos(d)); // negative is in the clockwise direction
            
            // if the are "equal", get the next one CCW
            if(std::abs(firstPointDelta) < epsilon) {
                currentLUTIndex = clockwise ? (currentLUTIndex + 1) : (currentLUTIndex - 1);
                firstPointDelta = segmentArcSize; // we start at the next lut point
            }
            
            // start counting from the offset
            remainingAngle -= firstPointDelta;
            isFirstPoint = false;
        } else {
			point = T(circlePoints[currentLUTIndex].x, circlePoints[currentLUTIndex].y, 0.f);
            if(clockwise) {
                currentLUTIndex++; // go to the next LUT point
                remainingAngle -= segmentArcSize; // account for next point
                // if the angle overshoots, then the while loop will fail next time
            } else {
                currentLUTIndex--; // go to the next LUT point
                remainingAngle -= segmentArcSize; // account for next point
                // if the angle overshoots, then the while loop will fail next time
            }
        }
        
        // keep the current lut index in range
        if(clockwise) {
            currentLUTIndex = currentLUTIndex % nCirclePoints;
        } else {
            if(currentLUTIndex < 0) currentLUTIndex = nCirclePoints + currentLUTIndex;
        }
        
        // add the point to the poly line
        point = point * radii + center;
        points.push_back(point);
        
        // if the next LUT point moves us past the end angle then
        // add a a point a the exact end angle and call it finished
        if(remainingAngle < epsilon) {
			point = T(cos(angleEndRad), sin(angleEndRad), 0.f);
            point = point * radii + center;
            points.push_back(point);
            remainingAngle = 0; // call it finished, the next while loop test will fail
        }
    }
    flagHasChanged();
}

//----------------------------------------------------------
template<class T>
float ofPolyline_<T>::getPerimeter() const {
    if(points.size() < 2) {
        return 0;
    } else {
        updateCache();
        return lengths.back();
    }
}

//----------------------------------------------------------
template<class T>
float ofPolyline_<T>::getArea() const{
    updateCache();
    return area;
}

//----------------------------------------------------------
template<class T>
T ofPolyline_<T>::getCentroid2D() const{
    updateCache();
    return centroid2D;
}

//----------------------------------------------------------
template<class T>
ofRectangle ofPolyline_<T>::getBoundingBox() const {
    
	ofRectangle box;
    for(size_t i = 0; i < size(); i++) {
        if(i == 0) {
            box.set(points[i],0,0);
        } else {
            box.growToInclude(points[i]);
        }
    }
	return box;
}

//----------------------------------------------------------
template<class T>
ofPolyline_<T> ofPolyline_<T>::getSmoothed(int smoothingSize, float smoothingShape) const {
	int n = size();
	smoothingSize = ofClamp(smoothingSize, 0, n);
	smoothingShape = ofClamp(smoothingShape, 0, 1);
	
	// precompute weights and normalization
	std::vector<float> weights;
	weights.resize(smoothingSize);
	// side weights
	for(int i = 1; i < smoothingSize; i++) {
		float curWeight = ofMap(i, 0, smoothingSize, 1, smoothingShape);
		weights[i] = curWeight;
	}
	
	// make a copy of this polyline
	ofPolyline_ result = *this;
	
	for(int i = 0; i < n; i++) {
		float sum = 1; // center weight
		for(int j = 1; j < smoothingSize; j++) {
			T cur(0);
			int leftPosition = i - j;
			int rightPosition = i + j;
			if(leftPosition < 0 && bClosed) {
				leftPosition += n;
			}
			if(leftPosition >= 0) {
				cur += points[leftPosition];
				sum += weights[j];
			}
			if(rightPosition >= n && bClosed) {
				rightPosition -= n;
			}
			if(rightPosition < n) {
				cur += points[rightPosition];
				sum += weights[j];
			}
			result[i] += cur * weights[j];
		}
		result[i] /= sum;
	}
	
	return result;
}

//----------------------------------------------------------
template<class T>
ofPolyline_<T> ofPolyline_<T>::getResampledBySpacing(float spacing) const {
    if(spacing==0 || size() == 0) return *this;
	ofPolyline_ poly;
    float totalLength = getPerimeter();
    float f=0;
    for(f=0; f<=totalLength; f += spacing) {
        poly.lineTo(getPointAtLength(f));
    }
    
    if(!isClosed()) {
        if( f != totalLength ){
            poly.lineTo(points.back());
        }
        poly.setClosed(false);
    } else {
        poly.setClosed(true);
    }
    
    return poly;
}

//----------------------------------------------------------
template<class T>
ofPolyline_<T> ofPolyline_<T>::getResampledByCount(int count) const {
	float perimeter = getPerimeter();
	if(count < 2) {
		ofLogWarning("ofPolyline_") << "getResampledByCount(): requested " << count <<" points, using minimum count of 2 ";
		count = 2;
    }
	return ofPolyline_<T>::getResampledBySpacing(perimeter / (count-1));
}

//----------------------------------------------------------
// http://local.wasp.uwa.edu.au/~pbourke/geometry/pointline/
template<class T>
inline T getClosestPointUtil(const T& p1, const T& p2, const T& p3, float* normalizedPosition) {
	// if p1 is coincident with p2, there is no line
	if(p1 == p2) {
		if(normalizedPosition != nullptr) {
			*normalizedPosition = 0;
		}
		return p1;
	}
	
	float u = (p3.x - p1.x) * (p2.x - p1.x);
	u += (p3.y - p1.y) * (p2.y - p1.y);
	// perfect place for fast inverse sqrt...
	float len = glm::length(toGlm(p2 - p1));
	u /= (len * len);
	
	// clamp u
	if(u > 1) {
		u = 1;
	} else if(u < 0) {
		u = 0;
	}
	if(normalizedPosition != nullptr) {
		*normalizedPosition = u;
	}
	return glm::mix(toGlm(p1), toGlm(p2), u);
}

//----------------------------------------------------------
template<class T>
// a much faster but less accurate version would check distances to vertices first,
// which assumes vertices are evenly spaced
T ofPolyline_<T>::getClosestPoint(const T& target, unsigned int* nearestIndex) const {
	const ofPolyline_ & polyline = *this;
    
	if(polyline.size() < 2) {
		if(nearestIndex != nullptr) {
			nearestIndex = 0;
		}
		return target;
	}
	
	float distance = 0;
	T nearestPoint(0);
	unsigned int nearest = 0;
	float normalizedPosition = 0;
	unsigned int lastPosition = polyline.size() - 1;
	if(polyline.isClosed()) {
		lastPosition++;
	}
	for(int i = 0; i < (int) lastPosition; i++) {
		bool repeatNext = i == (int) (polyline.size() - 1);
		
		const auto& cur = polyline[i];
		const auto& next = repeatNext ? polyline[0] : polyline[i + 1];
		
		float curNormalizedPosition = 0;
		auto curNearestPoint = getClosestPointUtil(cur, next, target, &curNormalizedPosition);
		float curDistance = glm::distance(toGlm(curNearestPoint), toGlm(target));
		if(i == 0 || curDistance < distance) {
			distance = curDistance;
			nearest = i;
			nearestPoint = curNearestPoint;
			normalizedPosition = curNormalizedPosition;
		}
	}
	
	if(nearestIndex != nullptr) {
		if(normalizedPosition > .5) {
			nearest++;
			if(nearest == polyline.size()) {
				nearest = 0;
			}
		}
		*nearestIndex = nearest;
	}
	
	return nearestPoint;
}

//--------------------------------------------------
template<class T>
bool ofPolyline_<T>::inside(const T & p, const ofPolyline_ & polyline){
	return ofPolyline_<T>::inside(p.x,p.y,polyline);
}

//--------------------------------------------------
template<class T>
bool ofPolyline_<T>::inside(float x, float y, const ofPolyline_ & polyline){
	int counter = 0;
	int i;
	double xinters;
	T p1,p2;
    
	int N = polyline.size();
    
	p1 = polyline[0];
	for (i=1;i<=N;i++) {
		p2 = polyline[i % N];
		if (y > std::min(p1.y,p2.y)) {
            if (y <= std::max(p1.y,p2.y)) {
                if (x <= std::max(p1.x,p2.x)) {
                    if (p1.y != p2.y) {
                        xinters = (y-p1.y)*(p2.x-p1.x)/(p2.y-p1.y)+p1.x;
                        if (p1.x == p2.x || x <= xinters)
                            counter++;
                    }
                }
            }
		}
		p1 = p2;
	}
    
	if (counter % 2 == 0) return false;
	else return true;
}

//--------------------------------------------------
template<class T>
bool ofPolyline_<T>::inside(float x, float y) const {
	return ofPolyline_<T>::inside(x, y, *this);
    
}

//--------------------------------------------------
template<class T>
bool ofPolyline_<T>::inside(const T & p) const {
	return ofPolyline_<T>::inside(p, *this);
}



//--------------------------------------------------
namespace of{
	namespace priv{

		//This is for polygon/contour simplification - we use it to reduce the number of points needed in
		//representing the letters as openGL shapes - will soon be moved to ofGraphics.cpp

		// From: http://softsurfer.com/Archive/algorithm_0205/algorithm_0205.htm
		// Copyright 2002, softSurfer (www.softsurfer.com)
		// This code may be freely used and modified for any purpose
		// providing that this copyright notice is included with it.
		// SoftSurfer makes no warranty for this code, and cannot be held
		// liable for any real or imagined damage resulting from its use.
		// Users of this code must verify correctness for their application.
		template<class T>
		struct Segment{
			T P0;
			T P1;
		};

		template<class T>
		static void simplifyDP(float tol, T* v, int j, int k, int* mk ){
			if (k <= j+1) // there is nothing to simplify
				return;

			// check for adequate approximation by segment S from v[j] to v[k]
			int     maxi	= j;          // index of vertex farthest from S
			float   maxd2	= 0;         // distance squared of farthest vertex
			float   tol2	= tol * tol;  // tolerance squared
			Segment<T> S	= {v[j], v[k]};  // segment from v[j] to v[k]
			auto u			= S.P1 - S.P0;   // segment direction vector
			double  cu		= glm::dot(toGlm(u), toGlm(u));     // segment length squared

			// test each vertex v[i] for max distance from S
			// compute using the Feb 2001 Algorithm's dist_ofPoint_to_Segment()
			// Note: this works in any dimension (2D, 3D, ...)
			T  w;
			T  Pb;                // base of perpendicular from v[i] to S
			float  b, cw, dv2;        // dv2 = distance v[i] to S squared

			for (int i=j+1; i<k; i++){
				// compute distance squared
				w = v[i] - S.P0;
				cw = glm::dot(toGlm(w), toGlm(u));
				if ( cw <= 0 ) dv2 = glm::length2(toGlm(v[i]) - toGlm(S.P0));
				else if ( cu <= cw ) dv2 = glm::length2(toGlm(v[i]) - toGlm(S.P1));
				else {
					b = (float)(cw / cu);
					Pb = S.P0 + u*b;
					dv2 = glm::length2(toGlm(v[i]) - toGlm(Pb));
				}
				// test with current max distance squared
				if (dv2 <= maxd2) continue;

				// v[i] is a new max vertex
				maxi = i;
				maxd2 = dv2;
			}
			if (maxd2 > tol2)        // error is worse than the tolerance
			{
				// split the polyline at the farthest vertex from S
				mk[maxi] = 1;      // mark v[maxi] for the simplified polyline
				// recursively simplify the two subpolylines at v[maxi]
				simplifyDP( tol, v, j, maxi, mk );  // polyline v[j] to v[maxi]
				simplifyDP( tol, v, maxi, k, mk );  // polyline v[maxi] to v[k]
			}
			// else the approximation is OK, so ignore intermediate vertices
		}
	}
}

//--------------------------------------------------
template<class T>
void ofPolyline_<T>::simplify(float tol){
    if(points.size() < 2) return;
    
	int n = size();
	
	if(n == 0) {
		return;
	}

	std::vector <T> sV;
	sV.resize(n);
    
    int    i, k, m, pv;            // misc counters
    float  tol2 = tol * tol;       // tolerance squared
    std::vector<T> vt;
    std::vector<int> mk;
    vt.resize(n);
	mk.resize(n,0);
    
    
    // STAGE 1.  Vertex Reduction within tolerance of prior vertex cluster
    vt[0] = points[0];              // start at the beginning
    for (i=k=1, pv=0; i<n; i++) {
		if (glm::length2((const glm::vec3&)points[i] - (const glm::vec3&)points[pv]) < tol2) continue;
        
        vt[k++] = points[i];
        pv = i;
    }
    if (pv < n-1) vt[k++] = points[n-1];      // finish at the end
    
    // STAGE 2.  Douglas-Peucker polyline simplification
    mk[0] = mk[k-1] = 1;       // mark the first and last vertices
	of::priv::simplifyDP( tol, &vt[0], 0, k-1, &mk[0] );
    
    // copy marked vertices to the output simplified polyline
    for (i=m=0; i<k; i++) {
        if (mk[i]) sV[m++] = vt[i];
    }
    
	//get rid of the unused points
	if( m < (int)sV.size() ){
		points.assign( sV.begin(),sV.begin()+m );
	}else{
		points = sV;
	}
    
}

//--------------------------------------------------
template<class T>
void ofPolyline_<T>::translate(const glm::vec3 & p){
    for(auto & point : points){
        point += p;
    }
    flagHasChanged();
}

//--------------------------------------------------
template<class T>
void ofPolyline_<T>::translate(const glm::vec2 &p){
    translate(glm::vec3(p, 0.0));
}

//--------------------------------------------------
template<class T>
void ofPolyline_<T>::rotateDeg(float degrees, const glm::vec3& axis){
    rotateRad(ofDegToRad(degrees), axis);
}

//--------------------------------------------------
template<class T>
void ofPolyline_<T>::rotateRad(float radians, const glm::vec3& axis){
    for(auto & point : points){
        point = glm::rotate(toGlm(point), radians, axis);
    }
    flagHasChanged();
}

//--------------------------------------------------
template<class T>
void ofPolyline_<T>::rotate(float degrees, const glm::vec3 &axis){
    rotateDeg(degrees, axis);
}

//--------------------------------------------------
template<class T>
void ofPolyline_<T>::rotateDeg(float degrees, const glm::vec2& axis){
    rotateRad(ofDegToRad(degrees), glm::vec3(axis, 0.0));
}

//--------------------------------------------------
template<class T>
void ofPolyline_<T>::rotateRad(float radians, const glm::vec2& axis){
    rotateRad(radians, glm::vec3(axis, 0.0));
}

//--------------------------------------------------
template<class T>
void ofPolyline_<T>::rotate(float degrees, const glm::vec2 &axis){
    rotateRad(ofDegToRad(degrees), glm::vec3(axis, 0.0));
}

//--------------------------------------------------
template<class T>
void ofPolyline_<T>::scale(float x, float y){
    for(auto & point : points){
        point.x *= x;
        point.y *= y;
    }
    flagHasChanged();
}

//--------------------------------------------------
template<class T>
void ofPolyline_<T>::draw() const{
	ofGetCurrentRenderer()->draw(*this);
}

//--------------------------------------------------
template<class T>
void ofPolyline_<T>::setRightVector(T v) {
    rightVector = v;
    flagHasChanged();
}

//--------------------------------------------------
template<class T>
T ofPolyline_<T>::getRightVector() const {
    return rightVector;
}

//--------------------------------------------------
template<class T>
float ofPolyline_<T>::getIndexAtLength(float length) const {
    if(points.size() < 2) return 0;
    updateCache();
    
    float totalLength = getPerimeter();
    length = ofClamp(length, 0, totalLength);
    
    int lastPointIndex = isClosed() ? points.size() : points.size()-1;
    
    int i1 = ofClamp(floor(length / totalLength * lastPointIndex), 0, lengths.size()-2);   // start approximation here
    int leftLimit = 0;
    int rightLimit = lastPointIndex;
    
    float distAt1, distAt2;
    for(int iterations = 0; iterations < 32; iterations ++) {	// limit iterations
        i1 = ofClamp(i1, 0, lengths.size()-1);
        distAt1 = lengths[i1];
        if(distAt1 <= length) {         // if Length at i1 is less than desired Length (this is good)
            distAt2 = lengths[i1+1];
            if(distAt2 >= length) {
                float t = ofMap(length, distAt1, distAt2, 0, 1);
                return i1 + t;
            } else {
                leftLimit = i1;
            }
        } else {
            rightLimit = i1;
        }
        i1 = (leftLimit + rightLimit)/2;
    }
    return 0;
}


//--------------------------------------------------
template<class T>
float ofPolyline_<T>::getIndexAtPercent(float f) const {
    return getIndexAtLength(f * getPerimeter());
}

//--------------------------------------------------
template<class T>
float ofPolyline_<T>::getLengthAtIndex(int index) const {
    if(points.size() < 2) return 0;
    updateCache();
    return lengths[getWrappedIndex(index)];
}

//--------------------------------------------------
template<class T>
float ofPolyline_<T>::getLengthAtIndexInterpolated(float findex) const {
    if(points.size() < 2) return 0;
    updateCache();
    int i1, i2;
    float t;
    getInterpolationParams(findex, i1, i2, t);
    return ofLerp(getLengthAtIndex(i1), getLengthAtIndex(i2), t);
}


//--------------------------------------------------
template<class T>
T ofPolyline_<T>::getPointAtLength(float f) const {
	if(points.size() < 2) return T();
    updateCache();
    return getPointAtIndexInterpolated(getIndexAtLength(f));
}

//--------------------------------------------------
template<class T>
T ofPolyline_<T>::getPointAtPercent(float f) const {
    float length = getPerimeter();
    return getPointAtLength(f * length);
}


//--------------------------------------------------
template<class T>
T ofPolyline_<T>::getPointAtIndexInterpolated(float findex) const {
	if(points.size() < 2) return T();
    int i1, i2;
    float t;
    getInterpolationParams(findex, i1, i2, t);
	T leftPoint(points[i1]);
	T rightPoint(points[i2]);
	return glm::mix(toGlm(leftPoint), toGlm(rightPoint), t);
}


//--------------------------------------------------
template<class T>
float ofPolyline_<T>::getAngleAtIndex(int index) const {
	return getDegreesAtIndex(index);
}

//--------------------------------------------------
template<class T>
float ofPolyline_<T>::getAngleAtIndexInterpolated(float findex) const {
	return getDegreesAtIndexInterpolated(findex);
}


//--------------------------------------------------
template<class T>
float ofPolyline_<T>::getDegreesAtIndex(int index) const {
	if(points.size() < 2) return 0;
	updateCache();
	return ofRadToDeg(angles[getWrappedIndex(index)]);
}

//--------------------------------------------------
template<class T>
float ofPolyline_<T>::getDegreesAtIndexInterpolated(float findex) const {
	if(points.size() < 2) return 0;
	int i1, i2;
	float t;
	getInterpolationParams(findex, i1, i2, t);
	return ofRadToDeg(ofLerp(getDegreesAtIndex(i1), getDegreesAtIndex(i2), t));
}


//--------------------------------------------------
template<class T>
float ofPolyline_<T>::getRadiansAtIndex(int index) const {
	if(points.size() < 2) return 0;
	updateCache();
	return angles[getWrappedIndex(index)];
}

//--------------------------------------------------
template<class T>
float ofPolyline_<T>::getRadiansAtIndexInterpolated(float findex) const {
	if(points.size() < 2) return 0;
	int i1, i2;
	float t;
	getInterpolationParams(findex, i1, i2, t);
	return ofLerp(getRadiansAtIndex(i1), getRadiansAtIndex(i2), t);
}

//--------------------------------------------------
template<class T>
T ofPolyline_<T>::getRotationAtIndex(int index) const {
	if(points.size() < 2) return T();
    updateCache();
    return rotations[getWrappedIndex(index)];
}

//--------------------------------------------------
template<class T>
T ofPolyline_<T>::getRotationAtIndexInterpolated(float findex) const {
	if(points.size() < 2) return T();
    int i1, i2;
    float t;
    getInterpolationParams(findex, i1, i2, t);
	return glm::mix(toGlm(getRotationAtIndex(i1)), toGlm(getRotationAtIndex(i2)), t);
}

//--------------------------------------------------
template<class T>
T ofPolyline_<T>::getTangentAtIndex(int index) const {
	if(points.size() < 2) return T();
    updateCache();
    return tangents[getWrappedIndex(index)];
}

//--------------------------------------------------
template<class T>
T ofPolyline_<T>::getTangentAtIndexInterpolated(float findex) const {
	if(points.size() < 2) return T();
    int i1, i2;
    float t;
    getInterpolationParams(findex, i1, i2, t);
	return glm::mix(toGlm(getTangentAtIndex(i1)), toGlm(getTangentAtIndex(i2)), t);
}

//--------------------------------------------------
template<class T>
T ofPolyline_<T>::getNormalAtIndex(int index) const {
	if(points.size() < 2) return T();
    updateCache();
    return normals[getWrappedIndex(index)];
}

//--------------------------------------------------
template<class T>
T ofPolyline_<T>::getNormalAtIndexInterpolated(float findex) const {
	if(points.size() < 2) return T();
    int i1, i2;
    float t;
    getInterpolationParams(findex, i1, i2, t);
	return glm::mix(toGlm(getNormalAtIndex(i1)), toGlm(getNormalAtIndex(i2)), t);
}


//--------------------------------------------------
template<class T>
void ofPolyline_<T>::calcData(int index, T &tangent, float &angle, T &rotation, T &normal) const {
	int i1 = getWrappedIndex( index - 1 );
	int i2 = getWrappedIndex( index     );
	int i3 = getWrappedIndex( index + 1 );

	const auto &p1 = toGlm(points[i1]);
	const auto &p2 = toGlm(points[i2]);
	const auto &p3 = toGlm(points[i3]);

	auto v1(p1 - p2); // vector to previous point
	auto v2(p3 - p2); // vector to next point
	
	v1 = glm::normalize(v1);
	v2 = glm::normalize(v2);

	// If just one of p1, p2, or p3 was identical, further calculations 
	// are (almost literally) pointless, as v1 or v2 will then contain 
	// NaN values instead of floats.

	bool noSegmentHasZeroLength = (v1 == v1 && v2 == v2);

	if ( noSegmentHasZeroLength ){
		tangent  = toOf( glm::length2(v2 - v1) > 0 ? glm::normalize(v2 - v1) : -v1 );
		normal   = toOf( glm::normalize( glm::cross( toGlm( rightVector ), toGlm( tangent ) ) ) );
		rotation = toOf( glm::cross( v1, v2 ) );
		angle    = glm::pi<float>() - acosf( ofClamp( glm::dot( v1, v2 ), -1.f, 1.f ) );
	} else{
		rotation = tangent = normal = T( 0.f );
		angle    = 0.f;
	}
}


//--------------------------------------------------
template<class T>
int ofPolyline_<T>::getWrappedIndex(int index) const {
    if(points.empty()) return 0;
    
    if(index < 0) return isClosed() ? (index + points.size()) % points.size() : 0;
    if(index > int(points.size())-1) return isClosed() ? index % points.size() : points.size() - 1;
    return index;
}

//--------------------------------------------------
template<class T>
void ofPolyline_<T>::getInterpolationParams(float findex, int &i1, int &i2, float &t) const {
    i1 = floor(findex);
    t = findex - i1;
    i1 = getWrappedIndex(i1);
    i2 = getWrappedIndex(i1 + 1);
}

//--------------------------------------------------
template<class T>
void ofPolyline_<T>::updateCache(bool bForceUpdate) const {
    if(bCacheIsDirty || bForceUpdate) {
        lengths.clear();
        angles.clear();
        rotations.clear();
        normals.clear();
        tangents.clear();
        area = 0;
		centroid2D = {0.f, 0.f, 0.f};
        bCacheIsDirty = false;
        
        if(points.size() < 2) return;

        // area
        for(int i=0;i<(int)points.size()-1;i++){
            area += points[i].x * points[i+1].y - points[i+1].x * points[i].y;
        }
        area += points[points.size()-1].x * points[0].y - points[0].x * points[points.size()-1].y;
        area *= 0.5;
        
        if(fabsf(area) < std::numeric_limits<float>::epsilon()) {
            centroid2D = getBoundingBox().getCenter();
        } else {
            // centroid
            // TODO: doesn't seem to work on all concave shapes
            for(int i=0;i<(int)points.size()-1;i++){
                centroid2D.x += (points[i].x + points[i+1].x) * (points[i].x*points[i+1].y - points[i+1].x*points[i].y);
                centroid2D.y += (points[i].y + points[i+1].y) * (points[i].x*points[i+1].y - points[i+1].x*points[i].y);
            }
            centroid2D.x += (points[points.size()-1].x + points[0].x) * (points[points.size()-1].x*points[0].y - points[0].x*points[points.size()-1].y);
            centroid2D.y += (points[points.size()-1].y + points[0].y) * (points[points.size()-1].x*points[0].y - points[0].x*points[points.size()-1].y);
            
            centroid2D.x /= (6*area);
            centroid2D.y /= (6*area);
        }

        
        // per vertex cache
        lengths.resize(points.size());
        tangents.resize(points.size());
        angles.resize(points.size());
        normals.resize(points.size());
        rotations.resize(points.size());
        
        float angle;
		T rotation;
		T normal;
		T tangent;

        float length = 0;
        for(int i=0; i<(int)points.size(); i++) {
            lengths[i] = length;

            calcData(i, tangent, angle, rotation, normal);
            tangents[i] = tangent;
            angles[i] = angle;
            rotations[i] = rotation;
            normals[i] = normal;
            
			length += glm::distance(toGlm(points[i]), toGlm(points[getWrappedIndex(i + 1)]));
        }
        
        if(isClosed()) lengths.push_back(length);
    }
}


//--------------------------------------------------
template<class T>
typename std::vector<T>::iterator ofPolyline_<T>::begin(){
	return points.begin();
}

//--------------------------------------------------
template<class T>
typename std::vector<T>::iterator ofPolyline_<T>::end(){
	return points.end();
}

//--------------------------------------------------
template<class T>
typename std::vector<T>::const_iterator ofPolyline_<T>::begin() const{
	return points.begin();
}

//--------------------------------------------------
template<class T>
typename std::vector<T>::const_iterator ofPolyline_<T>::end() const{
	return points.end();
}

//--------------------------------------------------
template<class T>
typename std::vector<T>::reverse_iterator ofPolyline_<T>::rbegin(){
	return points.rbegin();
}

//--------------------------------------------------
template<class T>
typename std::vector<T>::reverse_iterator ofPolyline_<T>::rend(){
	return points.rend();
}

//--------------------------------------------------
template<class T>
typename std::vector<T>::const_reverse_iterator ofPolyline_<T>::rbegin() const{
	return points.rbegin();
}

//--------------------------------------------------
template<class T>
typename std::vector<T>::const_reverse_iterator ofPolyline_<T>::rend() const{
	return points.rend();
}

