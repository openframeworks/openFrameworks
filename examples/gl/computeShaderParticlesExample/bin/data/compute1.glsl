#version 440

struct Particle{
	vec4 pos;
	vec4 vel;
	vec4 color;
};

layout(std140, binding=0) buffer particle{
    Particle p[];
};

layout(std140, binding=1) buffer particleBack{
    Particle p2[];
};

layout(std140, binding=2) buffer indices{
	uint idx[];
};

uniform float timeLastFrame;
uniform float elapsedTime;
uniform vec3 attractor1;
uniform vec3 attractor2;
uniform vec3 attractor3;
uniform float attraction;
uniform float cohesion;
uniform float repulsion;
uniform float max_speed;
uniform float attr1_force;
uniform float attr2_force;
uniform float attr3_force;

vec3 rule1(vec3 my_pos, vec3 their_pos){
	vec3 dir = my_pos-their_pos;
	float sqd = dot(dir,dir);
	if(sqd < 300.0*300.0){
		return dir;
	}
	return vec3(0.0);
} 

vec3 rule2(vec3 my_pos, vec3 their_pos, vec3 my_vel, vec3 their_vel){
	vec3 d = their_pos - my_pos;
	vec3 dv = their_vel - my_vel;
	return dv / (dot(dv,dv) + 10.0);
}

vec3 rule3(vec3 my_pos, vec3 their_pos){
	vec3 dir = their_pos-my_pos;
	float sqd = dot(dir,dir);
	if(sqd < 50.0*50.0){
		float f = 1000000.0/sqd;
		return normalize(dir)*f;
	}
	return vec3(0.0);
}

layout(local_size_x = 1024, local_size_y = 1, local_size_z = 1) in;
void main(){
	vec3 particle = p2[gl_GlobalInvocationID.x].pos.xyz;
	vec3 acc = vec3(0.0,0.0,0.0);
	uint m = uint(1024.0*8.0*elapsedTime);
	uint start = m%(1024*8-512);
	uint end = start + 512;
	float minDist;
	uint first = 1;
	for(uint i=start;i<end;i++){
		if(i!=gl_GlobalInvocationID.x){
			acc += rule1(particle,p2[i].pos.xyz) * repulsion;
			acc += rule2(particle,p2[i].pos.xyz, p2[gl_GlobalInvocationID.x].vel.xyz, p2[i].vel.xyz) * cohesion;
			acc += rule3(particle,p2[i].pos.xyz) * attraction;
		}
	}
	
	p[gl_GlobalInvocationID.x].pos.xyz += p[gl_GlobalInvocationID.x].vel.xyz*timeLastFrame;
	
	if(gl_GlobalInvocationID.x%2==1){
		vec3 dir = attractor1 - particle;
		acc += normalize(dir)*attr1_force;
	}
	
	
	if(gl_GlobalInvocationID.x%2==0){
		vec3 dir = attractor2 - particle;
		acc += normalize(dir)*attr2_force;
	}
	
	vec3 dir = attractor3 - particle;
	acc += normalize(dir)*attr3_force;
	
	p[gl_GlobalInvocationID.x].vel.xyz += acc*timeLastFrame;
	p[gl_GlobalInvocationID.x].vel.xyz *= 0.99;
	
	dir = normalize(p[gl_GlobalInvocationID.x].vel.xyz);
	
	if(length(p[gl_GlobalInvocationID.x].vel.xyz)>max_speed){
		p[gl_GlobalInvocationID.x].vel.xyz = dir * max_speed;
	}
	
	float max_component = max(max(dir.x,dir.y),dir.z);
	p[gl_GlobalInvocationID.x].color.rgb = dir/max_component;
	p[gl_GlobalInvocationID.x].color.a = 0.4;
}