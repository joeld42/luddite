//
//  render_device_es2.cpp
//  luddite_ios
//
//  Created by Joel Davis on 7/29/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#else
#include <GL/gl.h>
#endif

#import <glsw.h>

#include <GLKit/GLKMath.h>

#include <luddite/common/debug.h>
#include <luddite/render/render_device_gl.h>
#include <luddite/render/texture_info.h>
#include <luddite/render/param.h>
#include <luddite/render/particle_group.h>

// offsetof() gives a warning about non-POD types with xcode, so use these old
// school macros. This is OK because while VertType is non-POD, it doesn't have
// a vtable so these still work.
#define offset_d(i,f)    (long(&(i)->f) - long(i))
#define offset_s(t,f)    offset_d((t*)1000, f)

using namespace luddite;


//void RenderDeviceGL::_param(Param const & p)
void RenderDeviceGL::_param( const Param &p, const GLKMatrix4 &mresult )
{
    bool doDebug = false;
    if (p.m_name=="lightDir0")
    {
        doDebug = true;
    }
    
    switch (p.m_paramType)
    {
        case ParamType_SCALAR:
            glUniform1f( p.m_glParam, p.m_val.scalar );
            break;

        case ParamType_VEC2:
            glUniform2fv( p.m_glParam, 1, p.m_val.data );
            break;

        case ParamType_VEC3:

//            if (doDebug)
//            {
//                printf("Bind: %s %f %f %f (%s)\n", p.m_name.c_str(),
//                       p.m_val.data[0],p.m_val.data[1],
//                       p.m_val.data[2],
//                       p.m_space==ParamSpace_OBJECT?"object":"world"
//                       );
//            }

            
            if (p.m_space == ParamSpace_WORLD)
            {
                glUniform3fv( p.m_glParam, 1, p.m_val.data );
            }
            else if (p.m_space == ParamSpace_OBJECT)
            {
                // FIXME: do this at a higher level only once
                GLKVector3 pworld = GLKVector3MakeWithArray( p.m_val.data );
//                GLKMatrix4 xformInv = GLKMatrix4Transpose( mresult );
//                GLKVector3 plocal = GLKMatrix4MultiplyVector3( mresult, pworld );
//                printf("Light dir (local) %f %f %f\n", plocal.x, plocal.y, plocal.z );
                glUniform3fv( p.m_glParam, 1, pworld.v );
            }
            break;

        case ParamType_VEC4:
//            if (doDebug)
//            {
//                printf("Bind: %s %f %f %f %f\n", p.m_name.c_str(),
//                       p.m_val.data[0],p.m_val.data[1],
//                       p.m_val.data[2],p.m_val.data[3]
//                       );
//            }

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

void RenderDeviceGL::_prepareFrame()
{
    glEnable( GL_DEPTH_TEST );
}

void RenderDeviceGL::_drawGBatch( luddite::GBatch *gbatch )
{
    luddite::GBuff *gbuff = gbatch->m_gbuff;    
    //matrix4x4f mresult =  gbatch->m_xform * matBaseModelView;
    GLKMatrix4 mresult = GLKMatrix4Multiply( matBaseModelView, gbatch->m_xform  );
    GLKMatrix4 mresultPMV = GLKMatrix4Multiply(  matProjection, mresult );

    // setup material from this gbatch
    _setupMaterial(gbatch, mresult, mresultPMV);

    // Create gbo for this gbuff if not set up
     _bindGbuffVBO(gbuff);

    // Bind textures
    _bindGBatchTextures(gbatch);

    // Bind buffer data
    _bindDrawVertAttribs();

    // Draw it!
    if (gbatch->m_flags & GBatchFlag_LINES)
    {
        if (gbuff->m_dynamic) {
            glDrawArrays(GL_LINES, 0, (GLsizei)gbuff->m_dynamicSize );
        } else {
            glDrawArrays(GL_LINES, 0, (GLsizei)gbuff->m_vertData.size() );
        }
    } else {
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)gbuff->m_vertData.size() );
    }

}

void RenderDeviceGL::_enableBlendMode( bool enabled )
{
    if (enabled)
    {
        glEnable( GL_BLEND );
    }
    else
    {
        glDisable( GL_BLEND );
    }
}

