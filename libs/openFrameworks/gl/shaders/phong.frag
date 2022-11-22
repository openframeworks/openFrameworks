static const string fragmentShader = R"(
    IN vec2 v_texcoord; // phong receive the texCoord
    IN vec3 v_normal;
    IN vec3 v_transformedNormal;
    // Eye-coordinate position of vertex
    IN vec3 v_eyePosition;
    IN vec3 v_worldPosition;
	IN vec3 v_worldNormal;
#if HAS_COLOR
    IN vec4 v_color;
#endif
	uniform mat4 viewMatrix;


    struct lightData
    {
        float enabled;
        vec4 ambient;
        float type; // 0 = pointlight 1 = directionlight
        vec4 position; // where are we
        vec4 diffuse; // how diffuse
        vec4 specular; // what kinda specular stuff we got going on?
        // attenuation
        float constantAttenuation;
        float linearAttenuation;
        float quadraticAttenuation;
        // only for spot
        float spotCutoff;
        float spotCosCutoff;
        float spotExponent;
        // spot and area
        vec3 spotDirection;
        // only for directional
        vec3 halfVector;
        // only for area
        float width;
        float height;
        vec3 right;
        vec3 up;
    };

    uniform SAMPLER tex0;

    uniform vec4 mat_ambient;
    uniform vec4 mat_diffuse;
    uniform vec4 mat_specular;
    uniform vec4 mat_emissive;
    uniform float mat_shininess;
    uniform vec4 global_ambient;

    // these are passed in from OF programmable renderer
    uniform mat4 modelViewMatrix;
    uniform mat4 projectionMatrix;
    uniform mat4 textureMatrix;
    uniform mat4 modelViewProjectionMatrix;

    uniform lightData lights[MAX_LIGHTS];

	
	%shader_shadow_include%

	%custom_uniforms%

	#ifdef HAS_TEX_NORMAL
	//-- normal map code from ------------------------------------ //
	// http://www.geeks3d.com/20130122/normal-mapping-without-precomputed-tangent-space-vectors/
	// http://www.thetenthplanet.de/archives/1180
	mat3 CotangentFrame(in vec3 N, in vec3 p, in vec2 uv) {
		// get edge vectors of the pixel triangle
		vec3 dp1 = dFdx(p);
		vec3 dp2 = dFdy(p);
		vec2 duv1 = dFdx(uv);
		vec2 duv2 = dFdy(uv);
		
		// solve the linear system
		vec3 dp2perp = cross(dp2, N);
		vec3 dp1perp = cross(N, dp1);
		vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
		vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
		
		// construct a scale-invariant frame
		float invmax = inversesqrt(max(dot(T, T), dot(B, B)));
		return mat3(T * invmax, B * invmax, N);
	}

	vec3 PerturbNormal(in vec3 normalMap, in vec3 N, in vec3 V, in vec2 texcoord) {
		// assume N, the interpolated vertex normal and
		// V, the view vector (vertex to eye)
		vec3 map = normalMap * 255. / 127. - 128. / 127.;
		mat3 TBN = CotangentFrame(N, -V, texcoord);
		return normalize(TBN * map);
	}
	//-- end normal map code ------------------------------------ //
	#endif

	

    void pointLight( in lightData light, in vec3 normal, in vec3 ecPosition3, in float shadow, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular ){
        float nDotVP;       // normal . light direction
        float nDotHV;       // normal . light half vector
        float pf;           // power factor
        float attenuation;  // computed attenuation factor
        float d;            // distance from surface to light source
        vec3  VP;           // direction from surface to light position
        vec3  halfVector;   // direction of maximum highlights
        vec3 eye = -normalize(v_eyePosition);

        // Compute vector from surface to light position
        VP = vec3 (light.position.xyz) - ecPosition3;

        // Compute distance between surface and light position
        d = length(VP);


        // Compute attenuation
        attenuation = 1.0 / (light.constantAttenuation + light.linearAttenuation * d + light.quadraticAttenuation * d * d);

        // Normalize the vector from surface to light position
        VP = normalize(VP);
        halfVector = normalize(VP + eye);
        nDotHV = max(0.0, dot(normal, halfVector));
        nDotVP = max(0.0, dot(normal, VP));

        ambient += light.ambient.rgb * attenuation;
        diffuse += light.diffuse.rgb * shadow * nDotVP * attenuation;
#ifndef TARGET_OPENGLES
#define SPECULAR_REFLECTION
#endif
#ifndef SPECULAR_REFLECTION
        // ha! no branching :)
        pf = mix(0.0, pow(nDotHV, mat_shininess), step(0.0000001, nDotVP));
        specular += light.specular.rgb * shadow * pf * nDotVP * attenuation;
#else
        // fresnel factor
        // http://en.wikibooks.org/wiki/GLSL_Programming/Unity/Specular_Highlights_at_Silhouettes
        float w = pow(1.0 - max(0.0, dot(halfVector, VP)), 5.0);
        vec3 specularReflection = attenuation * vec3(light.specular.rgb)
          * mix(vec3(mat_specular.rgb), vec3(1.0), w)
          * pow(nDotHV, mat_shininess);
        specular += shadow * mix(vec3(0.0), specularReflection, step(0.0000001, nDotVP));
#endif
    }

	void pointLight( in lightData light, in vec3 normal, in vec3 ecPosition3, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular ){
		float tempShadow = 1.0;
		pointLight( light, normal, ecPosition3, tempShadow, ambient, diffuse, specular );
	}

    void directionalLight(in lightData light, in vec3 normal, in float shadow, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular){
        float nDotVP;         // normal . light direction
        float nDotHV;         // normal . light half vector
        float pf;             // power factor

        nDotVP = max(0.0, dot(normal, normalize(vec3(light.position))));
        nDotHV = max(0.0, dot(normal, light.halfVector));

        pf = mix(0.0, pow(nDotHV, mat_shininess), step(0.0000001, nDotVP));

        ambient += light.ambient.rgb;
        diffuse += light.diffuse.rgb * nDotVP * shadow;
        specular += light.specular.rgb * pf * nDotVP * shadow;
    }

	void directionalLight(in lightData light, in vec3 normal, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular){
		directionalLight(light, normal, 1.0, ambient, diffuse, specular);
	}

    void spotLight(in lightData light, in vec3 normal, in vec3 ecPosition3, in float shadow, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular){
        float nDotVP; // = max(dot(normal,normalize(vec3(light.position))),0.0);
        float nDotHV;       // normal . light half vector
        float pf;
        float d;            // distance from surface to light source
        vec3  VP;           // direction from surface to light position
        vec3 eye = -normalize(v_eyePosition);
        float spotEffect;
        float attenuation=1.0;
        vec3  halfVector;   // direction of maximum highlights
        // Compute vector from surface to light position
        VP = light.position.xyz - ecPosition3;
        spotEffect = dot(light.spotDirection, -normalize(VP));

        if (spotEffect > light.spotCosCutoff) {
            // Compute distance between surface and light position
            d = length(VP);
            spotEffect = pow(spotEffect, light.spotExponent);
            attenuation = spotEffect / (light.constantAttenuation + light.linearAttenuation * d + light.quadraticAttenuation * d * d);

            VP = normalize(VP);
            halfVector = normalize(VP + eye);
            nDotHV = max(0.0, dot(normal, halfVector));
            nDotVP = max(0.0, dot(normal, VP));

            pf = mix(0.0, pow(nDotHV, mat_shininess), step(0.0000001, nDotVP));

            diffuse += light.diffuse.rgb * shadow * nDotVP * attenuation;
            specular += light.specular.rgb * shadow * pf * nDotVP * attenuation;

        }

        ambient += light.ambient.rgb * attenuation;

    }

	void spotLight(in lightData light, in vec3 normal, in vec3 ecPosition3, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular) {
		spotLight(light, normal, ecPosition3, 1.0, ambient, diffuse, specular);
	}


    vec3 projectOnPlane(in vec3 point, in vec3 planeCenter, in vec3 planeNormal){
        return point - dot( point - planeCenter, planeNormal ) * planeNormal;
    }

    vec3 linePlaneIntersect(in vec3 lp, in vec3 lv, in vec3 pc, in vec3 pn){
       return lp+lv*(dot(pn,pc-lp)/dot(pn,lv));
    }

    void areaLight(in lightData light, in vec3 N, in vec3 V, float shadow, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular){
        vec3 right = light.right;
        vec3 pnormal = light.spotDirection;
        vec3 up = light.up;

        //width and height of the area light:
        float width = light.width*0.5;
        float height = light.height*0.5;
        float attenuation;

        //project onto plane and calculate direction from center to the projection.
        vec3 projection = projectOnPlane(V,light.position.xyz,pnormal);// projection in plane
        vec3 dir = projection-light.position.xyz;

        //calculate distance from area:
        vec2 diagonal = vec2(dot(dir,right),dot(dir,up));
        vec2 nearest2D = vec2(clamp( diagonal.x,-width,width  ),clamp(  diagonal.y,-height,height));
        vec3 nearestPointInside = vec3(light.position.xyz)+(right*nearest2D.x+up*nearest2D.y);
        float dist = distance(V,nearestPointInside);//real distance to area rectangle

        vec3 lightDir = normalize(nearestPointInside - V);
        attenuation = 1.0 / (light.constantAttenuation + light.linearAttenuation * dist + light.quadraticAttenuation * dist * dist);

        float NdotL = max( dot( pnormal, -lightDir ), 0.0 );
        float NdotL2 = max( dot( N, lightDir ), 0.0 );
        if ( NdotL * NdotL2 > 0.0 ) {
            float diffuseFactor = sqrt( NdotL * NdotL2 );
            vec3 R = reflect( normalize( -V ), N );
            vec3 E = linePlaneIntersect( V, R, light.position.xyz, pnormal );
            float specAngle = dot( R, pnormal );
            if (specAngle > 0.0){
                vec3 dirSpec = E - light.position.xyz;
                vec2 dirSpec2D = vec2(dot(dirSpec,right),dot(dirSpec,up));
                vec2 nearestSpec2D = vec2(clamp( dirSpec2D.x,-width,width  ),clamp(  dirSpec2D.y,-height,height));
                float specFactor = 1.0-clamp(length(nearestSpec2D-dirSpec2D) * 0.05 * mat_shininess,0.0,1.0);
                specular += light.specular.rgb * specFactor * specAngle * diffuseFactor * attenuation * shadow;
            }
            diffuse  += light.diffuse.rgb * shadow * diffuseFactor * attenuation;
        }
        ambient += light.ambient.rgb * attenuation;
    }

	void areaLight(in lightData light, in vec3 N, in vec3 V, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular) {
		areaLight(light, N, V, 1.0, ambient, diffuse, specular);
	}

