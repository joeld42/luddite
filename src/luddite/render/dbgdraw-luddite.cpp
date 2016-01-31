
#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#else
#include <GL/gl.h>
#endif


#include <GLKit/GLKMath.h>

#include <luddite/render/scene.h>
#include <luddite/render/render_device_gl.h>
#include <luddite/render/material_db.h>
#include <luddite/render/gbuff_prim.h>

#define DEBUG_DRAW_IMPLEMENTATION
#include "dbgdraw-luddite.h"



DDRenderInterfaceLuddite::DDRenderInterfaceLuddite( luddite::RenderDevice *device,
                                                    luddite::MaterialDB *mtlDB,
                                                    luddite::SceneNode *worldRoot ) :
                        mtlDB_( mtlDB )
{
    linesNode_ = new luddite::SceneNode();
    
    luddite::GBuff *gbuffLines = luddite::gbuff_dynamic( DEBUG_DRAW_VERTEX_BUFFER_SIZE * 10 );
    gbuff_setColorConstant( gbuffLines, GLKVector4Make( 1.0, 0.0, 1.0, 1.0) );
    
    linesGBuff_ = gbuffLines;
    
    luddite::Material *mtlLines = mtlDB_->getNamedMaterial( device, "mtl.dbg-linepoint" );
    luddite::GBatch *lineBatch = new luddite::GBatch();
    lineBatch->m_gbuff = gbuffLines;
    lineBatch->m_mtl = mtlLines;
    lineBatch->m_flags = GBatchFlag_LINES;
    linesNode_->addGBatch( lineBatch );
    
    worldRoot->addChild( linesNode_ );

}

void DDRenderInterfaceLuddite::beginDraw()
{
    linesGBuff_->m_dynamicSize = 0;
}

void DDRenderInterfaceLuddite::drawPointList(const dd::DrawVertex * points, const int count,
                                             const bool depthEnabled)
{
//    printf("TODO: drawPointList: %d count\n", count );
}

void DDRenderInterfaceLuddite::drawLineList(const dd::DrawVertex * lines, const int count, const bool depthEnabled)
{    

    // copy count verts from dd::DrawVertex into our format
    const dd::DrawVertex *curr = lines;
    for (int i=0; i < count; i++) {
        size_t ndx = linesGBuff_->m_dynamicSize + i;
        linesGBuff_->m_vertData[ndx].m_pos = GLKVector3Make( curr->line.x, curr->line.y, curr->line.z );
        linesGBuff_->m_vertData[ndx].m_color[0] = curr->line.r * 0xFF;
        linesGBuff_->m_vertData[ndx].m_color[1] = curr->line.g * 0xFF;
        linesGBuff_->m_vertData[ndx].m_color[2] = curr->line.b * 0xFF;
        linesGBuff_->m_vertData[ndx].m_color[3] = 0xFF;
        
//        printf("%lu : %3.2f %3.2f %3.2f\n", ndx, curr->line.x, curr->line.y, curr->line.z );
        
        curr++;
    }
    linesGBuff_->m_dynamicSize += count;
//    printf("drawLinesList: %d count %d dynSize\n", count, linesGBuff_->m_dynamicSize );
}

void DDRenderInterfaceLuddite::drawGlyphList(const dd::DrawVertex * glyphs, const int count,
                   dd::GlyphTextureHandle glyphTex)
{
//    printf("TODO: drawGlyphList: %d count\n", count );
}


dd::GlyphTextureHandle DDRenderInterfaceLuddite::createGlyphTexture(const int width, const int height,
                                                                    const void * pixels)
{
    return NULL;
}


void DDRenderInterfaceLuddite::destroyGlyphTexture(dd::GlyphTextureHandle glyphTex)
{
}

GLuint DDRenderInterfaceLuddite::handleToGL(dd::GlyphTextureHandle handle)
{
    const std::size_t temp = reinterpret_cast<std::size_t>(handle);
    return static_cast<GLuint>(temp);
}

dd::GlyphTextureHandle DDRenderInterfaceLuddite::GLToHandle(const GLuint id)
{
    const std::size_t temp = static_cast<std::size_t>(id);
    return reinterpret_cast<dd::GlyphTextureHandle>(temp);
}

