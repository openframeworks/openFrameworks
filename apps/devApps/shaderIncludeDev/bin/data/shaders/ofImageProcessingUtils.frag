
#pragma include "shaders/ofImageProcessingUtils.frag" 
#pragma include "shaders/ofBlendingUtils.frag"
  
vec4 blurH(sampler2DRect tex, vec2 texCoord, float scale){
	vec4 sum = vec4(0.0);

	sum += texture2DRect(tex, texCoord.xy - vec2(4, 0)*scale) * 0.05;
	sum += texture2DRect(tex, texCoord.xy - vec2(3, 0)*scale) * 0.09;
	sum += texture2DRect(tex, texCoord.xy - vec2(2, 0)*scale) * 0.12;
	sum += texture2DRect(tex, texCoord.xy - vec2(1, 0)*scale) * 0.15;
	sum += texture2DRect(tex, texCoord.xy) * 0.16;
	sum += texture2DRect(tex, texCoord.xy + vec2(1, 0)*scale) * 0.15;
	sum += texture2DRect(tex, texCoord.xy + vec2(2, 0)*scale) * 0.12;
	sum += texture2DRect(tex, texCoord.xy + vec2(3, 0)*scale) * 0.09;
	sum += texture2DRect(tex, texCoord.xy + vec2(4, 0)*scale) * 0.05;

	return sum;
}

vec4 blurV(sampler2DRect tex, vec2 texCoord, float scale){
	vec4 sum = vec4(0.0);

	sum += texture2DRect(tex, texCoord.xy - vec2(0, 4)*scale) * 0.05;
	sum += texture2DRect(tex, texCoord.xy - vec2(0, 3)*scale) * 0.09;
	sum += texture2DRect(tex, texCoord.xy - vec2(0, 2)*scale) * 0.12;
	sum += texture2DRect(tex, texCoord.xy - vec2(0, 1)*scale) * 0.15;
	sum += texture2DRect(tex, texCoord.xy) * 0.16;
	sum += texture2DRect(tex, texCoord.xy + vec2(0, 1)*scale) * 0.15;
	sum += texture2DRect(tex, texCoord.xy + vec2(0, 2)*scale) * 0.12;
	sum += texture2DRect(tex, texCoord.xy + vec2(0, 3)*scale) * 0.09;
	sum += texture2DRect(tex, texCoord.xy + vec2(0, 4)*scale) * 0.05;

	return sum;
}
