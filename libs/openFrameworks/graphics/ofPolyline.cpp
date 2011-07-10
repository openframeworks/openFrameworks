#include "ofPolyline.h"
#include "ofGraphics.h"

//----------------------------------------------------------
ofPolyline::ofPolyline(){
	clear();
}

//----------------------------------------------------------
ofPolyline::ofPolyline(const vector<ofPoint>& verts){
	clear();
	addVertexes(verts);
}

//----------------------------------------------------------
void ofPolyline::clear() {
	bClosed=false;
	points.clear();
	bHasChanged = true;
	curveVertices.clear();
}

//----------------------------------------------------------
void ofPolyline::addVertex(const ofPoint& p) {
	curveVertices.clear();
	points.push_back(p); bHasChanged=true;
}

//----------------------------------------------------------
void ofPolyline::addVertex(float x, float y, float z) {
	curveVertices.clear();
	addVertex(ofPoint(x,y,z)); bHasChanged=true;
}

//----------------------------------------------------------
void ofPolyline::addVertexes(const vector<ofPoint>& verts) {
	curveVertices.clear();
	points.insert( points.end(), verts.begin(), verts.end() );
	bHasChanged=true;
}

//----------------------------------------------------------
void ofPolyline::addVertexes(const ofPoint* verts, int numverts) {
	curveVertices.clear();
	points.insert( points.end(), verts, verts + numverts );
	bHasChanged=true;
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
	bHasChanged=true;
	return points[index];
}

//----------------------------------------------------------
void ofPolyline::resize(size_t size){
	bHasChanged=true;
	points.resize(size);
}

//----------------------------------------------------------
void ofPolyline::setClosed( bool tf ) {
	bHasChanged=true;
	bClosed = tf;
}

//----------------------------------------------------------
bool ofPolyline::isClosed() const {
	return bClosed;
}