void RenderDeviceGL::_drawParticleBatch( luddite::GBatch *pbatch )
{
    luddite::GBuff *gbuff = pbatch->m_gbuff;

    //matrix4x4f mresult =  gbatch->m_xform * matBaseModelView;
    GLKMatrix4 mresult = GLKMatrix4Multiply( matBaseModelView, pbatch->m_xform  );
    GLKMatrix4 mresultPMV = GLKMatrix4Multiply(  matProjection, mresult );

    // setup material from this gbatch
    _setupMaterial(pbatch, mresult, mresultPMV);
    
    glDepthMask( GL_FALSE );

    // Create gbo for this gbuff if not set up
    _bindParticleVBO(gbuff );

    // Bind textures
    _bindGBatchTextures(pbatch);

    // Bind buffer data
    _bindDrawVertAttribs();

    // Draw it!
    glDrawArrays(GL_POINTS, 0, (GLsizei)gbuff->m_vertData.size() );

    glDisable( GL_BLEND );
    glDepthMask( GL_TRUE );
}


void RenderDeviceGL::_bindDrawVertAttribs()
{
    glEnableVertexAttribArray( VertexAttrib_TEXCOORD );
    glVertexAttribPointer( VertexAttrib_TEXCOORD, 2, GL_FLOAT, GL_FALSE,
                          sizeof(DrawVert), (void*)offset_s( DrawVert, m_st ) );

    glEnableVertexAttribArray( VertexAttrib_POSITION );
    glVertexAttribPointer( VertexAttrib_POSITION, 3, GL_FLOAT, GL_FALSE,
                          sizeof(DrawVert), (void*)offset_s( DrawVert, m_pos) );

    glEnableVertexAttribArray( VertexAttrib_NORMAL );
    glVertexAttribPointer( VertexAttrib_NORMAL, 3, GL_FLOAT, GL_FALSE,
                          sizeof(DrawVert), (void*)offset_s( DrawVert, m_nrm) );

    glEnableVertexAttribArray( VertexAttrib_TANGENT );
    glVertexAttribPointer( VertexAttrib_TANGENT, 3, GL_FLOAT, GL_FALSE,
                          sizeof(DrawVert), (void*)offset_s( DrawVert, m_tangent) );

    glEnableVertexAttribArray( VertexAttrib_BITANGENT );
    glVertexAttribPointer( VertexAttrib_BITANGENT, 3, GL_FLOAT, GL_FALSE,
                          sizeof(DrawVert), (void*)offset_s( DrawVert, m_bitangent) );
    
    glEnableVertexAttribArray( VertexAttrib_COLOR );
    glVertexAttribPointer( VertexAttrib_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE,
                          sizeof(DrawVert), (void*)offset_s( DrawVert, m_color) );
    

}

void RenderDeviceGL::_bindGBatchTextures(GBatch *gbatch)
{
    // TODO: don't rebind if batches share the same texture
    // TODO: VAO support
    for (int i=3; i >= 0; i--)
    {
        if (!gbatch->m_mtl->m_tex[i]) continue;

        GLuint texId = gbatch->m_mtl->m_tex[i]->m_texId;
        if (texId)
        {
//            printf("  Bind texture %d (%s) slot %d\n", texId, gbatch->m_mtl->m_tex[i]->m_filename.c_str(), i );
            glActiveTexture( GL_TEXTURE0+i );
            glBindTexture(GL_TEXTURE_2D, gbatch->m_mtl->m_tex[i]->m_texId );
        }
    }
}

void RenderDeviceGL::_bindGbuffVBO(GBuff *gbuff )
{
    if (gbuff->m_vbo==0)
    {
        glGenBuffers( 1, &(gbuff->m_vbo) );

        glBindBuffer( GL_ARRAY_BUFFER, gbuff->m_vbo );
        glBufferData( GL_ARRAY_BUFFER, gbuff->m_vertData.size() * sizeof( DrawVert ),
                     gbuff->m_vertData.data(),
                     gbuff->m_dynamic?GL_DYNAMIC_DRAW:GL_STATIC_DRAW );

    }
    else
    {
        // Buffer is already created, just bind it
        glBindBuffer( GL_ARRAY_BUFFER, gbuff->m_vbo );
        
        // For dynamic gbuffs, update the data every draw
        if (gbuff->m_dynamic)
        {
            glBufferSubData( GL_ARRAY_BUFFER, 0,
                            gbuff->m_dynamicSize * sizeof(DrawVert),
                            gbuff->m_vertData.data() );
        }
    }
}

