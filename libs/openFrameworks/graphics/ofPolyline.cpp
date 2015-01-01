#include "ofPolyline.h"
#include "ofAppRunner.h"

//----------------------------------------------------------
ofPolyline::ofPolyline(){
    setRightVector();
	clear();
}

//----------------------------------------------------------
ofPolyline::ofPolyline(const vector<ofPoint>& verts){
    setRightVector();
	clear();
	addVertices(verts);
}

//----------------------------------------------------------
ofPolyline ofPolyline::fromRectangle(const ofRectangle& rect) {
    ofPolyline polyline;
    polyline.addVertex(rect.getMin());
    polyline.addVertex(rect.getMaxX(),rect.getMinY());
    polyline.addVertex(rect.getMax());
    polyline.addVertex(rect.getMinX(),rect.getMaxY());
    polyline.close();
    return polyline;
}

//----------------------------------------------------------
void ofPolyline::clear() {
	setClosed(false);
	points.clear();
	curveVertices.clear();
    flagHasChanged();
}

//----------------------------------------------------------
void ofPolyline::addVertex(const ofPoint& p) {
	curveVertices.clear();
	points.push_back(p);
    flagHasChanged();
}

//----------------------------------------------------------
void ofPolyline::addVertex(float x, float y, float z) {
	curveVertices.clear();
	addVertex(ofPoint(x,y,z));
    flagHasChanged();
}

//----------------------------------------------------------
void ofPolyline::addVertices(const vector<ofPoint>& verts) {
	curveVertices.clear();
	points.insert( points.end(), verts.begin(), verts.end() );
    flagHasChanged();
}

//----------------------------------------------------------
void ofPolyline::addVertices(const ofPoint* verts, int numverts) {
	curveVertices.clear();
	points.insert( points.end(), verts, verts + numverts );
    flagHasChanged();
}

//----------------------------------------------------------
void ofPolyline::insertVertex(const ofPoint &p, int index) {
    curveVertices.clear();
    points.insert(points.begin()+index, p);
    flagHasChanged();
}

//----------------------------------------------------------
void ofPolyline::insertVertex(float x, float y, float z, int index) {
    insertVertex(ofPoint(x, y, z), index);
}


//----------------------------------------------------------
size_t ofPolyline::size() const {
	return points.size();
}

//----------------------------------------------------------
const ofPoint& ofPolyline::operator[] (int index) const {
	return points[index];
}

//----------------------------------------------------------
ofPoint& ofPolyline::operator[] (int index) {
    flagHasChanged();
	return points[index];
}

//----------------------------------------------------------
void ofPolyline::resize(size_t size){
	points.resize(size);
    flagHasChanged();
}

//----------------------------------------------------------
void ofPolyline::setClosed( bool tf ) {
	bClosed = tf;
    flagHasChanged();
}

//----------------------------------------------------------
bool ofPolyline::isClosed() const {
	return bClosed;
}

//----------------------------------------------------------
void ofPolyline::close(){
    setClosed(true);
}

//----------------------------------------------------------
bool ofPolyline::hasChanged(){
    if(bHasChanged){
        bHasChanged=false;
        return true;
    }else{
        return false;
    }
}

//----------------------------------------------------------
void ofPolyline::flagHasChanged() {
    bHasChanged = true;
    bCacheIsDirty = true;
}

//----------------------------------------------------------
vector<ofPoint> & ofPolyline::getVertices(){
    flagHasChanged();
	return points;
}

//----------------------------------------------------------
const vector<ofPoint> & ofPolyline::getVertices() const {
	return points;
}


//----------------------------------------------------------
void ofPolyline::setCircleResolution(int res){
	if (res > 1 && res != (int)circlePoints.size()){
		circlePoints.resize(res);
        
		float angle = 0.0f;
		const float angleAdder = M_TWO_PI / (float)res;
		for (int i = 0; i < res; i++){
			circlePoints[i].x = cos(angle);
			circlePoints[i].y = sin(angle);
			circlePoints[i].z = 0.0f;
			angle += angleAdder;
		}
	}
}

//----------------------------------------------------------
// wraps any radian angle -FLT_MAX to +FLT_MAX into 0->2PI range.
// TODO, make angle treatment consistent across all functions
// should always be radians?  or should this take degrees?
// used internally, so perhaps not as important
float ofPolyline::wrapAngle(float angleRadians) {
	return ofWrap(angleRadians, 0.0f, TWO_PI);
}

