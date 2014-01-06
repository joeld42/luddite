//
//  render_device_es2.cpp
//  luddite_ios
//
//  Created by Joel Davis on 7/29/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#import <glsw/glsw.h>

#include <luddite/common/debug.h>
#include <luddite/render/render_device_es2.h>
#include <luddite/render/texture_info.h>
#include <luddite/render/param.h>

// offsetof() gives a warning about non-POD types with xcode, so use these old
// school macros. This is OK because while VertType is non-POD, it doesn't have
// a vtable so these still work.
#define offset_d(i,f)    (long(&(i)->f) - long(i))
#define offset_s(t,f)    offset_d((t*)1000, f)

using namespace luddite;


void RenderDeviceES2::_param(Param const & p)
{
    switch (p.m_paramType)
    {
        case ParamType_SCALAR:
            glUniform1f( p.m_glParam, p.m_val.scalar );
            break;

        case ParamType_VEC2:
            glUniform2fv( p.m_glParam, 1, p.m_val.data );
            break;

        case ParamType_VEC3:
            glUniform3fv( p.m_glParam, 1, p.m_val.data );
            break;

        case ParamType_VEC4:
            glUniform4fv( p.m_glParam, 1, p.m_val.data );
            break;

        case ParamType_MATRIX3:
            glUniformMatrix3fv( p.m_glParam, 1, GL_FALSE, p.m_val.data );
            break;

        case ParamType_MATRIX4:
            glUniformMatrix4fv( p.m_glParam, 1, GL_FALSE, p.m_val.data );
            break;
    }
}


void RenderDeviceES2::_drawGBatch( luddite::GBatch *gbatch )
{
    luddite::GBuff *gbuff = gbatch->m_gbuff;    
    matrix4x4f mresult =  gbatch->m_xform * matBaseModelView;
    mresult = mresult * matProjection;
    
//    matrix4x4f &nodeXform = gbatch->m_xform;
//    DBG::info( "nodeXForm      %3.2f %3.2f %3.2f %3.2f\n"
//               "              %3.2f %3.2f %3.2f %3.2f\n"
//               "              %3.2f %3.2f %3.2f %3.2f\n"
//               "              %3.2f %3.2f %3.2f %3.2f\n",                  
//              
//              nodeXform.m16[0],
//              nodeXform.m16[1],
//              nodeXform.m16[2],
//              nodeXform.m16[3],
//              
//              nodeXform.m16[4],
//              nodeXform.m16[5],
//              nodeXform.m16[6],
//              nodeXform.m16[7],
//              
//              nodeXform.m16[8],
//              nodeXform.m16[9],
//              nodeXform.m16[10],
//              nodeXform.m16[11],
//              
//              nodeXform.m16[12],
//              nodeXform.m16[13],
//              nodeXform.m16[14],
//              nodeXform.m16[15] );

    
    // Set transform and shader params from gbatch
    
    if (!gbatch->m_mtl)
    {
        // Use default material    
        glUniformMatrix4fv(uparam_modelViewProjection, 1, 0, mresult.m16 );
//    glUniformMatrix3fv(uparam_normalMat, 1, 0, _normalMatrix.m);
    }
    else
    {
        // use gbatch material
        glUseProgram(gbatch->m_mtl->m_shader->shaderProgram() );
        
        GLint mvp = glGetUniformLocation( gbatch->m_mtl->m_shader->shaderProgram(), "matrixPMV");
        glUniformMatrix4fv( mvp, 1, 0, mresult.m16 );

        // Set params from mtl
        for (Param &p : gbatch->m_mtl->mutable_params() )
        {
            // Does param need to be initialized?
            if (p.m_glParam==PARAM_UNINITIALIZED)
            {
                // find it
                p.m_glParam = glGetUniformLocation( gbatch->m_mtl->m_shader->shaderProgram(),
                                                     p.m_name.c_str() );
                printf("Got param %s (loc: %d)\n", p.m_name.c_str(), p.m_glParam );
            }

            _param( p );
        }
    }

    // Create gbo for this gbuff if not set up
    if (gbuff->m_vbo==0)
    {
        glGenBuffers( 1, &(gbuff->m_vbo) );
        
        glBindBuffer( GL_ARRAY_BUFFER, gbuff->m_vbo );
        glBufferData( GL_ARRAY_BUFFER, gbuff->m_vertData.size() * sizeof( DrawVert ), 
                     gbuff->m_vertData.data(), GL_STATIC_DRAW );
        
    }
    else
    {
        // Buffer is already created, just bind it
        glBindBuffer( GL_ARRAY_BUFFER, gbuff->m_vbo );
    }

    // Bind textures
    // TODO: don't rebind if batches share the same texture
    // TODO: support more than 1 texture
    glBindTexture(GL_TEXTURE_2D, gbatch->m_mtl->m_tex[0]->m_texId );

    // Bind buffer data
    glEnableVertexAttribArray( VertexAttrib_TEXCOORD );
    glVertexAttribPointer( VertexAttrib_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 
                          sizeof(DrawVert), (void*)offset_s( DrawVert, m_st ) );
    
    glEnableVertexAttribArray( VertexAttrib_POSITION );
    glVertexAttribPointer( VertexAttrib_POSITION, 3, GL_FLOAT, GL_FALSE, 
                          sizeof(DrawVert), (void*)offset_s( DrawVert, m_pos) );


    glEnableVertexAttribArray( VertexAttrib_NORMAL );
    glVertexAttribPointer( VertexAttrib_NORMAL, 3, GL_FLOAT, GL_FALSE, 
                          sizeof(DrawVert), (void*)offset_s( DrawVert, m_nrm) );
    
    glEnableVertexAttribArray( VertexAttrib_COLOR );
    glVertexAttribPointer( VertexAttrib_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, 
                          sizeof(DrawVert), (void*)offset_s( DrawVert, m_color) );
    
    // Draw it!
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)gbuff->m_vertData.size() );

}

