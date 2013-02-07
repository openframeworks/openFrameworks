#ifndef _ETF_H_
#define _ETF_H_

#include "imatrix.h"

struct Vect {
	double tx, ty, mag;
};


class ETF {
private:
	int Nr, Nc;
	Vect** p; 
	double max_grad;
public:
	ETF() 
    {
		Nr = 1, Nc = 1;
		p = new Vect*[Nr];
		for(int i = 0; i < Nr; i++)
		   p[i] = new Vect[Nc];
        p[0][0].tx=1.0; p[0][0].ty=0.0; p[0][0].mag=1.0;
        max_grad = 1.0;
    };
	ETF(int i, int j) 
    {
		Nr = i, Nc = j;
		p = new Vect*[Nr];
		for(i = 0; i < Nr; i++)
		   p[i] = new Vect[Nc];
		max_grad = 1.0;
    };
	void delete_all() {
		for (int i = 0; i < Nr; i++) 
			delete[] p[i];
		delete[] p;
	}
	~ETF() { delete_all(); }
	Vect* operator[](int i) { return p[i]; };
	Vect& get( int i, int j ) const { return p[i][j]; }
	int getRow() const { return Nr; }
	int getCol() const { return Nc; }
	void init(int i, int j) 
    {
		delete_all(); 
		Nr = i, Nc = j;
		p = new Vect*[Nr];
		for(i = 0; i < Nr; i++)
		   p[i] = new Vect[Nc];
		max_grad = 1.0;
    };
	void copy(ETF& s) 
    {
		for (int i = 0; i < Nr; i++) 
			for (int j = 0; j < Nc; j++) {
				p[i][j].tx = s.p[i][j].tx;
				p[i][j].ty = s.p[i][j].ty;
				p[i][j].mag = s.p[i][j].mag;
			}
		max_grad = s.max_grad;
    };
	void zero()
	{
		for (int i = 0; i < Nr; i++) 
			for (int j = 0; j < Nc; j++) 
				p[i][j].tx = p[i][j].ty = p[i][j].mag = 0.0;
	}
	void set(imatrix& image); 
	void set2(imatrix& image); 
	void Smooth(int half_w, int M);
	double GetMaxGrad() { return max_grad; }
	void normalize(); 
};


#endif