//----------------------------------------------------------
void ofPolyline::bezierTo( const ofPoint & cp1, const ofPoint & cp2, const ofPoint & to, int curveResolution ){
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
        
		for (int i = 0; i < curveResolution; i++){
			t 	=  (float)i / (float)(curveResolution-1);
			t2 = t * t;
			t3 = t2 * t;
			x = (ax * t3) + (bx * t2) + (cx * t) + x0;
			y = (ay * t3) + (by * t2) + (cy * t) + y0;
			z = (az * t3) + (bz * t2) + (cz * t) + z0;
			points.push_back(ofPoint(x,y,z));
		}
	}
    flagHasChanged();
}

//----------------------------------------------------------
void ofPolyline::quadBezierTo(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int curveResolution){
	curveVertices.clear();
	for(int i=0; i <= curveResolution; i++){
		double t = (double)i / (double)(curveResolution);
		double a = (1.0 - t)*(1.0 - t);
		double b = 2.0 * t * (1.0 - t);
		double c = t*t;
		double x = a * x1 + b * x2 + c * x3;
		double y = a * y1 + b * y2 + c * y3;
		double z = a * z1 + b * z2 + c * z3;
		points.push_back(ofPoint(x, y, z));
	}
    flagHasChanged();
}

//----------------------------------------------------------
void ofPolyline::curveTo( const ofPoint & to, int curveResolution ){
    
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
        
		for (int i = 0; i < curveResolution; i++){
            
			t 	=  (float)i / (float)(curveResolution-1);
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
            
			points.push_back(ofPoint(x,y,z));
		}
		curveVertices.pop_front();
	}
    flagHasChanged();
}

//----------------------------------------------------------
void ofPolyline::arc(const ofPoint & center, float radiusX, float radiusY, float angleBegin, float angleEnd, bool clockwise, int circleResolution){
    
    if(circleResolution<=1) circleResolution=2;
    setCircleResolution(circleResolution);
    points.reserve(points.size()+circleResolution);

    const float epsilon = 0.0001f;
    
    const size_t nCirclePoints = circlePoints.size();
    float segmentArcSize  = M_TWO_PI / (float)nCirclePoints;
    
    // convert angles to radians and wrap them into the range 0-M_TWO_PI and
    float angleBeginRad = wrapAngle(ofDegToRad(angleBegin));
    float angleEndRad =   wrapAngle(ofDegToRad(angleEnd));
    
    while(angleBeginRad >= angleEndRad) angleEndRad += M_TWO_PI;
    
    // determine the directional angle delta
    float d = clockwise ? angleEndRad - angleBeginRad : angleBeginRad - angleEndRad;
    // find the shortest angle delta, clockwise delta direction yeilds POSITIVE values
    float deltaAngle = atan2(sin(d),cos(d));
    
    // establish the remaining angle that we have to work through
    float remainingAngle = deltaAngle;
    
    // if the delta angle is in the CCW direction OR the start and stop angles are
    // effectively the same adjust the remaining angle to be a be a full rotation
    if(deltaAngle < 0 || abs(deltaAngle) < epsilon) remainingAngle += M_TWO_PI;
    
    ofPoint radii(radiusX,radiusY);
    ofPoint point;
    
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
            point = ofPoint(cos(angleBeginRad),sin(angleBeginRad));
            // set up the get any in between points from the LUT
            float ratio = angleBeginRad / M_TWO_PI * (float)nCirclePoints;
            currentLUTIndex = clockwise ? (int)ceil(ratio) : (int)floor(ratio);
            float lutAngleAtIndex = currentLUTIndex * segmentArcSize;
            // the angle between the beginning angle and the next angle in the LUT table
            float d = clockwise ? (lutAngleAtIndex - angleBeginRad) : (angleBeginRad - lutAngleAtIndex);
            float firstPointDelta = atan2(sin(d),cos(d)); // negative is in the clockwise direction
            
            // if the are "equal", get the next one CCW
            if(abs(firstPointDelta) < epsilon) {
                currentLUTIndex = clockwise ? (currentLUTIndex + 1) : (currentLUTIndex - 1);
                firstPointDelta = segmentArcSize; // we start at the next lut point
            }
            
            // start counting from the offset
            remainingAngle -= firstPointDelta;
            isFirstPoint = false;
        } else {
            point = ofPoint(circlePoints[currentLUTIndex].x,circlePoints[currentLUTIndex].y);
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
            point = ofPoint(cos(angleEndRad),sin(angleEndRad));
            point = point * radii + center;
            points.push_back(point);
            remainingAngle = 0; // call it finished, the next while loop test will fail
        }
    }
    flagHasChanged();
}

