/*
 * TERMS OF USE - EASING EQUATIONS
 *
 * Open source under the BSD License.
 *
 * Copyright © 2001 Robert Penner
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *   Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *   Neither the name of the author nor the names of contributors may be used
 *   to endorse or promote products derived from this software without specific
 *   prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "ofMath.h"

typedef float (*ofEasingFunction) (float,float,float,float);

float ofMap(float value, float inputMin, float inputMax, float outputMin, float outputMax, ofEasingFunction easing, bool clamp=true);

class ofEasing{
public:
	static float backIn (float t,float b , float c, float d);
	static float backOut(float t,float b , float c, float d);
	static float backInOut(float t,float b , float c, float d);

	static float bounceIn (float t,float b , float c, float d);
	static float bounceOut(float t,float b , float c, float d);
	static float bounceInOut(float t,float b , float c, float d);

	static float circIn (float t,float b , float c, float d);
	static float circOut(float t,float b , float c, float d);
	static float circInOut(float t,float b , float c, float d);

	static float cubicIn (float t,float b , float c, float d);
	static float cubicOut(float t,float b , float c, float d);
	static float cubicInOut(float t,float b , float c, float d);

	static float elasticIn (float t,float b , float c, float d);
	static float elasticOut(float t,float b , float c, float d);
	static float elasticInOut(float t,float b , float c, float d);

	static float expoIn (float t,float b , float c, float d);
	static float expoOut(float t,float b , float c, float d);
	static float expoInOut(float t,float b , float c, float d);

	static float quadIn (float t,float b , float c, float d);
	static float quadOut(float t,float b , float c, float d);
	static float quadInOut(float t,float b , float c, float d);

	static float quartIn (float t,float b , float c, float d);
	static float quartOut(float t,float b , float c, float d);
	static float quartInOut(float t,float b , float c, float d);

	static float quintIn (float t,float b , float c, float d);
	static float quintOut(float t,float b , float c, float d);
	static float quintInOut(float t,float b , float c, float d);

	static float sineIn (float t,float b , float c, float d);
	static float sineOut(float t,float b , float c, float d);
	static float sineInOut(float t,float b , float c, float d);
};
