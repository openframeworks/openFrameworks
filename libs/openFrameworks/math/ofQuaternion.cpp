#include "ofQuaternion.h"
#include "ofMatrix4x4.h"
#include "ofMath.h"
#include "ofMathConstants.h"
#include "glm/gtc/quaternion.hpp"

//----------------------------------------
ofQuaternion::ofQuaternion(const glm::quat & q):_v(q.x, q.y, q.z, q.w){}

//----------------------------------------
ofQuaternion::operator glm::quat() const{
	return glm::quat(_v.w, glm::vec3(_v.x, _v.y, _v.z));
}

void ofQuaternion::set(const ofMatrix4x4& matrix) {
	*this = matrix.getRotate();
}

void ofQuaternion::get(ofMatrix4x4& matrix) const {
	matrix.makeRotationMatrix(*this);
}


/// Set the elements of the Quat to represent a rotation of angle
/// (degrees) around the axis (x,y,z)
void ofQuaternion::makeRotate( float angle, float x, float y, float z ) {
	angle = ofDegToRad(angle);
	
	const float epsilon = 0.0000001f;

	float length = sqrtf( x * x + y * y + z * z );
	if (length < epsilon) {
		// ~zero length axis, so reset rotation to zero.
		*this = ofQuaternion();
		return;
	}

	float inversenorm  = 1.0f / length;
	float coshalfangle = cosf( 0.5f * angle );
	float sinhalfangle = sinf( 0.5f * angle );

	_v.x = x * sinhalfangle * inversenorm;
	_v.y = y * sinhalfangle * inversenorm;
	_v.z = z * sinhalfangle * inversenorm;
	_v.w = coshalfangle;
}


void ofQuaternion::makeRotate( float angle, const ofVec3f& vec ) {
	makeRotate( angle, vec.x, vec.y, vec.z );
}


void ofQuaternion::makeRotate ( float angle1, const ofVec3f& axis1,
                          float angle2, const ofVec3f& axis2,
                          float angle3, const ofVec3f& axis3) {
       ofQuaternion q1; q1.makeRotate(angle1,axis1);
       ofQuaternion q2; q2.makeRotate(angle2,axis2);
       ofQuaternion q3; q3.makeRotate(angle3,axis3);

       *this = q1*q2*q3;
}

/** Make a rotation Quat which will rotate vec1 to vec2

 This routine uses only fast geometric transforms, without costly acos/sin computations.
 It's exact, fast, and with less degenerate cases than the acos/sin method.

 For an explanation of the math used, you may see for example:
 http://logiciels.cnes.fr/MARMOTTES/marmottes-mathematique.pdf

 @note This is the rotation with shortest angle, which is the one equivalent to the
 acos/sin transform method. Other rotations exists, for example to additionally keep
 a local horizontal attitude.

 @author Nicolas Brodu
 */
