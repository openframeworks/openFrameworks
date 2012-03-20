/**
* Credits:
* Code adopted from Lode Vandevenne http://student.kuleuven.be/~m0216922/CG/
*/

#pragma once

#include "ofConstants.h"


class ofMatrix3x3 {


  public:

    /**
    * [ a b c ]
    * [ d e f ]
    * [ g h i ]
    */
    float a;
    float b;
    float c;
    float d;
    float e;
    float f;
    float g;
    float h;
    float i;



    ofMatrix3x3( float _a=0.0, float _b=0.0, float _c=0.0,
                  float _d=0.0, float _e=0.0, float _f=0.0,
                  float _g=0.0, float _h=0.0, float _i=0.0 );




    void set( float _a, float _b, float _c,
              float _d, float _e, float _f,
              float _g, float _h, float _i );


    float& operator[]( const int& index );


	/**
     * Transpose:
     * This changes the matrix.
     * [ a b c ]T    [ a d g ]
     * [ d e f ]  =  [ b e h ]
     * [ g h i ]     [ c f i ]
     */

	void transpose();

	/**
	* Transpose without changing the matrix.
    * Uses the "swap" method with additions and subtractions to swap the elements that aren't on the main diagonal.
    * @return transposed matrix.
    */

	ofMatrix3x3 transpose(const ofMatrix3x3& A);



	/**
    * Determinant: http://mathworld.wolfram.com/Determinant.html
    */

    float determinant() const;

    float determinant(const ofMatrix3x3& A);



    /**
    * Inverse of a 3x3 matrix
      the inverse is the adjoint divided through the determinant
      find the matrix of minors (minor = determinant of 2x2 matrix of the 2 rows/colums current element is NOT in)
      turn them in cofactors (= change some of the signs)
      find the adjoint by transposing the matrix of cofactors
      divide this through the determinant to get the inverse
    */

    void invert();

    ofMatrix3x3 inverse(const ofMatrix3x3& A);


    /**
    * Add two matrices
    */
    ofMatrix3x3 operator+(const ofMatrix3x3& B);

    void operator+=(const ofMatrix3x3& B);

    /**
    * Subtract two matrices
    */
    ofMatrix3x3 operator-(const ofMatrix3x3& B);

	void operator-=(const ofMatrix3x3& B);


    /**
    * Multiply a matrix with a scalar
    */
    ofMatrix3x3 operator*(float scalar);

  /**
   * Multiply a matrix by a matrix this = this*B (in that order)
   */
	void operator*=(const ofMatrix3x3& B);

  /**
   * Multiply a matrix by a matrix entry by entry (i.e. a*a, b*b, c*c...)
   This is referred to as an entrywise, Hadamard, or Schur product
   */
    ofMatrix3x3 entrywiseTimes(const ofMatrix3x3& A);

  /**
   * Multiply a matrix by a scalar (multiples all entries by scalar)
   */
    void operator*=(float scalar);

	friend ostream& operator<<(ostream& os, const ofMatrix3x3& M);
	friend istream& operator>>(istream& is, ofMatrix3x3& M);

	/**
     * Multiply a 3x3 matrix with a 3x3 matrix
     */
    ofMatrix3x3 operator*(const ofMatrix3x3& B);

    /**
    * Divide a matrix through a scalar
    */
    ofMatrix3x3 operator/(float scalar);


	void operator/=(const ofMatrix3x3& B);

    void operator/=(float scalar);

};

