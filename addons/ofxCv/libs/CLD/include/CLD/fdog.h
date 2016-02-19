
#ifndef _FDOG_H_
#define _FDOG_H_

extern void GaussSmoothSep(imatrix& image, double sigma);
extern void ConstructMergedImage(imatrix& image, imatrix& gray, imatrix& merged); 
extern void ConstructMergedImageMult(imatrix& image, imatrix& gray, imatrix& merged); 
extern void GetFDoG(imatrix& image, ETF& e, double sigma, double sigma3, double tau); 
extern void Binarize(imatrix& image, double thres); 
extern void GrayThresholding(imatrix& image, double thres); 

#endif