#ifdef HAS_SHADOWS
float SpotShadow(in lightData light, in vec3 ecPosition3, in shadowData aShadowData, in vec3 aWorldFragPos, in vec3 aWorldNormal) {
	
	vec3 VP = light.position.xyz - ecPosition3;
	float spotEffect = dot(light.spotDirection, -normalize(VP));
	
	if (spotEffect < light.spotCosCutoff) {
		return 0.0;
	}
	
	float shadowStrength = 1.0;
	if( light.spotExponent > 0.0 ) {
		float d = length(VP);
		spotEffect = pow(spotEffect, light.spotExponent);
		float attenuation = spotEffect / (light.constantAttenuation + light.linearAttenuation * d + light.quadraticAttenuation * d * d);
		shadowStrength = attenuation;
	}
	
	return shadowStrength * SpotShadow( aShadowData, aWorldFragPos, aWorldNormal);
}
#endif


    %postFragment%

    //////////////////////////////////////////////////////
    // here's the main method
    //////////////////////////////////////////////////////


    void main (void){

        vec3 ambient = global_ambient.rgb;
        vec3 diffuse = vec3(0.0,0.0,0.0);
        vec3 specular = vec3(0.0,0.0,0.0);
        
        vec3 tNormal = v_transformedNormal;
        #ifdef HAS_TEX_NORMAL
        	tNormal = PerturbNormal(TEXTURE(tex_normal, v_texcoord).rgb, N, V, v_texcoord);
        #endif
			
        vec3 transformedNormal = normalize(tNormal);
		
		#if defined(HAS_SHADOWS)
		vec3 worldNormalN = normalize(v_worldNormal);
		#endif
        for( int i = 0; i < MAX_LIGHTS; i++ ){
            if(lights[i].enabled<0.5) continue;
			float shadow = 0.0;
            if(lights[i].type<0.5){
				#ifdef HAS_SHADOWS
				if( shadows[i].enabled > 0.5 ) {
					shadow = PointLightShadow(shadows[i], v_worldPosition, worldNormalN);
					shadow *= shadows[i].strength;
				}
				#endif
                pointLight(lights[i], transformedNormal, v_eyePosition, 1.0-shadow, ambient, diffuse, specular);
            }else if(lights[i].type<1.5){
				#ifdef HAS_SHADOWS
				if( shadows[i].enabled > 0.5 ) {
					shadow = DirectionalShadow(shadows[i], v_worldPosition, worldNormalN);
					shadow *= shadows[i].strength;
				}
				#endif
                directionalLight(lights[i], transformedNormal, 1.0-shadow, ambient, diffuse, specular);
            }else if(lights[i].type<2.5){
				#ifdef HAS_SHADOWS
				if( shadows[i].enabled > 0.5 ) {
					shadow = SpotShadow(lights[i], v_eyePosition, shadows[i], v_worldPosition, worldNormalN);
					shadow *= shadows[i].strength;
				}
				#endif
                spotLight(lights[i], transformedNormal, v_eyePosition, 1.0-shadow, ambient, diffuse, specular);
            }else{
				#ifdef HAS_SHADOWS
				if( shadows[i].enabled > 0.5 ) {
					shadow = AreaShadow( shadows[i], v_worldPosition, worldNormalN, vec2(lights[i].width, lights[i].height) );
					shadow *= shadows[i].strength;
				}
				#endif
                areaLight(lights[i], transformedNormal, v_eyePosition, 1.0-shadow, ambient, diffuse, specular);
            }
        }
        
        // apply emmisive texture
        vec4 mat_emissive_color = mat_emissive;
        #ifdef HAS_TEX_EMISSIVE
        	mat_emissive_color *= TEXTURE(tex_emissive, v_texcoord);
        #endif
		
        // apply specular texture // these are mostly black and white
        #ifdef HAS_TEX_SPECULAR
        	vec4 spec_value = TEXTURE(tex_specular, v_texcoord);
        	specular *= spec_value.rgb; //apply the color
        	specular *= spec_value.a; //also apply alpha which is sometimes used as a mask
        #endif

        // apply ambient texture // these are mostly black and white
        #ifdef HAS_TEX_AMBIENT
        	vec4 ambient_value = TEXTURE(tex_ambient, v_texcoord);
        	ambient *= ambient_value.rgb; //apply the color
        #endif

        ////////////////////////////////////////////////////////////
        // now add the material info
        #if HAS_TEXTURE && !HAS_COLOR
            vec4 tex = TEXTURE(tex0, v_texcoord);
		vec4 localColor = vec4(ambient,1.0) * tex + vec4(diffuse,1.0) * tex + vec4(specular,1.0) + mat_emissive_color;
			localColor.a = tex.a; // allow for alpha in the texture
        #elif HAS_TEXTURE && HAS_COLOR
            vec4 tex = TEXTURE(tex0, v_texcoord);
            vec4 localColor = vec4(ambient,1.0) * tex * v_color + vec4(diffuse,1.0) * tex * v_color + vec4(specular,1.0) * mat_specular + mat_emissive_color;
			localColor.a = tex.a*v_color.a; // allow for alpha in the texture or vertex
        #elif HAS_COLOR
            vec4 localColor = vec4(ambient,1.0) * v_color + vec4(diffuse,1.0) * v_color + vec4(specular,1.0) * mat_specular + mat_emissive_color;
        #else
			vec4 localColor = vec4(ambient,1.0) * mat_ambient + vec4(diffuse,1.0) * mat_diffuse + vec4(specular,1.0) * mat_specular + mat_emissive_color;
        #endif
		
		
		

                
        #ifdef HAS_TEX_OCCLUSION
            float occlusioon = TEXTURE(tex_occlusion, v_texcoord).r;
            localColor.rgb *= occlusioon;
        #endif
		        
        FRAG_COLOR = clamp( postFragment(localColor), 0.0, 1.0 );
    }
)";
