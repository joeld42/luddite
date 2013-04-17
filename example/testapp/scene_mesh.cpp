
#include <EASTL/string.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <luddite/render/scene.h>
#include <luddite/render/render_device_gl.h>
#include <luddite/render/material_db.h>
#include <luddite/render/gbuff_prim.h>
#include <luddite/render/texture_info.h>

#include "scene_mesh.h"


eastl::string TestApp::SceneMesh::sceneName()
{
    return "mesh";
}


void TestApp::SceneMesh::init()
{
    // Set up luddite stuff
    m_renderDevice = new luddite::RenderDeviceGL();
    m_renderDevice = new luddite::RenderDeviceGL();

    // Init luddite material db
    //NSBundle *bundle = [NSBundle mainBundle];
    //NSString *resPath = [[bundle resourcePath] stringByAppendingString: @"/"];
    //NSLog( @"Resource Path is %@\n", resPath );

    // Initialize shader DB
    m_mtlDB = new luddite::MaterialDB();
    m_mtlDB->initShaderDB( "./gamedata" );

    // Add material def files
    m_mtlDB->addMaterialDefs("Sandbox.material.xml" );

    luddite::GBuff *gbuffCube = luddite::gbuff_cube( 1.0, vec3f( 0.0, 0.5, 0.0) );
    gbuff_setColorConstant( gbuffCube, vec4f( 1.0, 0.0, 1.0, 1.0) );

    luddite::GBuff *gbuffCyl = luddite::gbuff_cylinder();
    gbuff_setColorConstant( gbuffCyl, vec4f( 0.0, 1.0, 0.0, 1.0) );

    // Build scene graph
    luddite::SceneNode *worldRoot = new luddite::SceneNode( "worldRoot" );

    // Make a material
    luddite::Material *Material = m_mtlDB->_materialWithKey( "Sandbox.Plastic" );
    luddite::Material *mtl  = m_mtlDB->getNamedMaterial( m_renderDevice, "mtl.one" );
    luddite::Material *mtl2 = m_mtlDB->getNamedMaterial( m_renderDevice, "mtl.two" );
    luddite::Material *mtl3 = m_mtlDB->getNamedMaterial( m_renderDevice, "mtl.three" );


    printf("mtl %s tex[0] %p (%s)\n", mtl->m_materialName.c_str(),
            mtl->m_tex[0],
            mtl->m_tex[0]?mtl->m_tex[0]->m_filename.c_str():"none");

    printf("mtl2 %s tex[0] %p (%s)\n", mtl->m_materialName.c_str(),
            mtl2->m_tex[0],
            mtl2->m_tex[0]?mtl->m_tex[0]->m_filename.c_str():"none");

    printf("mtl3 %s tex[0] %p (%s)\n", mtl3->m_materialName.c_str(),
            mtl3->m_tex[0],
            mtl3->m_tex[0]?mtl3->m_tex[0]->m_filename.c_str():"none");

#if 0

    // DBG
//    Param pDbgColor("dbgColor");
//    pDbgColor = vec4f( 1.0, 1.0, 0.0, 1.0 );
//    mtl->setParam(pDbgColor);
//


    // Set color
//    luddite::Param Kd("Kd");
//    Kd = vec4f( 1.0f, 0.6f, 0.0f, 1.0 );
//    mtl->setParam( Kd );
//    printf("Set up param %s\n", Kd.m_name.c_str() );


//    for (Param &p : mtl->m_params)
//    {
//        printf("Param '%s'\n", p.m_name.c_str() );
//    }

    // make a ring of cube around the world root
    bool cube = true;
    luddite::SceneNode *cubeNode;
    for (float t=0.0; t <= 2.0*M_PI; t += 20.0 * (M_PI/180.0 ) )
    {
        vec3f cubePos = vec3f( cos(t)*2.0, 0.3, sin(t)*2.0 );
        cubeNode = new luddite::SceneNode( worldRoot );
        cubeNode->m_pos = cubePos;
//        NSLog( @"cube pos is %f %f %f", cubePos.x, cubePos.y, cubePos.z );

        // bind gbuff to a new gbatch, attach that to scene node
        GBatch *currBatch = new GBatch();
        currBatch->m_mtl = NULL;
        if (cube)
        {
            currBatch->m_gbuff = gbuffCube;
            currBatch->m_mtl = mtl2;
        }
        else
        {
            cubeNode->m_pos.y = sin(t*3) * 0.25;
            currBatch->m_gbuff = gbuffCyl;
            currBatch->m_mtl = mtl3;
        }
        cube = !cube;

        cubeNode->addGBatch( currBatch );
    }

    // Load the obj file in the center
//    NSString* filePath = [[NSBundle mainBundle] pathForResource:@"cube_mtl_test" ofType:@"obj"];
//    NSString* filePath = [[NSBundle mainBundle] pathForResource:@"cube_mtl_test_nrm" ofType:@"obj"];
//    NSString* filePath = [[NSBundle mainBundle] pathForResource:@"cube_mtl_test_nrm_st" ofType:@"obj"];
//    NSString* filePath = [[NSBundle mainBundle] pathForResource:@"cube_subd_nrm_st" ofType:@"obj"];
    NSString* filePath = [[NSBundle mainBundle] pathForResource:@"suzanne" ofType:@"obj"];
    NSLog( @"Loading OBJ from file path %@", filePath );

    luddite::SceneNode *objNode = scene_objfile([filePath UTF8String], _renderDevice, _mtlDB );

    if (objNode)
    {
//        // DBG: hardcode the mtl to all the obj file's batches
//        for (GBatchList::const_iterator bi = objNode->batches().begin();
//             bi != objNode->batches().end(); ++bi )
//        {
//            (*bi)->m_mtl = mtl;
//        }


        worldRoot->addChild( objNode );
    }

    // Load the grass texture
//    uint32_t texGrass = pfLoadTexture( "grass.png" );
//    NSLog( @"loaded grass texture, result is %d", texGrass );

    // Load the terrain file
    NSString* terrainPath = [[NSBundle mainBundle] pathForResource:@"grid10x10" ofType:@"obj"];
    NSLog( @"Loading OBJ from file path %@", terrainPath );

    luddite::SceneNode *terrainNode = scene_objfile([terrainPath UTF8String], _renderDevice, _mtlDB );

    // for now, just stuff the texture into the batches
//    const eastl::list<GBatch*> &terrBatches = terrainNode->batches();
//    for (auto bi=terrBatches.begin(); bi != terrBatches.end(); ++bi )
//    {
//        GBatch *batch = (*bi);
//        batch->m_tex[0] = texGrass;
//    }


    worldRoot->addChild( terrainNode );

    // fixme.. this is temporary, shouldn't need to do this
    _mtlDB->useAllShaders( _renderDevice );


    // Create scene
    _scene = new luddite::Scene( worldRoot );

    // Set up apple example stuff
//    [self loadShaders];
//
//    self.effect = [[GLKBaseEffect alloc] init];
//    self.effect.light0.enabled = GL_TRUE;
//    self.effect.light0.diffuseColor = GLKVector4Make(1.0f, 0.4f, 0.4f, 1.0f);
//    glEnable(GL_DEPTH_TEST);
//
//    glGenVertexArraysOES(1, &_vertexArray);
//    glBindVertexArrayOES(_vertexArray);
//
//    glGenBuffers(1, &_vertexBuffer);
//    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(gCubeVertexData), gCubeVertexData, GL_STATIC_DRAW);
//
//    glEnableVertexAttribArray(GLKVertexAttribPosition);
//    glVertexAttribPointer(GLKVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, 24, BUFFER_OFFSET(0));
//    glEnableVertexAttribArray(GLKVertexAttribNormal);
//    glVertexAttribPointer(GLKVertexAttribNormal, 3, GL_FLOAT, GL_FALSE, 24, BUFFER_OFFSET(12));
//
//    glBindVertexArrayOES(0);
#endif

}

void TestApp::SceneMesh::render()
{
    //printf("in SceneMesh::render...\n");
    glClearColor( 0.2f, 1.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );

}
