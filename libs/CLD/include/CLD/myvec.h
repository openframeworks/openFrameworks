#ifndef _MYVEC_H_
#define _MYVEC_H_

#include <cmath>

class myvec {
private:
	
public:
	int	N; 
	double* p; 
    myvec() 
    {
		N = 1;
		p = new double[1];
        p[0]=1.0;
    };
	myvec(int i) 
    {
		N = i;
		p = new double[N];
    };
	~myvec()
	{
		delete[] p;
	}
	double& operator[](int i) { return p[i]; } 
	const double& operator[](int i) const { return p[i]; } 
	void zero() {
		for (int i = 0; i < N; i++)
			p[i] = 0.0;
	}
	void make_unit() { 
		double sum = 0.0;
		for (int i = 0; i < N; i++) {
			sum += p[i]*p[i];
		}
		sum = sqrt(sum);
		if (sum > 0.0) { 
			for (int i = 0; i < N; i++) {
				p[i] = p[i] / sum;
			}
		}
	}
	double norm() { 
		double sum = 0.0;
		for (int i = 0; i < N; i++) {
			sum += p[i]*p[i];
		}
		sum = sqrt(sum);
		return sum;
	}
	double get(int n) const { return p[n]; }
	int getMax() { return N; }
	void init(int i) {
		delete[] p;
		N = i;
		p = new double[N];
	}
};

class mymatrix {
private:
	int Nr, Nc;
	double** p; 
	void delete_all() {
		for (int i = 0; i < Nr; i++) 
				delete[] p[i];
			delete[] p;
	}
public:
	mymatrix() 
    {
		Nr = 1, Nc = 1;
		p = new double*[Nr];
		for(int i = 0; i < Nr; i++)
		   p[i] = new double[Nc];
        p[0][0]=1.0; 
    };
	mymatrix(int i, int j) 
    {
		Nr = i, Nc = j;
		p = new double*[Nr];
		for(i = 0; i < Nr; i++)
		   p[i] = new double[Nc];
    };
	mymatrix(mymatrix& b) {
		Nr = b.Nr;
		Nc = b.Nc;
		p = new double*[Nr];
		for (int i = 0; i < Nr; i++) {
			p[i] = new double[Nc];
			for (int j = 0; j < Nc; j++) {
				p[i][j] = b[i][j];
			}
		}
	}
	~mymatrix() {
		delete_all();
	}
	double* operator[](int i) { return p[i]; };
	double& get( int i, int j ) const { return p[i][j]; }
	int getRow() const { return Nr; }
	int getCol() const { return Nc; }
	void init(int i, int j) 
    {
		delete_all();
		Nr = i, Nc = j;
		p = new double*[Nr];
		for(i = 0; i < Nr; i++)
		   p[i] = new double[Nc];
    };
	void zero()
	{
		for (int i = 0; i < Nr; i++) 
			for (int j = 0; j < Nc; j++) 
				p[i][j] = 0.0;
	}
};


#endif