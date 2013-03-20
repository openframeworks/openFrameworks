#ifndef _IMATRIX_H_
#define _IMATRIX_H_

class imatrix {
private:
	int Nr, Nc;
	int** p; 
	void delete_all() {
		for (int i = 0; i < Nr; i++) 
			delete[] p[i];
		delete[] p;
	}
public:
	imatrix() 
	{
		Nr = 1, Nc = 1;
		p = new int*[Nr];
		for(int i = 0; i < Nr; i++)
			p[i] = new int[Nc];
		p[0][0]=1; 
	};
	imatrix(int i, int j) 
	{
		Nr = i, Nc = j;
		
		p = new int*[Nr];
		for(i = 0; i < Nr; i++)
			p[i] = new int[Nc];
	};
	imatrix(imatrix& b) {
		Nr = b.Nr;
		Nc = b.Nc;
		p = new int*[Nr];
		for (int i = 0; i < Nr; i++) {
			p[i] = new int[Nc];
			for (int j = 0; j < Nc; j++) {
				p[i][j] = b[i][j];
			}
		}
	}
	void init(int i, int j) 
	{
		delete_all();
		Nr = i, Nc = j;
		p = new int*[Nr];
		for(i = 0; i < Nr; i++)
			p[i] = new int[Nc];
	};
	
	~imatrix()
	{
		delete_all();
	}
	int* operator[](int i) { return p[i]; };
	
	int& get( int i, int j ) const { return p[i][j]; }
	int getRow() const { return Nr; }
	int getCol() const { return Nc; }
	
	void zero()
	{
		for (int i = 0; i < Nr; i++) 
			for (int j = 0; j < Nc; j++) 
				p[i][j] = 0;
	}
	void copy(imatrix& b)
	{
		init(b.Nr, b.Nc);
		for (int i = 0; i < Nr; i++) 
			for (int j = 0; j < Nc; j++) 
				p[i][j] = b.p[i][j];
	}
};


#endif