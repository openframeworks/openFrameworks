#include "ofMatrix3x3.h"
#include <iomanip>

ofMatrix3x3::ofMatrix3x3( float _a, float _b, float _c,
			  float _d, float _e, float _f,
			  float _g, float _h, float _i )
{
	a = _a;
	b = _b;
	c = _c;
	d = _d;
	e = _e;
	f = _f;
	g = _g;
	h = _h;
	i = _i;
}




void ofMatrix3x3::set( float _a, float _b, float _c,
		  float _d, float _e, float _f,
		  float _g, float _h, float _i )
{
	a = _a;
	b = _b;
	c = _c;
	d = _d;
	e = _e;
	f = _f;
	g = _g;
	h = _h;
	i = _i;
}


float& ofMatrix3x3::operator[]( const int& index ) {
	switch(index) {
		case 0:  return a;
		case 1:  return b;
		case 2:  return c;
		case 3:  return d;
		case 4:  return e;
		case 5:  return f;
		case 6:  return g;
		case 7:  return h;
		case 8:  return i;
		default: return a;
	}
}


/*
 * Transpose:
 * This changes the matrix.
 * [ a b c ]T    [ a d g ]
 * [ d e f ]  =  [ b e h ]
 * [ g h i ]     [ c f i ]
 */

void ofMatrix3x3::transpose() {
	b += d; d = b - d; b -= d; //swap b and d
	c += g; g = c - g; c -= g; //swap c and g
	f += h; h = f - h; f -= h; //swap f and h
}

/*
* Transpose without changing the matrix.
* Uses the "swap" method with additions and subtractions to swap the elements that aren't on the main diagonal.
* @return transposed matrix.
*/

ofMatrix3x3 ofMatrix3x3::transpose(const ofMatrix3x3& A) {
	ofMatrix3x3 result = A;
	result.transpose();
	return result;
}



/*
* Determinant: http://mathworld.wolfram.com/Determinant.html
*/

float ofMatrix3x3::determinant() const {
	float det = a * e * i
			   + b * f * g
			   + d * h * c
			   - g * e * c
			   - d * b * i
			   - h * f * a;
	return det;
}

float ofMatrix3x3::determinant(const ofMatrix3x3& A) {
	return A.determinant();
}



/*
* Inverse of a 3x3 matrix
  the inverse is the adjoint divided through the determinant
  find the matrix of minors (minor = determinant of 2x2 matrix of the 2 rows/colums current element is NOT in)
  turn them in cofactors (= change some of the signs)
  find the adjoint by transposing the matrix of cofactors
  divide this through the determinant to get the inverse
*/

void ofMatrix3x3::invert() {
	 float det = determinant();
	 ofMatrix3x3 B;

	 //included in these calculations: minor, cofactor (changed signs), transpose (by the order of "="), division through determinant
	 B.a = ( e * i - h * f) / det;
	 B.b = (-b * i + h * c) / det;
	 B.c = ( b * f - e * c) / det;
	 B.d = (-d * i + g * f) / det;
	 B.e = ( a * i - g * c) / det;
	 B.f = (-a * f + d * c) / det;
	 B.g = ( d * h - g * e) / det;
	 B.h = (-a * h + g * b) / det;
	 B.i = ( a * e - d * b) / det;

	 *this = B;
}

ofMatrix3x3 ofMatrix3x3::inverse(const ofMatrix3x3& A) {
	ofMatrix3x3 result = A;
	result.invert();
	return result;
}



/*
* Add two matrices
*/
ofMatrix3x3 ofMatrix3x3::operator+(const ofMatrix3x3& B) {
	ofMatrix3x3 result;
	result.a = a + B.a;
	result.b = b + B.b;
	result.c = c + B.c;
	result.d = d + B.d;
	result.e = e + B.e;
	result.f = f + B.f;
	result.g = g + B.g;
	result.h = h + B.h;
	result.i = i + B.i;
	return result;
}

void ofMatrix3x3::operator+=(const ofMatrix3x3& B) {
	a += B.a;
	b += B.b;
	c += B.c;
	d += B.d;
	e += B.e;
	f += B.f;
	g += B.g;
	h += B.h;
	i += B.i;
}

/*
* Subtract two matrices
*/
ofMatrix3x3 ofMatrix3x3::operator-(const ofMatrix3x3& B) {
	ofMatrix3x3 result;
	result.a = a - B.a;
	result.b = b - B.b;
	result.c = c - B.c;
	result.d = d - B.d;
	result.e = e - B.e;
	result.f = f - B.f;
	result.g = g - B.g;
	result.h = h - B.h;
	result.i = i - B.i;
	return result;
}

