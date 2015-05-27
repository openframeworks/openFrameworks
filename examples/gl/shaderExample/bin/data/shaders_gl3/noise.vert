#version 150

in vec4 position;
in vec2 texcoord;

uniform mat4 modelViewProjectionMatrix;


uniform float timeValX;
uniform float timeValY;
uniform vec2 mouse;

//generate a random value from four points
vec4 rand(vec2 A,vec2 B,vec2 C,vec2 D){ 

	vec2 s=vec2(12.9898,78.233); 
	vec4 tmp=vec4(dot(A,s),dot(B,s),dot(C,s),dot(D,s)); 

	return fract(sin(tmp) * 43758.5453)* 2.0 - 1.0; 
} 

//this is similar to a perlin noise function
float noise(vec2 coord,float d){ 

	vec2 C[4]; 

	float d1 = 1.0/d;

	C[0]=floor(coord*d)*d1; 

	C[1]=C[0]+vec2(d1,0.0); 

	C[2]=C[0]+vec2(d1,d1); 

	C[3]=C[0]+vec2(0.0,d1);


	vec2 p=fract(coord*d); 

	vec2 q=1.0-p; 

	vec4 w=vec4(q.x*q.y,p.x*q.y,p.x*p.y,q.x*p.y); 

	return dot(vec4(rand(C[0],C[1],C[2],C[3])),w); 
} 


void main(){
    
	//get our current vertex position so we can modify it
	vec4 pos = modelViewProjectionMatrix * position;
	
	//generate some noise values based on vertex position and the time value which comes in from our OF app
	float noiseAmntX = noise( vec2(-timeValX + pos.x / 1000.0, 100.0), 20.0 );
	float noiseAmntY = noise( vec2(timeValY + pos.y / 1000.0, pos.x / 2000.0), 20.0 );

	//generate noise for our blue pixel value
	float noiseB = noise( vec2(timeValY * 0.25, pos.y / 2000.0), 20.0 );

	//lets also figure out the distance between the mouse and the vertex and apply a repelling force away from the mouse
	vec2 d = vec2(pos.x, pos.y) - mouse;
	float len =  sqrt(d.x*d.x + d.y*d.y);
	if( len < 300.0 && len > 0.0  ){
		
		//lets get the distance into 0-1 ranges
		float pct = len / 300.0; 
		
		//this turns our linear 0-1 value into a curved 0-1 value
		pct *= pct;

		//flip it so the closer we are the greater the repulsion
		pct = 1.0 - pct;
		
		//normalize our repulsion vector
		d /= len;
		
		//apply the repulsion to our position
		pos.x += d.x * pct * 90.0;
		pos.y += d.y * pct * 90.0;
	}

	//modify our position with the smooth noise
	pos.x += noiseAmntX * 20.0;
	pos.y += noiseAmntY * 10.0;
	
	//finally set the pos to be that actual position rendered
	gl_Position = pos;
}