//----------------------------------------------------------
void ofPolyline::close(){
	bClosed = true;
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
vector<ofPoint> & ofPolyline::getVertices(){
	return points;
}

//----------------------------------------------------------
void ofPolyline::setCircleResolution(int res){
	if (res > 1 && res != (int)circlePoints.size()){
		circlePoints.resize(res);

		float angle = 0.0f;
		float angleAdder = M_TWO_PI / (float)res;
		for (int i = 0; i < res; i++){
			circlePoints[i].x = cos(angle);
			circlePoints[i].y = sin(angle);
			circlePoints[i].z = 0.0f;
			angle += angleAdder;
		}
	}
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
}

//----------------------------------------------------------
void ofPolyline::arc( const ofPoint & center, float radiusX, float radiusY, float angleBegin, float angleEnd, int curveResolution){
	if(curveResolution==1) curveResolution=2;
	curveVertices.clear();
	setCircleResolution(curveResolution);
	points.reserve(points.size()+curveResolution);

	float size = (angleEnd - angleBegin)/360.0f;
	float begin = angleBegin/360.0f;

	if(size<1){
		const int segments = curveResolution*size;
		float angle,sinus,cosinus;
		float segment_size = PI*2.0*size/(float)segments;
		angle=-(PI*2.0*begin);
		for( int i=0; i<segments; i++){
			points.push_back(ofPoint(radiusX*circlePoints[i].x+center.x,radiusY*circlePoints[i].y+center.y));
			angle-=segment_size ;
		}
		angle=-(PI*2.0*begin);
		sinus = sin(angle);
		cosinus = cos(angle);
		points.push_back(ofPoint(radiusX*sinus+center.x,radiusY*cosinus+center.y));
	}else{
		for(int i=0;i<(int)circlePoints.size();i++){
			points.push_back(ofPoint(radiusX*circlePoints[i].x+center.x,radiusY*circlePoints[i].y+center.y,center.z));
		}
		points.push_back(ofPoint(radiusX*circlePoints[0].x+center.x,radiusY*circlePoints[0].y+center.y,center.z));
	}
}


//----------------------------------------------------------
float ofPolyline::getPerimeter() const {
	float perimeter = 0;
	int lastPosition = points.size() - 1;
	for(int i = 0; i < lastPosition; i++) {
		perimeter += points[i].distance(points[i + 1]);
	}
	if(bClosed && points.size() > 1) {
		perimeter += points[points.size() - 1].distance(points[0]);
	}
	return perimeter;
}

//----------------------------------------------------------
ofRectangle ofPolyline::getBoundingBox(){
	ofPolyline & polyline = *this;

	ofRectangle box;
	int n = polyline.size();
	if(n > 0) {
		const ofPoint& first = polyline[0];
		// inititally, use width and height as max x and max y
		box.set(first.x, first.y, first.x, first.y);
		for(int i = 0; i < n; i++) {
			const ofPoint& cur = polyline[i];
			if(cur.x < box.x) {
				box.x = cur.x;
			}
			if(cur.x > box.width) {
				box.width = cur.x;
			}
			if(cur.y < box.y) {
				box.y = cur.y;
			}
			if(cur.y > box.height) {
				box.height = cur.y;
			}
		}
		// later, we make width and height relative
		box.width -= box.x;
		box.height -= box.y;
	}
	return box;
}

//----------------------------------------------------------
ofPolyline ofPolyline::getSmoothed(int smoothingSize, float smoothingShape) {
	ofPolyline & polyline = *this;
	ofPolyline result = polyline;
	
	if(!polyline.isClosed()) {
		ofLog( OF_LOG_ERROR, "ofSmooth() currently only supports closed ofPolylines." );
		return polyline;
	}
	
	// precompute weights and normalization
	vector<float> weights;
	weights.resize(smoothingSize+1);
	float weightSum = 0;
	weights[0]=1; // center weight
	// side weights
	for(int i = 1; i <= smoothingSize; i++) {
		float curWeight = ofMap(i, 0, smoothingSize, 1, smoothingShape);
		weights[i]=curWeight;
		weightSum += curWeight;
	}
	float weightNormalization = 1 / (1 + 2 * weightSum);
	
	// use weights to make weighted averages of neighbors
	int n = polyline.size();
	for(int i = 0; i < n; i++) {
		for(int j = 1; j <= smoothingSize; j++) {
			int leftPosition = (n + i - j) % n;
			int rightPosition = (i + j) % n;
			const ofPoint& left = polyline[leftPosition];
			const ofPoint& right = polyline[rightPosition];
			result[i] += (left + right) * weights[j];
		}
		result[i] *= weightNormalization;
	}
	
	return result;
}

//----------------------------------------------------------
ofPolyline ofPolyline::getResampledBySpacing(float spacing) {
	ofPolyline & polyline = *this;
	ofPolyline result;
	// if more properties are added to ofPolyline, we need to copy them here
	result.setClosed(polyline.isClosed());

	float totalLength = 0;
	int curStep = 0;
	int lastPosition = polyline.size() - 1;
	if(polyline.isClosed()) {
		lastPosition++;
	}
	for(int i = 0; i < lastPosition; i++) {
		bool repeatNext = i == (int) (polyline.size() - 1);
	
		const ofPoint& cur = polyline[i];
		const ofPoint& next = repeatNext ? polyline[0] : polyline[i + 1];
		ofPoint diff = next - cur;
		
		float curSegmentLength = diff.length();
		totalLength += curSegmentLength;
		
		while(curStep * spacing <= totalLength) {
			float curSample = curStep * spacing;
			float curLength = curSample - (totalLength - curSegmentLength);
			float relativeSample = curLength / curSegmentLength;
			result.addVertex(cur.getInterpolated(next, relativeSample));
			curStep++;
		}
	}
	
	return result;
}

//----------------------------------------------------------
ofPolyline ofPolyline::getResampledByCount(int count) {
	float perimeter = getPerimeter();
	return ofPolyline::getResampledBySpacing(perimeter / count);
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
ofPoint ofPolyline::getClosestPoint(const ofPoint& target, unsigned int* nearestIndex) {
	ofPolyline & polyline = *this;

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

void ofPolyline::simplify(float tol){

	int n = size();

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

void ofPolyline::draw(){
	ofGetCurrentRenderer()->draw(*this);
}