int32_t RenderDeviceES2::loadShader( const std::string &shaderKey )
{
    GLint program;
	printf("loadShader %s ...", shaderKey.c_str() );
	
    // Create shader program
    program = glCreateProgram();
    
    // vertex and fragment shaders
    GLuint vertShader, fragShader;    
    
    // Get the vertex shader part
    const char *vertShaderText = glswGetShader( (shaderKey+".Vertex").c_str() );
    if (!vertShaderText)
    {
        printf("Couldn't find shader key: %s.Vertex\n", shaderKey.c_str() );
		return SHADER_FAIL;
    }
    
    // Compile the vertex shader
    vertShader = _compileShader( vertShaderText, GL_VERTEX_SHADER );
    
    // Get fragment shader
    const char *fragShaderText = glswGetShader( (shaderKey+".Fragment").c_str() );
    if (!fragShaderText)
    {
        printf("Couldn't find shader key: %s.Fragment\n", shaderKey.c_str() );
		return SHADER_FAIL;
    }
    
    printf( "compile fragment shader" );
    
    // Compile the fragment shader
    fragShader = _compileShader( fragShaderText, GL_FRAGMENT_SHADER );
    
    // Attach shaders
    glAttachShader( program, vertShader );
    glAttachShader( program, fragShader );
    
    printf("... bind attrs\n" );
	
	// Bind Attrs (todo put in subclass)
	// FIXME: some shaders dont have all these attrs..
	glBindAttribLocation( program, VertexAttrib_POSITION, "position" );
	glBindAttribLocation( program, VertexAttrib_TEXCOORD, "texcoord" );
	glBindAttribLocation( program, VertexAttrib_NORMAL,   "normal" );    
	glBindAttribLocation( program, VertexAttrib_COLOR,   "color" );
	
    
    //  Link Shader
    printf("... links shaders\n" );
    _link( program );
    
    // Release vert and frag shaders
    glDeleteShader( vertShader );
    glDeleteShader( fragShader );    
    
	
	// print shader params
	printf(" ----- %s ------\n", shaderKey.c_str() );
	int activeUniforms;
	glGetProgramiv( program, GL_ACTIVE_UNIFORMS, &activeUniforms );	
	printf(" Active Uniforms: %d\n", activeUniforms );
	
    // done
    return program;    
}

int32_t RenderDeviceES2::_compileShader( const char *shaderText, 
                                         uint32_t shaderType )
{
    GLint status;    
    GLuint shader;
    shader = glCreateShader( shaderType );
    
    // compile the shader
    glShaderSource( shader, 1, &shaderText, NULL );
    glCompileShader( shader );
    
    // Check for errors
    GLint logLength;
    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLength );
    if (logLength > 0)
    {
        char *log = (char *)malloc(logLength);
        glGetShaderInfoLog( shader, logLength, &logLength, log );
        
        printf("Error compiling shader:\n%s\n", log );
        free(log);        
    }
    
    glGetShaderiv( shader,GL_COMPILE_STATUS, &status );
    if (status==0)
    {
        glDeleteShader( shader );
        
        // TODO: better handle errors
        printf("Compile status is bad\n" );
        
        return 0;        
    }
    
    return shader;    
}

void RenderDeviceES2::_printShaderLog( int32_t program )
{
	GLint logLength;
    glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength );
    if (logLength > 0 )
    {
        char *log = (char*)malloc(logLength);
        glGetProgramInfoLog( program, logLength, &logLength, log );
        printf ("Link Log:\n%s\n", log );
        free(log);        
    }
	
}

void RenderDeviceES2::_link( int32_t program )
{
    GLint status;
    
    glLinkProgram( program );
	
	_printShaderLog( program );
	
    glGetProgramiv( program, GL_LINK_STATUS, &status);
    if (status==0)
    {
        printf("ERROR Linking shader\n");        
    }    
}


