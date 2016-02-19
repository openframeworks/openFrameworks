#include "ETF.h"
#include "imatrix.h"

#include "ofMain.h"

void ETF::set(imatrix& image) 
{
	int i, j;
	double MAX_VAL = 1020.; 
	double v[2];
	
	max_grad = -1.;
	
	for (i = 1; i < Nr - 1; i++) { 
		for (j = 1; j < Nc - 1; j++) {
			////////////////////////////////////////////////////////////////
			p[i][j].tx = (image[i+1][j-1] + 2*(double)image[i+1][j] + image[i+1][j+1] 
										- image[i-1][j-1] - 2*(double)image[i-1][j] - image[i-1][j+1]) / MAX_VAL;
			p[i][j].ty = (image[i-1][j+1] + 2*(double)image[i][j+1] + image[i+1][j+1]
										- image[i-1][j-1] - 2*(double)image[i][j-1] - image[i+1][j-1]) / MAX_VAL;
			/////////////////////////////////////////////
			v[0] = p[i][j].tx;
			v[1] = p[i][j].ty;
			p[i][j].tx = -v[1];
			p[i][j].ty = v[0];
			//////////////////////////////////////////////
			p[i][j].mag = sqrt(p[i][j].tx * p[i][j].tx + p[i][j].ty * p[i][j].ty);
			
			if (p[i][j].mag > max_grad) {
				max_grad = p[i][j].mag;
			}
		}
	}
	
	for (i = 1; i <= Nr - 2; i++) {
		p[i][0].tx = p[i][1].tx;
		p[i][0].ty = p[i][1].ty;
		p[i][0].mag = p[i][1].mag;
		p[i][Nc - 1].tx = p[i][Nc - 2].tx;
		p[i][Nc - 1].ty = p[i][Nc - 2].ty;
		p[i][Nc - 1].mag = p[i][Nc - 2].mag;
	}
	
	for (j = 1; j <= Nc - 2; j++) {
		p[0][j].tx = p[1][j].tx;
		p[0][j].ty = p[1][j].ty;
		p[0][j].mag = p[1][j].mag;
		p[Nr - 1][j].tx = p[Nr - 2][j].tx;
		p[Nr - 1][j].ty = p[Nr - 2][j].ty;
		p[Nr - 1][j].mag = p[Nr - 2][j].mag;
	}
	
	p[0][0].tx = ( p[0][1].tx + p[1][0].tx ) / 2;
	p[0][0].ty = ( p[0][1].ty + p[1][0].ty ) / 2;
	p[0][0].mag = ( p[0][1].mag + p[1][0].mag ) / 2;
	p[0][Nc-1].tx = ( p[0][Nc-2].tx + p[1][Nc-1].tx ) / 2;
	p[0][Nc-1].ty = ( p[0][Nc-2].ty + p[1][Nc-1].ty ) / 2;
	p[0][Nc-1].mag = ( p[0][Nc-2].mag + p[1][Nc-1].mag ) / 2;
	p[Nr-1][0].tx = ( p[Nr-1][1].tx + p[Nr-2][0].tx ) / 2;
	p[Nr-1][0].ty = ( p[Nr-1][1].ty + p[Nr-2][0].ty ) / 2;
	p[Nr-1][0].mag = ( p[Nr-1][1].mag + p[Nr-2][0].mag ) / 2;
	p[Nr - 1][Nc - 1].tx = ( p[Nr - 1][Nc - 2].tx + p[Nr - 2][Nc - 1].tx ) / 2;
	p[Nr - 1][Nc - 1].ty = ( p[Nr - 1][Nc - 2].ty + p[Nr - 2][Nc - 1].ty ) / 2;
	p[Nr - 1][Nc - 1].mag = ( p[Nr - 1][Nc - 2].mag + p[Nr - 2][Nc - 1].mag ) / 2;
	
	normalize();
	
}

