
#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#else
#include <GL/gl.h>
#endif


#include <GLKit/GLKMath.h>

#include <luddite/common/debug.h>
#include <luddite/render/scene.h>
#include <luddite/render/render_device_gl.h>
#include <luddite/render/material_db.h>
#include <luddite/render/gbuff_prim.h>

#define DEBUG_DRAW_IMPLEMENTATION
#include "dbgdraw-luddite.h"

#define MAX_DEBUG_POINTS (DEBUG_DRAW_VERTEX_BUFFER_SIZE * 10)

using namespace luddite;

DDRenderInterfaceLuddite::DDRenderInterfaceLuddite( luddite::RenderDevice *device,
                                                    luddite::MaterialDB *mtlDB,
                                                    luddite::SceneNode *worldRoot ) :
                        mtlDB_( mtlDB )
{
    luddite::Material *mtlLinePoint = mtlDB_->getNamedMaterial( device, "mtl.dbg-linepoint" );
    luddite::Material *mtlText = mtlDB_->getNamedMaterial( device, "mtl.dbg-text" );
    
    glyphsTex_ = mtlDB_->lookupTexture("dbgdraw-glyphs");
    
    
    debugRoot_ = new luddite::SceneNode();
    worldRoot->addChild( debugRoot_ );
    
    // Lines geom
    linesGBuff_ = makeDebugGeom( debugRoot_, mtlLinePoint, GBatchFlag_LINES );

    // Points geom
    pointsGBuff_ = makeDebugGeom( debugRoot_, mtlLinePoint, GBatchFlag_POINTS );

    // Glyphs geom
    glyphsGBuff_ = makeDebugGeom( debugRoot_, mtlText, GBatchFlag_SCREENSPACE | GBatchFlag_BLEND );
    
    m_active = true;
    
}

void DDRenderInterfaceLuddite::setScreenSize( luddite::RenderDevice *device, GLKVector2 screenSize )
{
    luddite::Material *mtlText = mtlDB_->getNamedMaterial( device, "mtl.dbg-text" );
    mtlText->param("u_screenDimensions") = screenSize;
}

luddite::GBuff *DDRenderInterfaceLuddite::makeDebugGeom( luddite::SceneNode *parent,
                                                        luddite::Material *mtl, int flags )
{
    luddite::SceneNode *debugNode = new luddite::SceneNode();
    luddite::GBuff *gbuff = luddite::gbuff_dynamic( MAX_DEBUG_POINTS );
    gbuff_setColorConstant( gbuff, GLKVector4Make( 1.0, 0.0, 1.0, 1.0) );
    
    luddite::GBatch *gbatch = new luddite::GBatch();
    gbatch->m_gbuff = gbuff;
    gbatch->m_mtl = mtl;
    gbatch->m_flags = flags;
    debugNode->addGBatch( gbatch );
    
    parent->addChild( debugNode );
    
    return gbuff;
}

void DDRenderInterfaceLuddite::setActive(bool active )
{
    m_active = active;
    debugRoot_->setVisible(active);
}

void DDRenderInterfaceLuddite::beginDraw()
{
    didWarnThisFrame_ = false;
    linesGBuff_->m_dynamicSize = 0;
    pointsGBuff_->m_dynamicSize = 0;
    glyphsGBuff_->m_dynamicSize = 0;
}

// this could be smarter but it's debug code so meh..
// returns true if things are full and you should stop adding.
bool DDRenderInterfaceLuddite::checkCount( size_t currentSize, int countAdded )
{
    if (currentSize == MAX_DEBUG_POINTS) {
        if (!didWarnThisFrame_) {
            DBG::warn("Max debug verts exceeded\n" );
        }
        didWarnThisFrame_ = true;
        return true;
    }
    return false;
}

