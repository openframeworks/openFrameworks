//#include "stdafx.h"
#include "ofMain.h"
#include <cmath>

#include "ETF.h"
#include "fdog.h"
#include "myvec.h"
#include "imatrix.h"

#ifndef ABS
	#define ABS(x) ( ((x)>0) ? (x) : (-(x)) )
#endif
#define round(x) ((int) ((x) + 0.5))

inline double gauss(double x, double mean, double sigma)
{
	return ( exp( (-(x-mean)*(x-mean)) / (2*sigma*sigma) ) / sqrt(PI * 2.0 * sigma * sigma) );
}

void MakeGaussianVector(double sigma, myvec& GAU)
{
	int i, j;
	
	double threshold = 0.001;
	
	i = 0;
	while(1) {
		i++;
		if ( gauss((double)i, 0.0, sigma) < threshold )
			break;
	}
	GAU.init(i+1); 
	GAU.zero(); 
	
	GAU[0] = gauss((double)0.0, 0.0, sigma);
	for (j = 1; j < GAU.getMax(); j++) {
		GAU[j] = gauss((double)j, 0.0, sigma);
	}
}

void GetDirectionalDoG(imatrix& image, ETF& e, mymatrix& dog, myvec& GAU1, myvec& GAU2, double tau)
{
	myvec vn(2);
	double x, y, d_x, d_y;
	double weight1, weight2, w_sum1, sum1, sum2, w_sum2;
	
	int s;
	int x1, y1;
	int i, j;
	int dd;
	double val;
	
	int half_w1, half_w2;
	
	half_w1 = GAU1.getMax()-1;
	half_w2 = GAU2.getMax()-1;
	
	int image_x, image_y;
	
	image_x = image.getRow();
	image_y = image.getCol();
	
	for (i = 0; i < image_x; i++) {
		for (j = 0; j < image_y; j++) {
			sum1 = sum2 = 0.0;
			w_sum1 = w_sum2 = 0.0;
			weight1 = weight2 = 0.0;
			
			vn[0] = -e[i][j].ty;
			vn[1] = e[i][j].tx;
			
			if (vn[0] == 0.0 && vn[1] == 0.0) {
				sum1 = 255.0;
				sum2 = 255.0;
				dog[i][j] = sum1 - tau * sum2;
				continue;
			}
			d_x = i; d_y = j;
			////////////////////////////////////////
			for (s = -half_w2; s <= half_w2; s++) { 
				////////////////////////
				x = d_x + vn[0] * s;
				y = d_y + vn[1] * s;
				/////////////////////////////////////////////////////
				if (x > (double)image_x-1 || x < 0.0 || y > (double)image_y-1 || y < 0.0) 
					continue;
				x1 = round(x);	if (x1 < 0) x1 = 0; if (x1 > image_x-1) x1 = image_x-1;
				y1 = round(y);	if (y1 < 0) y1 = 0; if (y1 > image_y-1) y1 = image_y-1;
				val = image[x1][y1];
				/////////////////////////////////////////////////////////
				dd = ABS(s);
				if (dd > half_w1) weight1 = 0.0;
				else weight1 = GAU1[dd];
				//////////////////////////////////
				sum1 += val * weight1;
				w_sum1 += weight1;
				/////////////////////////////////////////////////////
				weight2 = GAU2[dd];
				sum2 += val * weight2;
				w_sum2 += weight2;
			}
			/////////////////////////
			sum1 /= w_sum1; 
			sum2 /= w_sum2; 
			//////////////////////////////////////
			dog[i][j] = sum1 - tau * sum2;
		}
	}
	
}

