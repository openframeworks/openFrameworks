//
//     _____    ___
//    /    /   /  /     vboMeshDrawInstancedExample
//   /  __/ * /  /__    (c) ponies & light, 2013. All rights reserved.
//  /__/     /_____/    poniesandlight.co.uk
//
//  Created by tim on 30/07/2013.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#version 120
#extension GL_EXT_gpu_shader4 : require

uniform vec4 globalColor = vec4(1.0);
uniform	sampler2D tex0;			// we use this to sample depth data for our boxes.
uniform float timeValue=0.0;	// we get this from our ofApp, it's a time value moving from 0 to 1 every 30 seconds

varying vec4 colorVarying;

// ---------- A simplex noise helper method to please the eye:

// ---------
//
// Description : Array and textureless GLSL 2D simplex noise function.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
//

vec3 mod289(vec3 x) {
	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
	return mod289(((x*34.0)+1.0)*x);
}

float snoise(vec2 v)
{
	const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
						0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
						-0.577350269189626,  // -1.0 + 2.0 * C.x
						0.024390243902439); // 1.0 / 41.0
											// First corner
	vec2 i  = floor(v + dot(v, C.yy) );
	vec2 x0 = v -   i + dot(i, C.xx);
	
	// Other corners
	vec2 i1;
	//i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
	//i1.y = 1.0 - i1.x;
	i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
	// x0 = x0 - 0.0 + 0.0 * C.xx ;
	// x1 = x0 - i1 + 1.0 * C.xx ;
	// x2 = x0 - 1.0 + 2.0 * C.xx ;
	vec4 x12 = x0.xyxy + C.xxzz;
	x12.xy -= i1;
	
	// Permutations
	i = mod289(i); // Avoid truncation effects in permutation
	vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
					 + i.x + vec3(0.0, i1.x, 1.0 ));
	
	vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
	m = m*m ;
	m = m*m ;
	
	// Gradients: 41 points uniformly over a line, mapped onto a diamond.
	// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)
	
	vec3 x = 2.0 * fract(p * C.www) - 1.0;
	vec3 h = abs(x) - 0.5;
	vec3 ox = floor(x + 0.5);
	vec3 a0 = x - ox;
	
	// Normalise gradients implicitly by scaling m
	// Approximation of: m *= inversesqrt( a0*a0 + h*h );
	m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
	
	// Compute final noise value at P
	vec3 g;
	g.x  = a0.x  * x0.x  + h.x  * x0.y;
	g.yz = a0.yz * x12.xz + h.yz * x12.yw;
	return 130.0 * dot(m, g);
}

// --------- / end simplex noise



// ---------------------------------------------------------------------- 

void main()
{

	float iCount = 128;
	
	// when drawing instanced geometry, we can use gl_InstanceID
	// this tells you which primitive we are currently working on
	
	// we would like to spread our primitives out evenly along the x and an y coordinates
	// we calculate an x and an y coordinate value based on the current instance ID
	
	float instanceX = (mod(gl_InstanceID,iCount) - iCount/2.0) / 128.0;
	float instanceY = ((gl_InstanceID/iCount) - iCount/2.0) / 128.0;
	
	// next we get a mix-value, based on the current instance x coordinate and time, which
	// will help us to achieve some animation happiness.
	
	float timeDependentInstanceXValue = mod(instanceX * 0.25 + timeValue *2, 1.0);
	
	
	// get pixel depth by sampling from our depth texture.
	// we sample from the deph texture, but because our sample position moves along
	// along the x-coodinate depending on time, the result will animate.
	
	vec2 samplePos = vec2(timeDependentInstanceXValue, instanceY + 0.5);
	
	float pixelDepth = texture2D(tex0,samplePos).g;
	
	// set the color for this primitive based on the current pixelDepth modified by the time dependent instance x value.
	colorVarying = vec4(globalColor.rgb * pixelDepth, globalColor.a) * vec4(1.0,1.0-timeDependentInstanceXValue,(instanceX + 0.5) ,1.0);
	
	vec4 vPos = gl_Vertex;
	
	// extrude our primitive along the z coordinate, based on current PixelDepth.
	vPos.z = (vPos.z + 1.0) * pixelDepth * 200.0;
	// this will pull pur vertices apart by four time their original coordinate values,
	// then move them by an snoise value which is the same for every primitive
	// simplex noise makes things look a little more organic.
	vPos.x = vPos.x * 4.0 + snoise(vec2(instanceX,instanceY)) * 20.0;
	vPos.y = vPos.y * 4.0 + snoise(vec2(1.0-instanceX,instanceY)) * 300.0;
	// this will distribute our boxes in space,
	vPos = vPos + vec4(instanceX*20.0*128.0,instanceY*10.0*128.0, 0.0,0.0);
	vPos.z = vPos.z + snoise(vec2(instanceX,instanceY)) * 100.0;


	gl_Position = gl_ModelViewProjectionMatrix * vPos  ;
}