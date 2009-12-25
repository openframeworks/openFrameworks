
#include <stdio.h>
#include "ofxQuaternion.h"
#include "ofxMatrix4x4.h"

void ofxQuaternion::set(const ofxMatrix4x4& matrix) {
	*this = matrix.getRotate();
}

void ofxQuaternion::get(ofxMatrix4x4& matrix) const {
	matrix.makeRotationMatrix(*this);
}


/// Set the elements of the Quat to represent a rotation of angle
/// (radians) around the axis (x,y,z)
void ofxQuaternion::makeRotate( float angle, float x, float y, float z ) {
	const float epsilon = 0.0000001f;

	float length = sqrtf( x * x + y * y + z * z );
	if (length < epsilon) {
		// ~zero length axis, so reset rotation to zero.
		*this = ofxQuaternion();
		return;
	}

	float inversenorm  = 1.0f / length;
	float coshalfangle = cosf( 0.5f * angle );
	float sinhalfangle = sinf( 0.5f * angle );

	_v[0] = x * sinhalfangle * inversenorm;
	_v[1] = y * sinhalfangle * inversenorm;
	_v[2] = z * sinhalfangle * inversenorm;
	_v[3] = coshalfangle;
}


void ofxQuaternion::makeRotate( float angle, const ofxVec3f& vec ) {
	makeRotate( angle, vec.x, vec.y, vec.z );
}


void ofxQuaternion::makeRotate ( float angle1, const ofxVec3f& axis1,
                          float angle2, const ofxVec3f& axis2,
                          float angle3, const ofxVec3f& axis3) {
       ofxQuaternion q1; q1.makeRotate(angle1,axis1);
       ofxQuaternion q2; q2.makeRotate(angle2,axis2);
       ofxQuaternion q3; q3.makeRotate(angle3,axis3);

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
void ofxQuaternion::makeRotate( const ofxVec3f& from, const ofxVec3f& to ) {

	// This routine takes any vector as argument but normalized
	// vectors are necessary, if only for computing the dot product.
	// Too bad the API is that generic, it leads to performance loss.
	// Even in the case the 2 vectors are not normalized but same length,
	// the sqrt could be shared, but we have no way to know beforehand
	// at this point, while the caller may know.
	// So, we have to test... in the hope of saving at least a sqrt
	ofxVec3f sourceVector = from;
	ofxVec3f targetVector = to;

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
			_v[0] = 0.0;
			_v[1] = sourceVector.z / norm;
			_v[2] = -sourceVector.y / norm;
			_v[3] = 0.0;
		} else if (fabs(sourceVector.y) < 0.6) {
			const double norm = sqrt(1.0 - sourceVector.y * sourceVector.y);
			_v[0] = -sourceVector.z / norm;
			_v[1] = 0.0;
			_v[2] = sourceVector.x / norm;
			_v[3] = 0.0;
		} else {
			const double norm = sqrt(1.0 - sourceVector.z * sourceVector.z);
			_v[0] = sourceVector.y / norm;
			_v[1] = -sourceVector.x / norm;
			_v[2] = 0.0;
			_v[3] = 0.0;
		}
	}

	else {
		// Find the shortest angle quaternion that transforms normalized vectors
		// into one other. Formula is still valid when vectors are colinear
		const double s = sqrt(0.5 * dotProdPlus1);
		const ofxVec3f tmp = sourceVector.getCrossed(targetVector) / (2.0 * s);
		_v[0] = tmp.x;
		_v[1] = tmp.y;
		_v[2] = tmp.z;
		_v[3] = s;
	}
}


// Make a rotation Quat which will rotate vec1 to vec2
// Generally take adot product to get the angle between these
// and then use a cross product to get the rotation axis
// Watch out for the two special cases of when the vectors
// are co-incident or opposite in direction.
void ofxQuaternion::makeRotate_original( const ofxVec3f& from, const ofxVec3f& to ) {
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
			ofxVec3f tmp;
			if (fabs(from.x) < fabs(from.y))
				if (fabs(from.x) < fabs(from.z)) tmp.set(1.0, 0.0, 0.0); // use x axis.
				else tmp.set(0.0, 0.0, 1.0);
			else if (fabs(from.y) < fabs(from.z)) tmp.set(0.0, 1.0, 0.0);
			else tmp.set(0.0, 0.0, 1.0);

			ofxVec3f fromd(from.x, from.y, from.z);

			// find orthogonal axis.
			ofxVec3f axis(fromd.getCrossed(tmp));
			axis.normalize();

			_v[0] = axis[0]; // sin of half angle of PI is 1.0.
			_v[1] = axis[1]; // sin of half angle of PI is 1.0.
			_v[2] = axis[2]; // sin of half angle of PI is 1.0.
			_v[3] = 0; // cos of half angle of PI is zero.

		} else {
			// This is the usual situation - take a cross-product of vec1 and vec2
			// and that is the axis around which to rotate.
			ofxVec3f axis(from.getCrossed(to));
			float angle = acos( cosangle );
			makeRotate( angle, axis );
		}
}

void ofxQuaternion::getRotate( float& angle, ofxVec3f& vec ) const {
	float x, y, z;
	getRotate(angle, x, y, z);
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
}
// Get the angle of rotation and axis of this Quat object.
// Won't give very meaningful results if the Quat is not associated
// with a rotation!
void ofxQuaternion::getRotate( float& angle, float& x, float& y, float& z ) const {
	float sinhalfangle = sqrt( _v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2] );

	angle = 2.0 * atan2( sinhalfangle, _v[3] );
	if (sinhalfangle) {
		x = _v[0] / sinhalfangle;
		y = _v[1] / sinhalfangle;
		z = _v[2] / sinhalfangle;
	} else {
		x = 0.0;
		y = 0.0;
		z = 1.0;
	}

}


/// Spherical Linear Interpolation
/// As t goes from 0 to 1, the Quat object goes from "from" to "to"
/// Reference: Shoemake at SIGGRAPH 89
/// See also
/// http://www.gamasutra.com/features/programming/19980703/quaternions_01.htm
void ofxQuaternion::slerp( float t, const ofxQuaternion& from, const ofxQuaternion& to ) {
	const double epsilon = 0.00001;
	double omega, cosomega, sinomega, scale_from, scale_to ;

	ofxQuaternion quatTo(to);
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


#define QX  _v[0]
#define QY  _v[1]
#define QZ  _v[2]
#define QW  _v[3]