void GetFlowDoG(ETF& e, mymatrix& dog, mymatrix& tmp, myvec& GAU3)
{
	myvec vt(2);
	double x, y, d_x, d_y;
	double weight1, w_sum1, sum1;
	
	int i_x, i_y, k;
	int x1, y1;
	double val;
	int i, j;
	
	int image_x = dog.getRow();
	int image_y = dog.getCol();
	
	int half_l;
	half_l = GAU3.getMax()-1;
	
	int flow_DOG_sign = 0; 
	
	double step_size = 1.0; 
	
	for (i = 0; i < image_x; i++) {
		for (j = 0; j < image_y; j++) {
			sum1 = 0.0;
			w_sum1 = 0.0;
			weight1 = 0.0;
			/////////////////////////////////
			val = dog[i][j];
			weight1 = GAU3[0]; 
			sum1 = val * weight1;
			w_sum1 += weight1;
			////////////////////////////////////////////////
			d_x = (double)i; d_y = (double)j; 
			i_x = i; i_y = j;
			////////////////////////////
			for (k = 0; k < half_l; k++) {
				vt[0] = e[i_x][i_y].tx;
				vt[1] = e[i_x][i_y].ty;
				if (vt[0] == 0.0 && vt[1] == 0.0) {
					break;
				}
				x = d_x;
				y = d_y;
				/////////////////////////////////////////////////////
				if (x > (double)image_x-1 || x < 0.0 || y > (double)image_y-1 || y < 0.0) 
					break;
				x1 = round(x);	if (x1 < 0) x1 = 0; if (x1 > image_x-1) x1 = image_x-1;
				y1 = round(y);	if (y1 < 0) y1 = 0; if (y1 > image_y-1) y1 = image_y-1;
				val = dog[x1][y1];
				//////////////////////////////
				weight1 = GAU3[k]; 
				////////////////////
				sum1 += val * weight1;
				w_sum1 += weight1;
				/////////////////////////////////////////
				d_x += vt[0] * step_size; 
				d_y += vt[1] * step_size; 
				/////////////////////////////////////////
				i_x = round(d_x); 
				i_y = round(d_y); 
				if (d_x < 0 || d_x > image_x-1 || d_y < 0 || d_y > image_y-1) break;
				/////////////////////////
			}
			////////////////////////////////////////////////
			d_x = (double)i; d_y = (double)j; 
			i_x = i; i_y = j;
			for (k = 0; k < half_l; k++) {
				vt[0] = -e[i_x][i_y].tx;
				vt[1] = -e[i_x][i_y].ty;
				if (vt[0] == 0.0 && vt[1] == 0.0) {
					break;
				}
				x = d_x;
				y = d_y;
				/////////////////////////////////////////////////////
				if (x > (double)image_x-1 || x < 0.0 || y > (double)image_y-1 || y < 0.0) 
					break;
				x1 = round(x);	if (x1 < 0) x1 = 0; if (x1 > image_x-1) x1 = image_x-1;
				y1 = round(y);	if (y1 < 0) y1 = 0; if (y1 > image_y-1) y1 = image_y-1;
				val = dog[x1][y1];
				//////////////////////////////
				weight1 = GAU3[k]; 
				////////////////////
				sum1 += val * weight1;
				w_sum1 += weight1;
				/////////////////////////////////////////
				d_x += vt[0] * step_size; 
				d_y += vt[1] * step_size; 
				/////////////////////////////////////////
				i_x = round(d_x); 
				i_y = round(d_y); 
				if (d_x < 0 || d_x > image_x-1 || d_y < 0 || d_y > image_y-1) break;
				/////////////////////////
			}
			////////////////////////////////////////
			sum1 /= w_sum1; 
			//////////////////////////////////////
			if (sum1 > 0) tmp[i][j] = 1.0; 
			else tmp[i][j] = 1.0 + tanh(sum1);
		}
	}
}

void GetFDoG(imatrix& image, ETF& e, double sigma, double sigma3, double tau) 
{
	int	i, j;
	
	int image_x = image.getRow();
	int image_y = image.getCol();
	
	myvec GAU1, GAU2, GAU3;
	MakeGaussianVector(sigma, GAU1); 
	MakeGaussianVector(sigma*1.6, GAU2); 
	
	int half_w1, half_w2, half_l;
	half_w1 = GAU1.getMax()-1;
	half_w2 = GAU2.getMax()-1;
	
	MakeGaussianVector(sigma3, GAU3); 
	half_l = GAU3.getMax()-1;
	
	mymatrix tmp(image_x, image_y);
	mymatrix dog(image_x, image_y);
	
	GetDirectionalDoG(image, e, dog, GAU1, GAU2, tau);
	GetFlowDoG(e, dog, tmp, GAU3);
	
	for (i = 0; i < image_x; i++) { 
		for (j = 0; j < image_y; j++) {
			image[i][j] = round(tmp[i][j] * 255.);
		}
	}
}

