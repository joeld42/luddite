
//
//  CollectGame.glsl
//  luddite collect game demo/example
//
//  Created by joeld on 8/13/2012.
//  Copyright Tapnik 2012. All rights reserved.
//

-- BasicMtl.Vertex ------------------------------------------

// uniform params
uniform mat4 matrixPMV;
uniform mat3 normalMatrix;

uniform vec3 lightPos0;
uniform vec3 lightColor0;

uniform vec3 Kd; // Diffuse color
uniform float roughness;

// per-vertex data
attribute highp vec4 position;
attribute highp vec4 color;
attribute mediump vec2 texcoord;
attribute mediump vec3 normal;

// outputs to fragment shader
varying vec3 diffuseColor;
varying vec3 specColor;
varying mediump vec2 st;

void main()
{
	vec3 N = normal;
	vec3 N2 = normalMatrix * normal;
	
	vec3 L0 = lightPos0;
		
    // Eye vector
	vec3 E = vec3( 0, 0, -1 );
	//vec3 H = normalize( L0 + E );
	
	float df0 = max( 0.0, dot(N, L0 ));
	
	// fresnel fake backlight
	float sf = max( 0.0, dot(E, N2 ));
	sf = (1.0 - pow( sf, 0.01 )) * 20.0;	
		
	st = texcoord.st;
	
	vec4 posw = vec4( position.x, position.y, position.z, 1.0 );
	gl_Position = matrixPMV * posw;

//    diffuseColor.rgb = (lightColor0 * df0) + (lightColor1 * df1);
    
    // Ignore light color for now
    diffuseColor.rgb = color.rgb * df0;
	
	specColor.rgb = vec3(sf);
}


-- BasicMtl.Fragment ------------------------------------------

uniform mediump vec4 dbgColor;

varying lowp vec3 diffuseColor;
varying lowp vec3 specColor;
varying mediump vec2 st;

uniform sampler2D sampler_dif0;

void main()
{
	lowp vec4 dif0 = texture2D( sampler_dif0, st );
	gl_FragColor.rgb = (diffuseColor * dif0.rgb )+specColor;
	gl_FragColor.a = dif0.a;
    
    //gl_FragColor = (texture2D( sampler_dif0, st ) + vec4( dbgColor.x, dbgColor.y, dbgColor.z, 1.0 )) * 0.5;
    gl_FragColor = texture2D( sampler_dif0, st );

//    gl_FragColor = colorVarying;
//    gl_FragColor = vec4(st.x, 0.0, st.y, 1.0 );

    // DBG
//   gl_FragColor = vec4( 1.0, 0.0, 1.0, 1.0 );
//   gl_FragColor = dbgColor;
}