void RenderDeviceGL::_bindParticleVBO(GBuff *gbuff )
{
    if (gbuff->m_vbo==0)
    {
        glGenBuffers( 1, &(gbuff->m_vbo) );

        glBindBuffer( GL_ARRAY_BUFFER, gbuff->m_vbo );
        glBufferData( GL_ARRAY_BUFFER, gbuff->m_capacity * sizeof( DrawVert ),
               NULL, GL_DYNAMIC_DRAW );
    }
    else
    {
        // Buffer is already created, just bind it
        glBindBuffer( GL_ARRAY_BUFFER, gbuff->m_vbo );
    }

    // Copy active particles into buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0, gbuff->m_vertData.size() * sizeof( DrawVert ), gbuff->m_vertData.data() );
}


void RenderDeviceGL::_setupMaterial(GBatch *gbatch, const GLKMatrix4 &mresult, const GLKMatrix4 &mresultPMV)
{
    // Set transform and shader params from gbatch
    if (!gbatch->m_mtl)
    {
        // Use default material
        glUniformMatrix4fv(uparam_modelViewProjection, 1, 0, mresultPMV.m );
//    glUniformMatrix3fv(uparam_normalMat, 1, 0, _normalMatrix.m);
    }
    else
    {
        // set blend mode
        glEnable( GL_BLEND );
        
        if (gbatch->m_flags & GBatchFlag_BLEND)
        {
            if (gbatch->m_mtl->m_blendAdd)
            {
                glBlendFunc( GL_ONE , GL_ONE );
            }
            else
            {
                glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
            }
        }
        
        
        // use gbatch material
        GLuint progGBatch = gbatch->m_mtl->m_shader->shaderProgram();
        glUseProgram(progGBatch );

        GLint mvp = glGetUniformLocation( progGBatch, "matrixPMV");
        glUniformMatrix4fv( mvp, 1, 0, mresultPMV.m );

        GLint modelView = glGetUniformLocation( progGBatch, "matrixModelview");
        glUniformMatrix4fv( modelView, 1, 0, mresult.m );
        
        GLint mnrm = glGetUniformLocation( progGBatch, "normalMatrix");
        GLKMatrix3 nrmMat = GLKMatrix4GetMatrix3( mresult );
//        nrmMat = GLKMatrix3Transpose( nrmMat );
        
        glUniformMatrix3fv( mnrm, 1, GL_FALSE, nrmMat.m );
        

        // Set params from mtl
//        printf("mtl: %s\n", gbatch->m_mtl->m_materialName.c_str() );
        for (Param &p : gbatch->m_mtl->mutable_params() )
        {
            // Does param need to be initialized?
            if (p.m_glParam==PARAM_UNINITIALIZED)
            {
                // find it
                p.m_glParam = glGetUniformLocation( gbatch->m_mtl->m_shader->shaderProgram(),
                                                     p.m_name.c_str() );
            }

            _param( p, mresult );
        }

        // bind textures to slots
        for (int i=0; i < kMaxTextureSlot; i++)
        {
//            printf("mtl %s SLOT %d tex %p\n",
//                   gbatch->m_mtl->m_materialName.c_str(),
//                   i,
//                   gbatch->m_mtl->m_tex[i]
//                   );

            if (gbatch->m_mtl->m_tex[i] && (!gbatch->m_mtl->m_tex[i]->m_paramName.empty()))
            {
                // Do we need to find the texture param?
                if (gbatch->m_mtl->m_texParam[i] == PARAM_UNINITIALIZED)
                {
                    GLuint texp = glGetUniformLocation( gbatch->m_mtl->m_shader->shaderProgram(),
                                                        gbatch->m_mtl->m_tex[i]->m_paramName.c_str() );
//                    printf("Looking for param name %s, result %d\n", gbatch->m_mtl->m_tex[i]->m_paramName.c_str(), texp );

                    gbatch->m_mtl->m_texParam[i] = texp;
                }
                glUniform1i( gbatch->m_mtl->m_texParam[i], i );
//                printf("bind texture param %d slot %d\n", gbatch->m_mtl->m_texParam[i], i );
            }

        }
    }
}

