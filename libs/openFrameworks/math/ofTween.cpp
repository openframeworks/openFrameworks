#include "ofTween.h"

float ofMap(float value, float inputMin, float inputMax, float outputMin, float outputMax, ofEasingFunction easing, bool clamp){
	if(clamp){
		value = ofClamp(value, inputMin, inputMax);
	}
	float t = value - inputMin;
	float c = outputMax - outputMin;
	float d = inputMax - inputMin;
	float b = outputMin;

	return easing(t,b,c,d);
}

float ofEasing::backIn (float t,float b , float c, float d) {
	float s = 1.70158f;
	float postFix = t/=d;
	return c*(postFix)*t*((s+1)*t - s) + b;
}

float ofEasing::backOut(float t,float b , float c, float d) {
	float s = 1.70158f;
	return c*((t=t/d-1)*t*((s+1)*t + s) + 1) + b;
}

float ofEasing::backInOut(float t,float b , float c, float d) {
	float s = 1.70158f;
	if ((t/=d/2) < 1) return c/2*(t*t*(((s*=(1.525f))+1)*t - s)) + b;
	float postFix = t-=2;
	return c/2*((postFix)*t*(((s*=(1.525f))+1)*t + s) + 2) + b;
}

float ofEasing::bounceIn (float t,float b , float c, float d) {
	return c - bounceOut (d-t, 0, c, d) + b;
}

float ofEasing::bounceOut(float t,float b , float c, float d) {
	if ((t/=d) < (1/2.75f)) {
		return c*(7.5625f*t*t) + b;
	} else if (t < (2/2.75f)) {
		float postFix = t-=(1.5f/2.75f);
		return c*(7.5625f*(postFix)*t + .75f) + b;
	} else if (t < (2.5/2.75)) {
			float postFix = t-=(2.25f/2.75f);
		return c*(7.5625f*(postFix)*t + .9375f) + b;
	} else {
		float postFix = t-=(2.625f/2.75f);
		return c*(7.5625f*(postFix)*t + .984375f) + b;
	}
}


float ofEasing::bounceInOut(float t,float b , float c, float d) {
	if (t < d/2) return bounceIn (t*2, 0, c, d) * .5f + b;
	else return bounceOut (t*2-d, 0, c, d) * .5f + c*.5f + b;
}

float ofEasing::circIn (float t,float b , float c, float d) {
	return -c * (sqrt(1 - (t/=d)*t) - 1) + b;
}

float ofEasing::circOut(float t,float b , float c, float d) {
	return c * sqrt(1 - (t=t/d-1)*t) + b;
}

float ofEasing::circInOut(float t,float b , float c, float d) {
	if ((t/=d/2) < 1) return -c/2 * (sqrt(1 - t*t) - 1) + b;
	return c/2 * (sqrt(1 - t*(t-=2)) + 1) + b;
}

float ofEasing::cubicIn (float t,float b , float c, float d) {
	return c*(t/=d)*t*t + b;
}

float ofEasing::cubicOut(float t,float b , float c, float d) {
	return c*((t=t/d-1)*t*t + 1) + b;
}

float ofEasing::cubicInOut(float t,float b , float c, float d) {
	if ((t/=d/2) < 1) return c/2*t*t*t + b;
	return c/2*((t-=2)*t*t + 2) + b;
}

float ofEasing::elasticIn (float t,float b , float c, float d) {
	if (t==0) return b;  if ((t/=d)==1) return b+c;
	float p=d*.3f;
	float a=c;
	float s=p/4;
	float postFix =a*pow(2,10*(t-=1)); // this is a fix, again, with post-increment operators
	return -(postFix * sin((t*d-s)*(2*PI)/p )) + b;
}

float ofEasing::elasticOut(float t,float b , float c, float d) {
	if (t==0) return b;  if ((t/=d)==1) return b+c;
	float p=d*.3f;
	float a=c;
	float s=p/4;
	return (a*pow(2,-10*t) * sin( (t*d-s)*(2*PI)/p ) + c + b);
}

float ofEasing::elasticInOut(float t,float b , float c, float d) {
	if (t==0) return b;  if ((t/=d/2)==2) return b+c;
	float p=d*(.3f*1.5f);
	float a=c;
	float s=p/4;

	if (t < 1) {
		float postFix =a*pow(2,10*(t-=1)); // postIncrement is evil
		return -.5f*(postFix* sin( (t*d-s)*(2*PI)/p )) + b;
	}
	float postFix =  a*pow(2,-10*(t-=1)); // postIncrement is evil
	return postFix * sin( (t*d-s)*(2*PI)/p )*.5f + c + b;
}

float ofEasing::expoIn (float t,float b , float c, float d) {
	return (t==0) ? b : c * pow(2, 10 * (t/d - 1)) + b;
}

float ofEasing::expoOut(float t,float b , float c, float d) {
	return (t==d) ? b+c : c * (-pow(2, -10 * t/d) + 1) + b;
}

float ofEasing::expoInOut(float t,float b , float c, float d) {
	if (t==0) return b;
	if (t==d) return b+c;
	if ((t/=d/2) < 1) return c/2 * pow(2, 10 * (t - 1)) + b;
	return c/2 * (-pow(2, -10 * --t) + 2) + b;
}

float ofEasing::quadIn (float t,float b , float c, float d) {
	return c*(t/=d)*t + b;
}

float ofEasing::quadOut(float t,float b , float c, float d) {
	return -c *(t/=d)*(t-2) + b;
}

float ofEasing::quadInOut(float t,float b , float c, float d) {
	if ((t/=d/2) < 1) return c/2*t*t + b;
	return -c/2 * ((--t)*(t-2) - 1) + b;

	/*

	originally return -c/2 * (((t-2)*(--t)) - 1) + b;

	I've had to swap (--t)*(t-2) due to diffence in behaviour in
	pre-increment operators between java and c++, after hours
	of joy

	 James George:: The fix refered to above actually broke the equation,
	 it would land at 50% all the time at the end
	 copying back the original equation from online fixed it...

	 potentially compiler dependent.
	*/

}

float ofEasing::quartIn (float t,float b , float c, float d) {
	return c*(t/=d)*t*t*t + b;
}

float ofEasing::quartOut(float t,float b , float c, float d) {
	return -c * ((t=t/d-1)*t*t*t - 1) + b;
}

float ofEasing::quartInOut(float t,float b , float c, float d) {
	if ((t/=d/2) < 1) return c/2*t*t*t*t + b;
	return -c/2 * ((t-=2)*t*t*t - 2) + b;
}

float ofEasing::quintIn (float t,float b , float c, float d) {
	return c*(t/=d)*t*t*t*t + b;
}

float ofEasing::quintOut(float t,float b , float c, float d) {
	return c*((t=t/d-1)*t*t*t*t + 1) + b;
}

float ofEasing::quintInOut(float t,float b , float c, float d) {
	if ((t/=d/2) < 1) return c/2*t*t*t*t*t + b;
	return c/2*((t-=2)*t*t*t*t + 2) + b;
}

float ofEasing::sineIn (float t,float b , float c, float d) {
	return -c * cos(t/d * (PI/2)) + c + b;
}

float ofEasing::sineOut(float t,float b , float c, float d) {
	return c * sin(t/d * (PI/2)) + b;
}

float ofEasing::sineInOut(float t,float b , float c, float d) {
	return -c/2 * (cos(PI*t/d) - 1) + b;
}