//----------------------------------------------------------
float ofPolyline::getPerimeter() const {
    if(points.size() < 2) {
        return 0;
    } else {
        updateCache();
        return lengths.back();
    }
}

//----------------------------------------------------------
float ofPolyline::getArea() const{
    updateCache();
    return area;
}

//----------------------------------------------------------
ofPoint ofPolyline::getCentroid2D() const{
    updateCache();
    return centroid2D;
}

//----------------------------------------------------------
ofRectangle ofPolyline::getBoundingBox() const {
    
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
ofPolyline ofPolyline::getSmoothed(int smoothingSize, float smoothingShape) const {
	int n = size();
	smoothingSize = ofClamp(smoothingSize, 0, n);
	smoothingShape = ofClamp(smoothingShape, 0, 1);
	
	// precompute weights and normalization
	vector<float> weights;
	weights.resize(smoothingSize);
	// side weights
	for(int i = 1; i < smoothingSize; i++) {
		float curWeight = ofMap(i, 0, smoothingSize, 1, smoothingShape);
		weights[i] = curWeight;
	}
	
	// make a copy of this polyline
	ofPolyline result = *this;
	
	for(int i = 0; i < n; i++) {
		float sum = 1; // center weight
		for(int j = 1; j < smoothingSize; j++) {
			ofVec3f cur;
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
ofPolyline ofPolyline::getResampledBySpacing(float spacing) const {
    if(spacing==0 || size() == 0) return *this;
    ofPolyline poly;
    float totalLength = getPerimeter();
    for(float f=0; f<totalLength; f += spacing) {
        poly.lineTo(getPointAtLength(f));
    }
    
    if(!isClosed()) {
        if(poly.size() > 0) poly.points.back() = points.back();
        poly.setClosed(false);
    } else {
        poly.setClosed(true);
    }
    
    return poly;
}

//----------------------------------------------------------
ofPolyline ofPolyline::getResampledByCount(int count) const {
	float perimeter = getPerimeter();
	if(count < 2) {
		ofLogWarning("ofPolyline") << "getResampledByCount(): requested " << count <<" points, using minimum count of 2 ";
		count = 2;
    }
	return ofPolyline::getResampledBySpacing(perimeter / (count-1));
}

//----------------------------------------------------------
// http://local.wasp.uwa.edu.au/~pbourke/geometry/pointline/
static ofPoint getClosestPointUtil(const ofPoint& p1, const ofPoint& p2, const ofPoint& p3, float* normalizedPosition) {
	// if p1 is coincident with p2, there is no line
	if(p1 == p2) {
		if(normalizedPosition != NULL) {
			*normalizedPosition = 0;
		}
		return p1;
	}
	
	float u = (p3.x - p1.x) * (p2.x - p1.x);
	u += (p3.y - p1.y) * (p2.y - p1.y);
	// perfect place for fast inverse sqrt...
	float len = (p2 - p1).length();
	u /= (len * len);
	
	// clamp u
	if(u > 1) {
		u = 1;
	} else if(u < 0) {
		u = 0;
	}
	if(normalizedPosition != NULL) {
		*normalizedPosition = u;
	}
	return p1.getInterpolated(p2, u);
}

//----------------------------------------------------------
// a much faster but less accurate version would check distances to vertices first,
// which assumes vertices are evenly spaced
ofPoint ofPolyline::getClosestPoint(const ofPoint& target, unsigned int* nearestIndex) const {
	const ofPolyline & polyline = *this;
    
	if(polyline.size() < 2) {
		if(nearestIndex != NULL) {
			nearestIndex = 0;
		}
		return target;
	}
	
	float distance = 0;
	ofPoint nearestPoint;
	unsigned int nearest = 0;
	float normalizedPosition = 0;
	unsigned int lastPosition = polyline.size() - 1;
	if(polyline.isClosed()) {
		lastPosition++;
	}
	for(int i = 0; i < (int) lastPosition; i++) {
		bool repeatNext = i == (int) (polyline.size() - 1);
		
		const ofPoint& cur = polyline[i];
		const ofPoint& next = repeatNext ? polyline[0] : polyline[i + 1];
		
		float curNormalizedPosition = 0;
		ofPoint curNearestPoint = getClosestPointUtil(cur, next, target, &curNormalizedPosition);
		float curDistance = curNearestPoint.distance(target);
		if(i == 0 || curDistance < distance) {
			distance = curDistance;
			nearest = i;
			nearestPoint = curNearestPoint;
			normalizedPosition = curNormalizedPosition;
		}
	}
	
	if(nearestIndex != NULL) {
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
bool ofPolyline::inside(const ofPoint & p, const ofPolyline & polyline){
	return ofPolyline::inside(p.x,p.y,polyline);
}

//--------------------------------------------------
bool ofPolyline::inside(float x, float y, const ofPolyline & polyline){
	int counter = 0;
	int i;
	double xinters;
	ofPoint p1,p2;
    
	int N = polyline.size();
    
	p1 = polyline[0];
	for (i=1;i<=N;i++) {
		p2 = polyline[i % N];
		if (y > MIN(p1.y,p2.y)) {
            if (y <= MAX(p1.y,p2.y)) {
                if (x <= MAX(p1.x,p2.x)) {
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
bool ofPolyline::inside(float x, float y) const {
    return ofPolyline::inside(x, y, *this);
    
}

//--------------------------------------------------
bool ofPolyline::inside(const ofPoint & p) const {
    return ofPolyline::inside(p, *this);
}

//This is for polygon/contour simplification - we use it to reduce the number of points needed in
//representing the letters as openGL shapes - will soon be moved to ofGraphics.cpp

// From: http://softsurfer.com/Archive/algorithm_0205/algorithm_0205.htm
// Copyright 2002, softSurfer (www.softsurfer.com)
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.

typedef struct{
	ofPoint P0;
	ofPoint P1;
}Segment;

// dot product (3D) which allows vector operations in arguments
#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)
#define norm2(v)   dot(v,v)        // norm2 = squared length of vector
#define norm(v)    sqrt(norm2(v))  // norm = length of vector
#define d2(u,v)    norm2(u-v)      // distance squared = norm2 of difference
#define d(u,v)     norm(u-v)       // distance = norm of difference

//--------------------------------------------------
static void simplifyDP(float tol, ofPoint* v, int j, int k, int* mk ){
    if (k <= j+1) // there is nothing to simplify
        return;
    
    // check for adequate approximation by segment S from v[j] to v[k]
    int     maxi	= j;          // index of vertex farthest from S
    float   maxd2	= 0;         // distance squared of farthest vertex
    float   tol2	= tol * tol;  // tolerance squared
    Segment S		= {v[j], v[k]};  // segment from v[j] to v[k]
    ofPoint u;
	u				= S.P1 - S.P0;   // segment direction vector
    double  cu		= dot(u,u);     // segment length squared
    
    // test each vertex v[i] for max distance from S
    // compute using the Feb 2001 Algorithm's dist_ofPoint_to_Segment()
    // Note: this works in any dimension (2D, 3D, ...)
    ofPoint  w;
    ofPoint   Pb;                // base of perpendicular from v[i] to S
    float  b, cw, dv2;        // dv2 = distance v[i] to S squared
    
    for (int i=j+1; i<k; i++){
        // compute distance squared
        w = v[i] - S.P0;
        cw = dot(w,u);
        if ( cw <= 0 ) dv2 = d2(v[i], S.P0);
        else if ( cu <= cw ) dv2 = d2(v[i], S.P1);
        else {
            b = (float)(cw / cu);
            Pb = S.P0 + u*b;
            dv2 = d2(v[i], Pb);
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
    return;
}

//--------------------------------------------------
void ofPolyline::simplify(float tol){
    if(points.size() < 2) return;
    
	int n = size();
	
	if(n == 0) {
		return;
	}

	vector <ofPoint> sV;
	sV.resize(n);
    
    int    i, k, m, pv;            // misc counters
    float  tol2 = tol * tol;       // tolerance squared
    vector<ofPoint> vt;
    vector<int> mk;
    vt.resize(n);
	mk.resize(n,0);
    
    
    // STAGE 1.  Vertex Reduction within tolerance of prior vertex cluster
    vt[0] = points[0];              // start at the beginning
    for (i=k=1, pv=0; i<n; i++) {
        if (d2(points[i], points[pv]) < tol2) continue;
        
        vt[k++] = points[i];
        pv = i;
    }
    if (pv < n-1) vt[k++] = points[n-1];      // finish at the end
    
    // STAGE 2.  Douglas-Peucker polyline simplification
    mk[0] = mk[k-1] = 1;       // mark the first and last vertices
    simplifyDP( tol, &vt[0], 0, k-1, &mk[0] );
    
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
void ofPolyline::draw() const{
	ofGetCurrentRenderer()->draw(*this);
}

//--------------------------------------------------
void ofPolyline::setRightVector(ofVec3f v) {
    rightVector = v;
    flagHasChanged();
}

//--------------------------------------------------
ofVec3f ofPolyline::getRightVector() const {
    return rightVector;
}


//--------------------------------------------------
float ofPolyline::getIndexAtLength(float length) const {
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
float ofPolyline::getIndexAtPercent(float f) const {
    return getIndexAtLength(f * getPerimeter());
}

//--------------------------------------------------
float ofPolyline::getLengthAtIndex(int index) const {
    if(points.size() < 2) return 0;
    updateCache();
    return lengths[getWrappedIndex(index)];
}

//--------------------------------------------------
float ofPolyline::getLengthAtIndexInterpolated(float findex) const {
    if(points.size() < 2) return 0;
    updateCache();
    int i1, i2;
    float t;
    getInterpolationParams(findex, i1, i2, t);
    return ofLerp(getLengthAtIndex(i1), getLengthAtIndex(i2), t);
}


//--------------------------------------------------
ofPoint ofPolyline::getPointAtLength(float f) const {
    if(points.size() < 2) return ofPoint();
    updateCache();
    return getPointAtIndexInterpolated(getIndexAtLength(f));
}

//--------------------------------------------------
ofPoint ofPolyline::getPointAtPercent(float f) const {
    float length = getPerimeter();
    return getPointAtLength(f * length);
}


//--------------------------------------------------
ofPoint ofPolyline::getPointAtIndexInterpolated(float findex) const {
    if(points.size() < 2) return ofPoint();
    int i1, i2;
    float t;
    getInterpolationParams(findex, i1, i2, t);
    ofPoint leftPoint(points[i1]);
    ofPoint rightPoint(points[i2]);
    return leftPoint.getInterpolated(rightPoint, t);
}


//--------------------------------------------------
float ofPolyline::getAngleAtIndex(int index) const {
    if(points.size() < 2) return 0;
    updateCache();
    return angles[getWrappedIndex(index)];
}

//--------------------------------------------------
float ofPolyline::getAngleAtIndexInterpolated(float findex) const {
    if(points.size() < 2) return 0;
    int i1, i2;
    float t;
    getInterpolationParams(findex, i1, i2, t);
    return ofLerp(getAngleAtIndex(i1), getAngleAtIndex(i2), t);
}

//--------------------------------------------------
ofVec3f ofPolyline::getRotationAtIndex(int index) const {
    if(points.size() < 2) return ofVec3f();
    updateCache();
    return rotations[getWrappedIndex(index)];
}

//--------------------------------------------------
ofVec3f ofPolyline::getRotationAtIndexInterpolated(float findex) const {
    if(points.size() < 2) return ofVec3f();
    int i1, i2;
    float t;
    getInterpolationParams(findex, i1, i2, t);
    return getRotationAtIndex(i1).getInterpolated(getRotationAtIndex(i2), t);
}

//--------------------------------------------------
ofVec3f ofPolyline::getTangentAtIndex(int index) const {
    if(points.size() < 2) return ofVec3f();
    updateCache();
    return tangents[getWrappedIndex(index)];
}

//--------------------------------------------------
ofVec3f ofPolyline::getTangentAtIndexInterpolated(float findex) const {
    if(points.size() < 2) return ofVec3f();
    int i1, i2;
    float t;
    getInterpolationParams(findex, i1, i2, t);
    return getTangentAtIndex(i1).getInterpolated(getTangentAtIndex(i2), t);
}

//--------------------------------------------------
ofVec3f ofPolyline::getNormalAtIndex(int index) const {
    if(points.size() < 2) return ofVec3f();
    updateCache();
    return normals[getWrappedIndex(index)];
}

//--------------------------------------------------
ofVec3f ofPolyline::getNormalAtIndexInterpolated(float findex) const {
    if(points.size() < 2) return ofVec3f();
    int i1, i2;
    float t;
    getInterpolationParams(findex, i1, i2, t);
    return getNormalAtIndex(i1).getInterpolated(getNormalAtIndex(i2), t);
}


//--------------------------------------------------
void ofPolyline::calcData(int index, ofVec3f &tangent, float &angle, ofVec3f &rotation, ofVec3f &normal) const {
    int i1 = getWrappedIndex(index - 1);
    int i2 = getWrappedIndex(index);
    int i3 = getWrappedIndex(index + 1);
    
    ofPoint p1(points[i1]);
    ofPoint p2(points[i2]);
    ofPoint p3(points[i3]);
    
    ofVec3f v1(p1 - p2); // vector to previous point
    ofVec3f v2(p3 - p2); // vector to next point
    v1.normalize();
    v2.normalize();
    
    tangent = (v2 - v1);
    tangent.normalize();
    
    rotation = v1.getCrossed(v2);
    angle = 180 - ofRadToDeg(acos(ofClamp(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z, -1, 1)));

    normal = rightVector.getCrossed(tangent);
    normal.normalize();
}


//--------------------------------------------------
int ofPolyline::getWrappedIndex(int index) const {
    if(points.empty()) return 0;
    
    if(index < 0) return isClosed() ? (index + points.size()) % points.size() : 0;
    if(index > int(points.size())-1) return isClosed() ? index % points.size() : points.size() - 1;
    return index;
}

//--------------------------------------------------
void ofPolyline::getInterpolationParams(float findex, int &i1, int &i2, float &t) const {
    i1 = floor(findex);
    t = findex - i1;
    i1 = getWrappedIndex(i1);
    i2 = getWrappedIndex(i1 + 1);
}

//--------------------------------------------------
void ofPolyline::updateCache(bool bForceUpdate) const {
    if(bCacheIsDirty || bForceUpdate) {
        lengths.clear();
        angles.clear();
        rotations.clear();
        normals.clear();
        tangents.clear();
        area = 0;
        centroid2D.set(0, 0, 0);
        bCacheIsDirty = false;
        
        if(points.size() < 2) return;

        // area
        for(int i=0;i<(int)points.size()-1;i++){
            area += points[i].x * points[i+1].y - points[i+1].x * points[i].y;
        }
        area += points[points.size()-1].x * points[0].y - points[0].x * points[points.size()-1].y;
        area *= 0.5;

        
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

        
        // per vertex cache
        lengths.resize(points.size());
        tangents.resize(points.size());
        angles.resize(points.size());
        normals.resize(points.size());
        rotations.resize(points.size());
        
        float angle;
        ofVec3f rotation;
        ofVec3f normal;
        ofVec3f tangent;

        float length = 0;
        for(int i=0; i<(int)points.size(); i++) {
            lengths[i] = length;

            calcData(i, tangent, angle, rotation, normal);
            tangents[i] = tangent;
            angles[i] = angle;
            rotations[i] = rotation;
            normals[i] = normal;
            
            length += points[i].distance(points[getWrappedIndex(i + 1)]);
        }
        
        if(isClosed()) lengths.push_back(length);
    }
}


//--------------------------------------------------
vector<ofPoint>::iterator ofPolyline::begin(){
	return points.begin();
}

//--------------------------------------------------
vector<ofPoint>::iterator ofPolyline::end(){
	return points.end();
}

//--------------------------------------------------
vector<ofPoint>::const_iterator ofPolyline::begin() const{
	return points.begin();
}

//--------------------------------------------------
vector<ofPoint>::const_iterator ofPolyline::end() const{
	return points.end();
}

//--------------------------------------------------
vector<ofPoint>::reverse_iterator ofPolyline::rbegin(){
	return points.rbegin();
}

//--------------------------------------------------
vector<ofPoint>::reverse_iterator ofPolyline::rend(){
	return points.rend();
}

//--------------------------------------------------
vector<ofPoint>::const_reverse_iterator ofPolyline::rbegin() const{
	return points.rbegin();
}

//--------------------------------------------------
vector<ofPoint>::const_reverse_iterator ofPolyline::rend() const{
	return points.rend();
}