void DDRenderInterfaceLuddite::drawPointList(const dd::DrawVertex * points, const int count,
                                             const bool depthEnabled)
{
    // copy count verts from dd::DrawVertex into our format
    const dd::DrawVertex *curr = points;
    size_t countAdded = 0;
    for (int i=0; i < count; i++) {
        size_t ndx = pointsGBuff_->m_dynamicSize + i;
        pointsGBuff_->m_vertData[ndx].m_pos = GLKVector3Make( curr->point.x, curr->point.y, curr->point.z );
        pointsGBuff_->m_vertData[ndx].m_st = GLKVector3Make( curr->point.size, 0.0, 0.0 ); // stuff point size into ST coord
        pointsGBuff_->m_vertData[ndx].m_color[0] = curr->point.r * 0xFF;
        pointsGBuff_->m_vertData[ndx].m_color[1] = curr->point.g * 0xFF;
        pointsGBuff_->m_vertData[ndx].m_color[2] = curr->point.b * 0xFF;
        pointsGBuff_->m_vertData[ndx].m_color[3] = 0xFF;
        
        //        printf("%lu : %3.2f %3.2f %3.2f\n", ndx, curr->line.x, curr->line.y, curr->line.z );
        
        countAdded++;
        curr++;
        if (checkCount( pointsGBuff_->m_dynamicSize, countAdded ) ) {
            break;
        }
    }
    pointsGBuff_->m_dynamicSize += countAdded;
    //    printf("drawLinesList: %d count %d dynSize\n", count, linesGBuff_->m_dynamicSize );

}

void DDRenderInterfaceLuddite::drawLineList(const dd::DrawVertex * lines, const int count, const bool depthEnabled)
{    
    // copy count verts from dd::DrawVertex into our format
    const dd::DrawVertex *curr = lines;
    size_t countAdded = 0;
    for (int i=0; i < count; i++) {
        size_t ndx = linesGBuff_->m_dynamicSize + i;
        linesGBuff_->m_vertData[ndx].m_pos = GLKVector3Make( curr->line.x, curr->line.y, curr->line.z );
        linesGBuff_->m_vertData[ndx].m_color[0] = curr->line.r * 0xFF;
        linesGBuff_->m_vertData[ndx].m_color[1] = curr->line.g * 0xFF;
        linesGBuff_->m_vertData[ndx].m_color[2] = curr->line.b * 0xFF;
        linesGBuff_->m_vertData[ndx].m_color[3] = 0xFF;
        
//        printf("%lu : %3.2f %3.2f %3.2f\n", ndx, curr->line.x, curr->line.y, curr->line.z );
        
        countAdded++;
        curr++;
        if (checkCount( linesGBuff_->m_dynamicSize, countAdded ) ) {
            break;
        }
    }
    linesGBuff_->m_dynamicSize += countAdded;
//    printf("drawLinesList: %d count %d dynSize\n", count, linesGBuff_->m_dynamicSize );
}

void DDRenderInterfaceLuddite::drawGlyphList(const dd::DrawVertex * glyphs, const int count,
                   dd::GlyphTextureHandle glyphTex)
{
    // copy count verts from dd::DrawVertex into our format
    const dd::DrawVertex *curr = glyphs;
    size_t countAdded = 0;
    for (int i=0; i < count; i++) {
        size_t ndx = glyphsGBuff_->m_dynamicSize + i;
        glyphsGBuff_->m_vertData[ndx].m_pos = GLKVector3Make( curr->glyph.x, curr->glyph.y, 0.0 );
        glyphsGBuff_->m_vertData[ndx].m_st = GLKVector3Make( curr->glyph.u, curr->glyph.v, 0.0 );
        
        glyphsGBuff_->m_vertData[ndx].m_color[0] = curr->glyph.r * 0xFF;
        glyphsGBuff_->m_vertData[ndx].m_color[1] = curr->glyph.g * 0xFF;
        glyphsGBuff_->m_vertData[ndx].m_color[2] = curr->glyph.b * 0xFF;
        glyphsGBuff_->m_vertData[ndx].m_color[3] = 0xFF;
        
        countAdded++;
        curr++;
        if (checkCount( glyphsGBuff_->m_dynamicSize, countAdded ) ) {
            break;
        }
    }
    glyphsGBuff_->m_dynamicSize += countAdded;
//    printf("drawGlyphList: %d count %d dynSize\n", count, linesGBuff_->m_dynamicSize );

}


dd::GlyphTextureHandle DDRenderInterfaceLuddite::createGlyphTexture(const int width, const int height,
                                                                    const void * pixels)
{
    assert( mtlDB_ );
    
    printf("createGlyphTexture %dx%d (glyphsTex_ is %u sz: %d %d)\n", width, height,
           glyphsTex_->m_texId,
           glyphsTex_->m_width,
           glyphsTex_->m_height );
    
    glBindTexture(GL_TEXTURE_2D, glyphsTex_->m_texId  );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels );
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    
    
    return reinterpret_cast<dd::GlyphTextureHandle>( glyphsTex_ );
}


void DDRenderInterfaceLuddite::destroyGlyphTexture(dd::GlyphTextureHandle glyphTex)
{
    // meh... let it leak..
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

