
#include <stdio.h>

#include <EASTL/string.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <luddite/common/useful.h>
#include <luddite/render/scene.h>
#include <luddite/render/scene_objfile.h>
#include <luddite/platform/platform.h>
#include <luddite/render/render_device_gl.h>
#include <luddite/render/material_db.h>
#include <luddite/render/gbuff_prim.h>
#include <luddite/render/texture_info.h>
#include <luddite/render/color_util.h>

#include "scene_mesh.h"


eastl::string TestApp::SceneMesh::sceneName()
{
    return "mesh";
}


void TestApp::SceneMesh::init()
{
    // Set up luddite stuff
    luddite::RenderDeviceGL *renderDeviceGL = new luddite::RenderDeviceGL();
    m_renderDevice = renderDeviceGL;

    // Setup camera (TODO: do this differently)
    glhPerspectivef2( renderDeviceGL->matProjection, 20.0, 800.0/600.0, 1.0, 500.0 );
    matrix4x4f cameraXlate, cameraRot;
    cameraXlate.Translate(0.0, -4, -15.0);
    cameraRot.RotateX( 15.0 * (M_PI/180.0) );
    renderDeviceGL->matBaseModelView = cameraXlate * cameraRot;
    
    // Initialize shader DB
    m_mtlDB = new luddite::MaterialDB( );
    m_mtlDB->initShaderDB();

    // Add material def files
    m_mtlDB->addMaterialDefs("Sandbox.material.xml" );
   
    luddite::GBuff *gbuffCube = luddite::gbuff_cube( 0.7, vec3f( 0.0, 0.5, 0.0) );
    gbuff_setColorConstant( gbuffCube, vec4f( 1.0, 0.0, 1.0, 1.0) );

    luddite::GBuff *gbuffCyl = luddite::gbuff_cylinder();
    gbuff_setColorConstant( gbuffCyl, vec4f( 0.0, 1.0, 0.0, 1.0) );

    // Build scene graph
    m_worldRoot = new luddite::SceneNode( "worldRoot" );

    // Make a material
    luddite::Material *mtl  = m_mtlDB->getNamedMaterial( m_renderDevice, "mtl.one" );
    luddite::Material *mtl3 = m_mtlDB->getNamedMaterial( m_renderDevice, "mtl.three" );
    
    // empty node to spin the scene
    m_spinnyNode = new luddite::SceneNode( m_worldRoot );

    // Load the monkey obj file in the center. This obj has 3 different materials,
    // each will be a separate gbatch
    m_monkeyNode = scene_objfile_named( "suzanne.obj", m_renderDevice, m_mtlDB );
    if (m_monkeyNode)
    {
        m_monkeyNode->m_rot.SetAngleAxis( 20.0 * (M_PI/180.0f), vec3f( 1.0f, 0.0, 0.0 ));
        m_spinnyNode->addChild( m_monkeyNode );
    }
        
    // instance a ring of shapes around the monkey
    bool cube = true;
    for (float t=0.0; t <= 2.0*M_PI; t += 20.0 * (M_PI/180.0 ) )
    {
        // Make a node for this shape
        vec3f shapePos = vec3f( cos(t)*3.0, 0.3, sin(t)*3.0 );
        luddite::SceneNode *shapeNode;
        shapeNode = new luddite::SceneNode( m_spinnyNode );
        shapeNode->m_pos = shapePos;

        // bind gbuff to a new gbatch, attach that to scene node
        luddite::GBatch *currBatch = new luddite::GBatch();
        currBatch->m_mtl = NULL;
        if (cube)
        {
            currBatch->m_gbuff = gbuffCube;
            currBatch->m_mtl = new luddite::Material( *mtl );
        }
        else
        {
            // alternate shapes are a cylinder in a wavy pattern
            shapeNode->m_pos.y = sin(t*3) * 0.25;
            currBatch->m_gbuff = gbuffCyl;
            currBatch->m_mtl = new luddite::Material( *mtl3 );
        }
        cube = !cube;
        
        // Set the material color
        vec3f meshColor = hsv2rgb( vec3f( t * (180.0/M_PI), 1.0, 1.0 ));
        currBatch->m_mtl->param( "dbgColor") = vec4f( meshColor.r, meshColor.g, meshColor.b, 1.0 );
        
        // Add this as a batch to the node
        shapeNode->addGBatch( currBatch );
        m_meshNodes.push_back(shapeNode);
    }


    // FIXME.. this is temporary, shouldn't need to do this (and we shouldn't
    // upload all the shaders, only used ones)
    m_mtlDB->useAllShaders( m_renderDevice );

    // Finally, create scene for our node graph
    m_scene = new luddite::Scene( m_worldRoot );
}

void TestApp::SceneMesh::updateFixed( float dt )
{
    quat4f qrot;
    qrot.SetEuler( vec3f(0.0, 0.2f*dt, 0.0), prmath::EULER_XYZ );

    // update the spinny node
    m_spinnyNode->m_rot *= qrot;
    
    // spin the meshes in the opposite direction
    qrot.SetEuler( vec3f(0.0, -0.3f*dt, 0.0), prmath::EULER_XYZ );
    for (auto mesh : m_meshNodes)
    {
        mesh->m_rot *= qrot;
    }
}

void TestApp::SceneMesh::updateDynamic( float dt )
{    
}

void TestApp::SceneMesh::render()
{
//    static long frameCount;
//    printf("in SceneMesh::render [%ld]\n", frameCount++ );
    glClearColor( 0.2f, 0.2f, 0.25f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    // Eval and render the scene
    m_scene->eval( m_renderDevice );
    m_renderDevice->renderFrame();

}
