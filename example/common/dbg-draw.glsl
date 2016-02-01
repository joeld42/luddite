//
//  dbg-draw
//  shader support for dbg-draw library
//
//  Copyright Tapnik 2016. All rights reserved.
//

-- LinePoint.Vertex -----------------------------------------

// uniform params
uniform mat4 matrixPMV;
uniform mat3 normalMatrix;

// per-vertex data
attribute highp vec4 position;
attribute highp vec4 color;
attribute mediump vec2 texcoord;

varying mediump vec4 v_Color;

void main()
{
    gl_Position  = matrixPMV * vec4(position.xyz, 1.0);
    gl_PointSize = texcoord.x;
    v_Color      = vec4(color.xyz, 1.0);
}

-- LinePoint.Fragment -----------------------------------------

varying mediump vec4 v_Color;

void main()
{
    gl_FragColor = v_Color;
//    gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0 );
}

-- Text.Vertex -----------------------------------------

// uniform params
uniform mat4 matrixPMV;
uniform mat3 normalMatrix;

// per-vertex data
attribute highp vec4 position;
attribute highp vec4 color;
attribute mediump vec2 texcoord;

//uniform highp vec2 u_screenDimensions;

varying highp vec2 v_TexCoords;
varying highp vec4 v_Color;

void main()
{
    highp vec2 u_screenDimensions = vec2( 768, 1024);
    
    // Map to normalized clip coordinates:
    highp float x = ((2.0 * (position.x - 0.5)) / u_screenDimensions.x) - 1.0;
    highp float y = 1.0 - ((2.0 * (position.y - 0.5)) / u_screenDimensions.y);

    gl_Position = vec4(x, y, 0.0, 1.0);
    v_TexCoords = texcoord;
    v_Color     = vec4(color.xyz, 1.0);
}

-- Text.Fragment -----------------------------------------
varying highp vec2 v_TexCoords;
varying highp vec4 v_Color;

uniform sampler2D sampler_dif0;

void main()
{
    mediump vec4 texColor = texture2D(sampler_dif0, v_TexCoords).rrrr;
//    gl_FragColor = vec4( texColor.rgb, 1.0 );
    gl_FragColor = v_Color * texture2D(sampler_dif0, v_TexCoords).rrrr;
//    gl_FragColor = vec4( 1.0, 1.0, 1.0, 1.0 );
}