void ofQuaternion::makeRotate( const ofVec3f& from, const ofVec3f& to ) {

	// This routine takes any vector as argument but normalized
	// vectors are necessary, if only for computing the dot product.
	// Too bad the API is that generic, it leads to performance loss.
	// Even in the case the 2 vectors are not normalized but same length,
	// the sqrt could be shared, but we have no way to know beforehand
	// at this point, while the caller may know.
	// So, we have to test... in the hope of saving at least a sqrt
	ofVec3f sourceVector = from;
	ofVec3f targetVector = to;

	float fromLen2 = from.lengthSquared();
	float fromLen;
	// normalize only when necessary, epsilon test
	if ((fromLen2 < 1.0 - 1e-7) || (fromLen2 > 1.0 + 1e-7)) {
		fromLen = sqrt(fromLen2);
		sourceVector /= fromLen;
	} else fromLen = 1.0;

	float toLen2 = to.lengthSquared();
	// normalize only when necessary, epsilon test
	if ((toLen2 < 1.0 - 1e-7) || (toLen2 > 1.0 + 1e-7)) {
		float toLen;
		// re-use fromLen for case of mapping 2 vectors of the same length
		if ((toLen2 > fromLen2 - 1e-7) && (toLen2 < fromLen2 + 1e-7)) {
			toLen = fromLen;
		} else toLen = sqrt(toLen2);
		targetVector /= toLen;
	}


	// Now let's get into the real stuff
	// Use "dot product plus one" as test as it can be re-used later on
	double dotProdPlus1 = 1.0 + sourceVector.dot(targetVector);

	// Check for degenerate case of full u-turn. Use epsilon for detection
	if (dotProdPlus1 < 1e-7) {

		// Get an orthogonal vector of the given vector
		// in a plane with maximum vector coordinates.
		// Then use it as quaternion axis with pi angle
		// Trick is to realize one value at least is >0.6 for a normalized vector.
		if (fabs(sourceVector.x) < 0.6) {
			const double norm = sqrt(1.0 - sourceVector.x * sourceVector.x);
			_v.x = 0.0;
			_v.y = sourceVector.z / norm;
			_v.z = -sourceVector.y / norm;
			_v.w = 0.0;
		} else if (fabs(sourceVector.y) < 0.6) {
			const double norm = sqrt(1.0 - sourceVector.y * sourceVector.y);
			_v.x = -sourceVector.z / norm;
			_v.y = 0.0;
			_v.z = sourceVector.x / norm;
			_v.w = 0.0;
		} else {
			const double norm = sqrt(1.0 - sourceVector.z * sourceVector.z);
			_v.x = sourceVector.y / norm;
			_v.y = -sourceVector.x / norm;
			_v.z = 0.0;
			_v.w = 0.0;
		}
	}

	else {
		// Find the shortest angle quaternion that transforms normalized vectors
		// into one other. Formula is still valid when vectors are colinear
		const double s = sqrt(0.5 * dotProdPlus1);
		const ofVec3f tmp = sourceVector.getCrossed(targetVector) / (2.0 * s);
		_v.x = tmp.x;
		_v.y = tmp.y;
		_v.z = tmp.z;
		_v.w = s;
	}
}


// Make a rotation Quat which will rotate vec1 to vec2
// Generally take adot product to get the angle between these
// and then use a cross product to get the rotation axis
// Watch out for the two special cases of when the vectors
// are co-incident or opposite in direction.
void ofQuaternion::makeRotate_original( const ofVec3f& from, const ofVec3f& to ) {
	const float epsilon = 0.0000001f;

	float length1  = from.length();
	float length2  = to.length();

	// dot product vec1*vec2
	float cosangle = from.dot(to) / (length1 * length2);

	if ( fabs(cosangle - 1) < epsilon ) {
		//osg::notify(osg::INFO)<<"*** Quat::makeRotate(from,to) with near co-linear vectors, epsilon= "<<fabs(cosangle-1)<<std::endl;

		// cosangle is close to 1, so the vectors are close to being coincident
		// Need to generate an angle of zero with any vector we like
		// We'll choose (1,0,0)
		makeRotate( 0.0, 0.0, 0.0, 1.0 );
	} else
		if ( fabs(cosangle + 1.0) < epsilon ) {
			// vectors are close to being opposite, so will need to find a
			// vector orthongonal to from to rotate about.
			ofVec3f tmp;
			if (fabs(from.x) < fabs(from.y))
				if (fabs(from.x) < fabs(from.z)) tmp.set(1.0, 0.0, 0.0); // use x axis.
				else tmp.set(0.0, 0.0, 1.0);
			else if (fabs(from.y) < fabs(from.z)) tmp.set(0.0, 1.0, 0.0);
			else tmp.set(0.0, 0.0, 1.0);

			ofVec3f fromd(from.x, from.y, from.z);

			// find orthogonal axis.
			ofVec3f axis(fromd.getCrossed(tmp));
			axis.normalize();

			_v.x = axis[0]; // sine of half angle of PI is 1.0.
			_v.y = axis[1]; // sine of half angle of PI is 1.0.
			_v.z = axis[2]; // sine of half angle of PI is 1.0.
			_v.w = 0; // cosine of half angle of PI is zero.

		} else {
			// This is the usual situation - take a cross-product of vec1 and vec2
			// and that is the axis around which to rotate.
			ofVec3f axis(from.getCrossed(to));
			float angle = acos( cosangle );
			makeRotate( angle, axis );
		}
}

