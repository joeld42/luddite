//
//  dbgdraw-luddite.h
//  pmines
//
//  Created by Joel Davis on 1/30/16.
//
//

#ifndef dbgdraw_luddite_h
#define dbgdraw_luddite_h

#include "debug_draw.hpp"

#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#else
#include <GL/gl.h>
#endif

#include <luddite/render/scene.h>
#include <luddite/render/render_device_gl.h>
#include <luddite/render/material_db.h>

class DDRenderInterfaceLuddite : public dd::RenderInterface
{
public:
    DDRenderInterfaceLuddite( luddite::RenderDevice *device,
                              luddite::MaterialDB *mtlDB,
                              luddite::SceneNode *worldRoot );
    
    void drawPointList(const dd::DrawVertex * points, const int count,
                       const bool depthEnabled) override;
    
    void drawLineList(const dd::DrawVertex * lines, const int count,
                      const bool depthEnabled) override;
    
    void drawGlyphList(const dd::DrawVertex * glyphs, const int count,
                       dd::GlyphTextureHandle glyphTex) override;
    
    dd::GlyphTextureHandle createGlyphTexture(const int width, const int height,
                                              const void * pixels) override;
    
    
    void destroyGlyphTexture(dd::GlyphTextureHandle glyphTex) override;
    
    void beginDraw() override;
    
    void endDraw() override { }
    
    
private:
    // stuff
    bool checkCount( size_t currentSize, int countAdded );
    
    static GLuint handleToGL(dd::GlyphTextureHandle handle);
    static dd::GlyphTextureHandle GLToHandle(const GLuint id);
    
    luddite::MaterialDB *mtlDB_ = nullptr;
    luddite::SceneNode *linesNode_ = nullptr;
    luddite::GBuff *linesGBuff_ = nullptr;
    
    luddite::SceneNode *pointsNode_ = nullptr;
    luddite::GBuff *pointsGBuff_ = nullptr;
    
    bool didWarnThisFrame_ = false;

};


#endif /* dbgdraw_luddite_h */