void GaussSmoothSep(imatrix& image, double sigma)
{
	int	i, j;
	int MAX_GRADIENT = -1;
	double g, max_g, min_g;
	int s, t;
	int x, y;
	double weight, w_sum;
	
	int image_x = image.getRow();
	int image_y = image.getCol();
	
	myvec GAU1;
	MakeGaussianVector(sigma, GAU1); 
	int half = GAU1.getMax()-1;
	
	mymatrix tmp(image_x, image_y);
	
	max_g = -1;
	min_g = 10000000;
	for (j = 0; j < image_y; j++) {
		for (i = 0; i < image_x; i++) {
			g = 0.0;
			weight = w_sum = 0.0;
			for (s = -half; s <= half; s++) {
				x = i+s; y = j;
				if (x > image_x-1) x = image_x-1;
				else if (x < 0) x = 0;
				if (y > image_y-1) y = image_y-1;
				else if (y < 0) y = 0;
				weight = GAU1[ABS(s)];
				g += weight * image[x][y];
				w_sum += weight;
			}
			g /= w_sum;
			if (g > max_g) max_g = g;
			if (g < min_g) min_g = g;
			tmp[i][j] = g;
		}
	}
	for (j = 0; j < image_y; j++) {
		for (i = 0; i < image_x; i++) {
			g = 0.0;
			weight = w_sum = 0.0;
			for (t = -half; t <= half; t++) {
				x = i; y = j+t;
				if (x > image_x-1) x = image_x-1;
				else if (x < 0) x = 0;
				if (y > image_y-1) y = image_y-1;
				else if (y < 0) y = 0;
				weight = GAU1[ABS(t)];
				g += weight * tmp[x][y];
				w_sum += weight;
			}
			g /= w_sum;
			if (g > max_g) max_g = g;
			if (g < min_g) min_g = g;
			image[i][j] = round(g);
		}
	}
	
	printf("max_g = %f\n", max_g);
	printf("min_g = %f\n", min_g);
}

void ConstructMergedImage(imatrix& image, imatrix& gray, imatrix& merged) 
{
	int x, y;
	
	int image_x = image.getRow();
	int image_y = image.getCol();
	
	for (y = 0; y < image_y; y++) {
		for (x = 0; x < image_x; x++) {
			if (gray[x][y] == 0) merged[x][y] = 0;
			else merged[x][y] = image[x][y];
		}
	}
}

void ConstructMergedImageMult(imatrix& image, imatrix& gray, imatrix& merged) 
// using multiplication
{
	int x, y;
	double gray_val, line_darkness;
	
	int image_x = image.getRow();
	int image_y = image.getCol();
	
	for (y = 0; y < image_y; y++) {
		for (x = 0; x < image_x; x++) {
			gray_val = image[x][y] / 255.0; 
			line_darkness = gray[x][y] / 255.0; 
			gray_val *= line_darkness; 
			merged[x][y] = round(gray_val * 255.0);
		}
	}
}

void Binarize(imatrix& image, double thres) 
{
	int	i, j;
	double val;
	
	int image_x = image.getRow();
	int image_y = image.getCol();
	
	for (i = 0; i < image_x; i++) { 
		for (j = 0; j < image_y; j++) {
			val = image[i][j] / 255.0; 
			if (val < thres)
				image[i][j] = 0; 
			else image[i][j] = 255; 
		}
	}
}

void GrayThresholding(imatrix& image, double thres) 
{
	int	i, j;
	double val;
	
	int image_x = image.getRow();
	int image_y = image.getCol();
	
	for (i = 0; i < image_x; i++) { 
		for (j = 0; j < image_y; j++) {
			val = image[i][j] / 255.0; 
			if (val < thres)
				image[i][j] = round(val * 255.0); 
			else image[i][j] = 255; 
		}
	}
}

