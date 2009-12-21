/**
* Credits:
* Code adopted from Lode Vandevenne http://student.kuleuven.be/~m0216922/CG/
*/

#ifndef _OFX_MATRIX_3X3
#define _OFX_MATRIX_3X3




class ofxMatrix3x3 {


  public:

    /**
    * [ a b c ]
    * [ d e f ]
    * [ g h i ]
    */
    double a;
    double b;
    double c;
    double d;
    double e;
    double f;
    double g;
    double h;
    double i;



    ofxMatrix3x3( double _a=0.0, double _b=0.0, double _c=0.0,
                  double _d=0.0, double _e=0.0, double _f=0.0,
                  double _g=0.0, double _h=0.0, double _i=0.0 );




    void set( double _a, double _b, double _c,
              double _d, double _e, double _f,
              double _g, double _h, double _i );


    double& operator[]( const int& index );


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

	ofxMatrix3x3 transpose(const ofxMatrix3x3& A);



	/**
    * Determinant: http://mathworld.wolfram.com/Determinant.html
    */

    double determinant() const;

    double determinant(const ofxMatrix3x3& A);



    /**
    * Inverse of a 3x3 matrix
      the inverse is the adjoint divided through the determinant
      find the matrix of minors (minor = determinant of 2x2 matrix of the 2 rows/colums current element is NOT in)
      turn them in cofactors (= change some of the signs)
      find the adjoint by transposing the matrix of cofactors
      divide this through the determinant to get the inverse
    */

    void invert();

    ofxMatrix3x3 inverse(const ofxMatrix3x3& A);



    /**
    * Add two matrices
    */
    ofxMatrix3x3 operator+(const ofxMatrix3x3& B);

    void operator+=(const ofxMatrix3x3& B);

    /**
    * Subtract two matrices
    */
    ofxMatrix3x3 operator-(const ofxMatrix3x3& B);

	void operator-=(const ofxMatrix3x3& B);


    /**
    * Multiply a matrix with a scalar
    */
    ofxMatrix3x3 operator*(double scalar);


	void operator*=(const ofxMatrix3x3& B);

    void operator*=(double scalar);

     /**
     * Multiply a 3x3 matrix with a 3x3 matrix
     */
    ofxMatrix3x3 operator*(const ofxMatrix3x3& B);

    /**
    * Divide a matrix through a scalar
    */
    ofxMatrix3x3 operator/(double scalar);


	void operator/=(const ofxMatrix3x3& B);

    void operator/=(double scalar);

};


#endif