void RenderDeviceGL::_finishFrame()
{
}


int32_t RenderDeviceGL::loadShader( const std::string &shaderKey )
{
    GLint program;
//	printf("loadShader %s ...", shaderKey.c_str() );
	
    // Create shader program
    program = glCreateProgram();
    
    // vertex and fragment shaders
    GLuint vertShader, fragShader;    
    
    // Get the vertex shader part
    const char *vertShaderText = glswGetShader( (shaderKey+".Vertex").c_str() );
    if (!vertShaderText)
    {
        printf("Couldn't find shader key: %s.Vertex (Error: %s)\n", shaderKey.c_str(), glswGetError() );
		return SHADER_FAIL;
    }
    
//    printf( "compile vertex shader:\n%s\n-----\n", vertShaderText );
    
    // Compile the vertex shader
    vertShader = _compileShader( vertShaderText, GL_VERTEX_SHADER );
    
    // Get fragment shader
    const char *fragShaderText = glswGetShader( (shaderKey+".Fragment").c_str() );
    if (!fragShaderText)
    {
        printf("Couldn't find shader key: %s.Fragment (Error: %s)\n", shaderKey.c_str(), glswGetError() );
		return SHADER_FAIL;
    }
    
//    printf( "compile fragment shader:\n%s\n-----\n", fragShaderText );
    
    // Compile the fragment shader
    fragShader = _compileShader( fragShaderText, GL_FRAGMENT_SHADER );
    
    // Attach shaders
    glAttachShader( program, vertShader );
    glAttachShader( program, fragShader );
    
//    printf("... bind attrs\n" );
	
	// Bind Attrs (todo put in subclass)
	// FIXME: some shaders dont have all these attrs..
	glBindAttribLocation( program, VertexAttrib_POSITION, "position" );
	glBindAttribLocation( program, VertexAttrib_TEXCOORD, "texcoord" );
	glBindAttribLocation( program, VertexAttrib_NORMAL,   "normal" );    
	glBindAttribLocation( program, VertexAttrib_COLOR,    "color" );
	glBindAttribLocation( program, VertexAttrib_TANGENT,  "tangent" );
    glBindAttribLocation( program, VertexAttrib_BITANGENT, "bitangent" );
    
    //  Link Shader
//    printf("... links shaders\n" );
    _link( program );
    
    // Release vert and frag shaders
    glDeleteShader( vertShader );
    glDeleteShader( fragShader );    
    
	
	// print shader params
//	printf(" ----- %s ------\n", shaderKey.c_str() );
	int activeUniforms;
	glGetProgramiv( program, GL_ACTIVE_UNIFORMS, &activeUniforms );	
//	printf(" Active Uniforms: %d\n", activeUniforms );
	
    // done
    return program;    
}

int32_t RenderDeviceGL::_compileShader( const char *shaderText, 
                                        uint32_t shaderType )
{
    GLint status;    
    GLuint shader;
    shader = glCreateShader( shaderType );
    
    // compile the shader
    glShaderSource( shader, 1, &shaderText, NULL );
    glCompileShader( shader );
    
    // Check for errors
    GLint logLength=0;
    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLength );
    if (logLength > 0)
    {
        char *log = (char *)malloc(logLength);
        glGetShaderInfoLog( shader, logLength, NULL, log );
        printf("--------------------------\n%s\n\n", shaderText );
        printf("Error compiling %s shader:\n%s\n-------\n",
               (shaderType==GL_VERTEX_SHADER)?"Vertex":"Fragment",
               log );
        free(log);        
    }
    
    glGetShaderiv( shader,GL_COMPILE_STATUS, &status );
    if (status==0)
    {
        glDeleteShader( shader );
        
        // TODO: better handle errors
//        printf("Compile status is bad\n" );
        
        return 0;        
    }
    
    return shader;    
}

void RenderDeviceGL::_printShaderLog( int32_t program )
{
	GLint logLength=0;
    glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength );
    if (logLength > 0 )
    {
        char *log = (char*)malloc(logLength);
        glGetProgramInfoLog( program, logLength, &logLength, log );
        printf ("Link Log:\n%s\n", log );
        free(log);
    }
	
}

void RenderDeviceGL::_link( int32_t program )
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