void ETF::set2(imatrix& image) 
{
	int i, j;
	double MAX_VAL = 1020.; 
	double v[2];
	
	max_grad = -1.;
	
	imatrix tmp(Nr, Nc);
	
	for (i = 1; i < Nr - 1; i++) { 
		for (j = 1; j < Nc - 1; j++) {
			////////////////////////////////////////////////////////////////
			p[i][j].tx = (image[i+1][j-1] + 2*(double)image[i+1][j] + image[i+1][j+1] 
										- image[i-1][j-1] - 2*(double)image[i-1][j] - image[i-1][j+1]) / MAX_VAL;
			p[i][j].ty = (image[i-1][j+1] + 2*(double)image[i][j+1] + image[i+1][j+1]
										- image[i-1][j-1] - 2*(double)image[i][j-1] - image[i+1][j-1]) / MAX_VAL;
			/////////////////////////////////////////////
			v[0] = p[i][j].tx;
			v[1] = p[i][j].ty;
			//////////////////////////////////////////////
			tmp[i][j] = sqrt(p[i][j].tx * p[i][j].tx + p[i][j].ty * p[i][j].ty);
			
			if (tmp[i][j] > max_grad) {
				max_grad = tmp[i][j];
			}
		}
	}
	
	for (i = 1; i <= Nr - 2; i++) {
		tmp[i][0] = tmp[i][1];
		tmp[i][Nc - 1] = tmp[i][Nc - 2];
	}
	
	for (j = 1; j <= Nc - 2; j++) {
		tmp[0][j] = tmp[1][j];
		tmp[Nr - 1][j] = tmp[Nr - 2][j];
	}
	
	tmp[0][0] = ( tmp[0][1] + tmp[1][0] ) / 2;
	tmp[0][Nc-1] = ( tmp[0][Nc-2] + tmp[1][Nc-1] ) / 2;
	tmp[Nr-1][0] = ( tmp[Nr-1][1] + tmp[Nr-2][0] ) / 2;
	tmp[Nr - 1][Nc - 1] = ( tmp[Nr - 1][Nc - 2] + tmp[Nr - 2][Nc - 1] ) / 2;
	
	imatrix gmag(Nr, Nc);
	
	// normalize the magnitude
	for (i = 0; i < Nr; i++) { 
		for (j = 0; j < Nc; j++) {
			tmp[i][j] /= max_grad;
			gmag[i][j] = (int) (tmp[i][j] * 255.0);
		}
	}
	
	for (i = 1; i < Nr - 1; i++) { 
		for (j = 1; j < Nc - 1; j++) {
			////////////////////////////////////////////////////////////////
			p[i][j].tx = (gmag[i+1][j-1] + 2*(double)gmag[i+1][j] + gmag[i+1][j+1] 
										- gmag[i-1][j-1] - 2*(double)gmag[i-1][j] - gmag[i-1][j+1]) / MAX_VAL;
			p[i][j].ty = (gmag[i-1][j+1] + 2*(double)gmag[i][j+1] + gmag[i+1][j+1]
										- gmag[i-1][j-1] - 2*(double)gmag[i][j-1] - gmag[i+1][j-1]) / MAX_VAL;
			/////////////////////////////////////////////
			v[0] = p[i][j].tx;
			v[1] = p[i][j].ty;
			p[i][j].tx = -v[1];
			p[i][j].ty = v[0];
			//////////////////////////////////////////////
			p[i][j].mag = sqrt(p[i][j].tx * p[i][j].tx + p[i][j].ty * p[i][j].ty);
			
			if (p[i][j].mag > max_grad) {
				max_grad = p[i][j].mag;
			}
		}
	}
	
	for (i = 1; i <= Nr - 2; i++) {
		p[i][0].tx = p[i][1].tx;
		p[i][0].ty = p[i][1].ty;
		p[i][0].mag = p[i][1].mag;
		p[i][Nc - 1].tx = p[i][Nc - 2].tx;
		p[i][Nc - 1].ty = p[i][Nc - 2].ty;
		p[i][Nc - 1].mag = p[i][Nc - 2].mag;
	}
	
	for (j = 1; j <= Nc - 2; j++) {
		p[0][j].tx = p[1][j].tx;
		p[0][j].ty = p[1][j].ty;
		p[0][j].mag = p[1][j].mag;
		p[Nr - 1][j].tx = p[Nr - 2][j].tx;
		p[Nr - 1][j].ty = p[Nr - 2][j].ty;
		p[Nr - 1][j].mag = p[Nr - 2][j].mag;
	}
	
	p[0][0].tx = ( p[0][1].tx + p[1][0].tx ) / 2;
	p[0][0].ty = ( p[0][1].ty + p[1][0].ty ) / 2;
	p[0][0].mag = ( p[0][1].mag + p[1][0].mag ) / 2;
	p[0][Nc-1].tx = ( p[0][Nc-2].tx + p[1][Nc-1].tx ) / 2;
	p[0][Nc-1].ty = ( p[0][Nc-2].ty + p[1][Nc-1].ty ) / 2;
	p[0][Nc-1].mag = ( p[0][Nc-2].mag + p[1][Nc-1].mag ) / 2;
	p[Nr-1][0].tx = ( p[Nr-1][1].tx + p[Nr-2][0].tx ) / 2;
	p[Nr-1][0].ty = ( p[Nr-1][1].ty + p[Nr-2][0].ty ) / 2;
	p[Nr-1][0].mag = ( p[Nr-1][1].mag + p[Nr-2][0].mag ) / 2;
	p[Nr - 1][Nc - 1].tx = ( p[Nr - 1][Nc - 2].tx + p[Nr - 2][Nc - 1].tx ) / 2;
	p[Nr - 1][Nc - 1].ty = ( p[Nr - 1][Nc - 2].ty + p[Nr - 2][Nc - 1].ty ) / 2;
	p[Nr - 1][Nc - 1].mag = ( p[Nr - 1][Nc - 2].mag + p[Nr - 2][Nc - 1].mag ) / 2;
	
	normalize();
}