void ofQuaternion::getRotate( float& angle, ofVec3f& vec ) const {
	float x, y, z;
	getRotate(angle, x, y, z);
	vec.x = x;
	vec.y = y;
	vec.z = z;
}
// Get the angle of rotation and axis of this Quat object.
// Won't give very meaningful results if the Quat is not associated
// with a rotation!
void ofQuaternion::getRotate( float& angle, float& x, float& y, float& z ) const {
	float sinhalfangle = sqrt( _v.x * _v.x + _v.y * _v.y + _v.z * _v.z );

	angle = 2.0 * atan2( sinhalfangle, _v.w );
	if (sinhalfangle) {
		x = _v.x / sinhalfangle;
		y = _v.y / sinhalfangle;
		z = _v.z / sinhalfangle;
	} else {
		x = 0.0;
		y = 0.0;
		z = 1.0;
	}
	
	angle = ofRadToDeg(angle);
}


/// Spherical Linear Interpolation
/// As t goes from 0 to 1, the Quat object goes from "from" to "to"
/// Reference: Shoemake at SIGGRAPH 89
/// See also
/// http://www.gamasutra.com/features/programming/19980703/quaternions_01.htm
void ofQuaternion::slerp( float t, const ofQuaternion& from, const ofQuaternion& to ) {
	const double epsilon = 0.00001;
	double omega, cosomega, sinomega, scale_from, scale_to ;

	ofQuaternion quatTo(to);
	// this is a dot product

	cosomega = from.asVec4().dot(to.asVec4());

	if ( cosomega < 0.0 ) {
		cosomega = -cosomega;
		quatTo = -to;
	}

	if ( (1.0 - cosomega) > epsilon ) {
		omega = acos(cosomega) ; // 0 <= omega <= Pi (see man acos)
		sinomega = sin(omega) ;  // this sinomega should always be +ve so
		// could try sinomega=sqrt(1-cosomega*cosomega) to avoid a sin()?
		scale_from = sin((1.0 - t) * omega) / sinomega ;
		scale_to = sin(t * omega) / sinomega ;
	} else {
		/* --------------------------------------------------
		The ends of the vectors are very close
		we can use simple linear interpolation - no need
		to worry about the "spherical" interpolation
		-------------------------------------------------- */
		scale_from = 1.0 - t ;
		scale_to = t ;
	}

	*this = (from * scale_from) + (quatTo * scale_to);

	// so that we get a Vec4
}


// ref at http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/index.htm
ofVec3f ofQuaternion::getEuler() const {
	float test = x()*y() + z()*w();
	float heading;
	float attitude;
	float bank;
	if (test > 0.499) { // singularity at north pole
		heading = 2 * atan2(x(), w());
		attitude = PI/2;
		bank = 0;
	} else if (test < -0.499) { // singularity at south pole
		heading = -2 * atan2(x(), w());
		attitude = - PI/2;
		bank = 0;
	} else {
		float sqx = x() * x();
		float sqy = y() * y();
		float sqz = z() * z();
		heading = atan2(2.0f * y() * w() - 2.0f * x() * z(), 1.0f - 2.0f*sqy - 2.0f*sqz);
		attitude = asin(2*test);
		bank = atan2(2.0f*x() * w() - 2.0f * y() * z(), 1.0f - 2.0f*sqx - 2.0f*sqz);
	}
	
	return ofVec3f(ofRadToDeg(bank), ofRadToDeg(heading), ofRadToDeg(attitude));
}

#define QX  _v.x
#define QY  _v.y
#define QZ  _v.z
#define QW  _v.w

//----------------------------------------
std::ostream& operator<<(std::ostream& os, const ofQuaternion &q) {
    os << q._v.x << ", " << q._v.y << ", " << q._v.z << ", " << q._v.w;
    return os;
}


//----------------------------------------
std::istream& operator>>(std::istream& is, ofQuaternion &q) {
    is >> q._v.x;
    is.ignore(2);
    is >> q._v.y;
    is.ignore(2);
    is >> q._v.z;
    is.ignore(2);
    is >> q._v.w;
    return is;
}
