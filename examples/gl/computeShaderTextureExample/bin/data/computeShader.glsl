#version 440

layout(rgba8,binding=0) uniform readonly image2D src;
layout(rgba8,binding=1) uniform readonly image2D bg;
layout(r8,binding=2) uniform writeonly image2D dst;


layout(local_size_x = 32, local_size_y = 30, local_size_z = 1) in;
void main(){
	vec4 c = imageLoad(src,ivec2(gl_GlobalInvocationID.xy));
	vec4 cbg = imageLoad(bg,ivec2(gl_GlobalInvocationID.xy));
	float gray = 0.299*c.r + 0.587*c.g + 0.114*c.b;
	float grayBg = 0.299*cbg.r + 0.587*cbg.g + 0.114*cbg.b;
	float diff = abs(grayBg - gray);
	if(diff>0.6){
		imageStore(dst,ivec2(gl_GlobalInvocationID.xy),vec4(255));
	}else{
		imageStore(dst,ivec2(gl_GlobalInvocationID.xy),vec4(0));
	}
}