inline void make_unit(double& vx, double& vy)
{
	double mag = sqrt( vx*vx + vy*vy );
	if (mag != 0.0) { 
		vx /= mag; 
		vy /= mag;
	}
}

void ETF::normalize() 
{
	int i, j;
	
	for (i = 0; i < Nr; i++) { 
		for (j = 0; j < Nc; j++) {
			make_unit(p[i][j].tx, p[i][j].ty);
			p[i][j].mag /= max_grad;
		}
	}
}


void ETF::Smooth(int half_w, int M)
{
	int	i, j, k;
	int MAX_GRADIENT = -1;
	double weight;
	int s, t;
	int x, y;
	double mag_diff;
	
	int image_x = getRow();
	int image_y = getCol();
	
	ETF e2; 
	
	e2.init(image_x, image_y); 
	e2.copy(*this); 
	
	double v[2], w[2], g[2];
	double angle;
	double factor;
	
	for (k = 0; k < M; k++) {
		////////////////////////
		// horizontal
		for (j = 0; j < image_y; j++) {
			for (i = 0; i < image_x; i++) {
				g[0] = g[1] = 0.0;
				v[0] = p[i][j].tx;
				v[1] = p[i][j].ty;
				for (s = -half_w; s <= half_w; s++) {
					////////////////////////////////////////
					x = i+s; y = j;
					if (x > image_x-1) x = image_x-1;
					else if (x < 0) x = 0;
					if (y > image_y-1) y = image_y-1;
					else if (y < 0) y = 0;
					////////////////////////////////////////
					mag_diff = p[x][y].mag - p[i][j].mag; 
					//////////////////////////////////////////////////////
					w[0] = p[x][y].tx;
					w[1] = p[x][y].ty;
					////////////////////////////////
					factor = 1.0;
					angle = v[0] * w[0] + v[1] * w[1];
					if (angle < 0.0) {
						factor = -1.0; 
					}
					weight = mag_diff + 1;  
					//////////////////////////////////////////////////////
					g[0] += weight * p[x][y].tx * factor;
					g[1] += weight * p[x][y].ty * factor;
				}
				make_unit(g[0], g[1]);
				e2[i][j].tx = g[0];
				e2[i][j].ty = g[1];
			}
		}
		this->copy(e2);
		/////////////////////////////////
		// vertical
		for (j = 0; j < image_y; j++) {
			for (i = 0; i < image_x; i++) {
				g[0] = g[1] = 0.0;
				v[0] = p[i][j].tx;
				v[1] = p[i][j].ty;
				for (t = -half_w; t <= half_w; t++) {
					////////////////////////////////////////
					x = i; y = j+t;
					if (x > image_x-1) x = image_x-1;
					else if (x < 0) x = 0;
					if (y > image_y-1) y = image_y-1;
					else if (y < 0) y = 0;
					////////////////////////////////////////
					mag_diff = p[x][y].mag - p[i][j].mag; 
					//////////////////////////////////////////////////////
					w[0] = p[x][y].tx;
					w[1] = p[x][y].ty;
					////////////////////////////////
					factor = 1.0;
					///////////////////////////////
					angle = v[0] * w[0] + v[1] * w[1];
					if (angle < 0.0) factor = -1.0; 
					/////////////////////////////////////////////////////////
					weight = mag_diff + 1; 
					//////////////////////////////////////////////////////
					g[0] += weight * p[x][y].tx * factor;
					g[1] += weight * p[x][y].ty * factor;
				}
				make_unit(g[0], g[1]);
				e2[i][j].tx = g[0];
				e2[i][j].ty = g[1];
			}
		}
		this->copy(e2);
	}
	////////////////////////////////////////////
}