void ofMatrix3x3::operator-=(const ofMatrix3x3& B) {
	a -= B.a;
	b -= B.b;
	c -= B.c;
	d -= B.d;
	e -= B.e;
	f -= B.f;
	g -= B.g;
	h -= B.h;
	i -= B.i;
}


/*
* Multiply a matrix with a scalar
*/
ofMatrix3x3 ofMatrix3x3::operator*(float scalar) {
	ofMatrix3x3 result;
	result.a = a * scalar;
	result.b = b * scalar;
	result.c = c * scalar;
	result.d = d * scalar;
	result.e = e * scalar;
	result.f = f * scalar;
	result.g = g * scalar;
	result.h = h * scalar;
	result.i = i * scalar;
	return result;
}


void ofMatrix3x3::operator*=(const ofMatrix3x3& B) {
  *this = *this*B;
}

ofMatrix3x3 ofMatrix3x3::entrywiseTimes(const ofMatrix3x3& B){
  ofMatrix3x3 C = *this;
	C.a *= B.a;
	C.b *= B.b;
	C.c *= B.c;
	C.d *= B.d;
	C.e *= B.e;
	C.f *= B.f;
	C.g *= B.g;
	C.h *= B.h;
	C.i *= B.i;
  return C;
}

void ofMatrix3x3::operator*=(float scalar) {
	a *= scalar;
	b *= scalar;
	c *= scalar;
	d *= scalar;
	e *= scalar;
	f *= scalar;
	g *= scalar;
	h *= scalar;
	i *= scalar;
}

 /*
 * Multiply a 3x3 matrix with a 3x3 matrix
 */
ofMatrix3x3 ofMatrix3x3::operator*(const ofMatrix3x3& B) {
	ofMatrix3x3 C;
	C.a = a * B.a + b * B.d + c * B.g;
	C.b = a * B.b + b * B.e + c * B.h;
	C.c = a * B.c + b * B.f + c * B.i;
	C.d = d * B.a + e * B.d + f * B.g;
	C.e = d * B.b + e * B.e + f * B.h;
	C.f = d * B.c + e * B.f + f * B.i;
	C.g = g * B.a + h * B.d + i * B.g;
	C.h = g * B.b + h * B.e + i * B.h;
	C.i = g * B.c + h * B.f + i * B.i;
	return C;
}

/*
* Divide a matrix through a scalar
*/
ofMatrix3x3 ofMatrix3x3::operator/(float scalar) {
	ofMatrix3x3 result;
	result.a = a / scalar;
	result.b = b / scalar;
	result.c = c / scalar;
	result.d = d / scalar;
	result.e = e / scalar;
	result.f = f / scalar;
	result.g = g / scalar;
	result.h = h / scalar;
	result.i = i / scalar;
	return result;
}


void ofMatrix3x3::operator/=(const ofMatrix3x3& B) {
	a /= B.a;
	b /= B.b;
	c /= B.c;
	d /= B.d;
	e /= B.e;
	f /= B.f;
	g /= B.g;
	h /= B.h;
	i /= B.i;
}

void ofMatrix3x3::operator/=(float scalar) {
	a /= scalar;
	b /= scalar;
	c /= scalar;
	d /= scalar;
	e /= scalar;
	f /= scalar;
	g /= scalar;
	h /= scalar;
	i /= scalar;
}


std::ostream& operator<<(std::ostream& os, const ofMatrix3x3& M) {
	int w = 8;
	os	<< std::setw(w)
		<< M.a << ", " << std::setw(w)
		<< M.b << ", " << std::setw(w)
		<< M.c << std::endl;

	os	<< std::setw(w)
		<< M.d << ", " << std::setw(w)
		<< M.e << ", " << std::setw(w)
		<< M.f << std::endl;

	os	<< std::setw(w)
		<< M.g << ", " << std::setw(w)
		<< M.h << ", " << std::setw(w)
		<< M.i;
	return os;
}

std::istream& operator>>(std::istream& is, ofMatrix3x3& M) {
	is >> M.a; is.ignore(2);
	is >> M.b; is.ignore(2);
	is >> M.c; is.ignore(1);

	is >> M.d; is.ignore(2);
	is >> M.e; is.ignore(2);
	is >> M.f; is.ignore(1);

	is >> M.g; is.ignore(2);
	is >> M.h; is.ignore(2);
	is >> M.i;
	return is;
}


