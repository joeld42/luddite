
//
//  PScenic
//  luddite sandbox demo/example
//
//  Created by joeld on 8/13/2012.
//  Copyright Tapnik 2012. All rights reserved.
//

-- FlatColor.Vertex ------------------------------------------

// A solid color, probably mostly useful for debugging or

// uniform params
uniform mat4 matrixPMV;
uniform mat3 normalMatrix;

// per-vertex
attribute highp vec4 position;

void main()
{
    vec4 posw = vec4( position.x, position.y, position.z, 1.0 );
    gl_Position = matrixPMV * posw;
}

-- FlatColor.Fragment ------------------------------------------

uniform mediump vec4 flatColor;

void main()
{
    gl_FragColor = flatColor;
}

-- GlossyNrm.Vertex ------------------------------------------

// uniform params
uniform mat4 matrixPMV;
uniform mat3 normalMatrix;

uniform vec3 lightDir0;
uniform vec4 lightColor0;

uniform vec3 Kd; // Diffuse color
uniform float roughness;

// per-vertex data
attribute highp vec4 position;
attribute highp vec4 color;
attribute mediump vec2 texcoord;

attribute mediump vec3 normal;
attribute mediump vec3 tangent;
attribute mediump vec3 bitangent;

// outputs to fragment shader
varying vec3 diffuseColor;
varying vec3 specColor;
varying mediump vec2 st;
varying highp mat3 TBN;
varying highp vec3 L0;
varying highp vec3 P;

//varying highp vec3 V;

void main()
{
    vec4 posw = vec4( position.x, position.y, position.z, 1.0 );
    gl_Position = matrixPMV * posw;
    P = gl_Position.xyz;

    vec3 n2  = normalMatrix * normal;
    vec3 t2  = normalMatrix * tangent;
    vec3 bt2 = normalMatrix * bitangent;
    TBN = mat3( t2, bt2, n2);
//    TBN = mat3( tangent, bitangent, normal );

    st = texcoord.st * 4.0;
    diffuseColor = vec3(1.0,1.0,1.0);
    specColor = vec3(1.0,1.0,1.0);

    L0 = lightDir0;
    diffuseColor = lightColor0.rgb * 1.5;
    
//    TBN = mat3( tangent, bitangent, normal );
    
    /*
	vec3 N = normal;
	vec3 N2 = normalMatrix * normal;
	
    L0 = lightDir0;
			
	float df0 = max( 0.0, dot(N, L0 ));
	
	// fresnel fake backlight
//	float sf = max( 0.0, dot(E, N2 ));
//	sf = (1.0 - pow( sf, 0.01 )) * 20.0;	
		
	st = texcoord.st;
	
	vec4 posw = vec4( position.x, position.y, position.z, 1.0 );
    gl_Position = matrixPMV * posw;
    P = gl_Position.xyz;

    
//    // Eye vector
//    H = normalize( L0 + Eo );

//    diffuseColor.rgb = (lightColor0 * df0) + (lightColor1 * df1);
    
    // Ignore light color for now
//    diffuseColor.rgb = color.rgb * df0;
  
//    diffuseColor = lightColor0.rgb * 1.5;
//    diffuseColor = lightColor0.rgb * df0;
    diffuseColor = N2;

    //diffuseColor = N2.rgb;
    
    vec3 t2 = normalMatrix * tangent;
    vec3 bt2 = normalMatrix * bitangent;
    TBN = mat3( bt2, t2, N2 );
//    TBN = mat3( tangent, bitangent, normal );
	
	specColor.rgb = vec3(1.0, 1.0, 1.0);
    */
}


-- GlossyNrm.Fragment ------------------------------------------

uniform mediump vec4 dbgColor;
uniform highp vec3 eyePos;

varying lowp vec3 diffuseColor;
varying lowp vec3 specColor;
varying mediump vec2 st;
varying highp mat3 TBN;
varying highp vec3 L0;
varying highp vec3 P;

uniform sampler2D sampler_dif0;
uniform sampler2D sampler_nrm0;
uniform sampler2D sampler_spec0;

void main()
{
	lowp vec4 dif0 = texture2D( sampler_dif0, st );
    lowp vec4 spec0 = texture2D( sampler_spec0, st );
	gl_FragColor.rgb = (diffuseColor * dif0.rgb )+specColor;
	gl_FragColor.a = dif0.a;
    
    highp vec4 nrm1 = texture2D( sampler_nrm0, st );
    highp vec4 nrm2 = (nrm1 * 2.0) - 1.0;
//    highp vec4 nrm2 = vec4( 0.0, 0.0, 1.0, 0.0 );
    highp vec3 nrm3 = TBN * nrm2.xyz;

    highp float df0 = max( 0.1, dot(nrm3, L0 ));
    
//    highp vec3 eyeDir = normalize(P - eyePos);
//    highp vec3 LL = vec3( 1.0, 0.0, 0.0 );
    highp vec3 H = normalize( L0 + vec3(0.0, 0.0, 1.0) );
    
    highp float nh = dot( nrm3, H );
    highp float spec = pow( clamp( nh, 0.0, 1.0 ), 5.0 );
    

//    gl_FragColor.rgb = df0 * diffuseColor * dif0.rgb;
//    gl_FragColor.rgb = (specColor * spec * spec0.r);// + (df0 * diffuseColor * dif0.rgb);
//    gl_FragColor.rgb = specColor * spec;
    
    gl_FragColor.rgb = (2.0*spec0.r*spec) * specColor + (df0 * diffuseColor * dif0.rgb);
    
//    if (P.y > 2.0)
//    {
//         //gl_FragColor.rgb = nrm3;
//        gl_FragColor.rgb = TBN[0].xyz;
//    }
//    else if (P.y > -2.0)
//    {
//        gl_FragColor.rgb = TBN[1].xyz;
//    }
//    else
//    {
//        gl_FragColor.rgb = TBN[2].xyz;
//    }
//    gl_FragColor.a = 1.0;

}
