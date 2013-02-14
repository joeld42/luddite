//
//  SandboxViewController.m
//  sandbox
//
//  Created by Joel Davis on 6/7/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#import "SandboxViewController.h"

#import <luddite/common/debug.h>
#import <luddite/game/gameloop.h>

#include <luddite/render/gbuff.h>
#include <luddite/render/gbuff_prim.h>
#include <luddite/render/render_device_es2.h>
#include <luddite/render/scene.h>
#include <luddite/render/scene_node.h>
#include <luddite/render/color_util.h>
#include <luddite/render/scene_objfile.h>
#include <luddite/render/material.h>
#include <luddite/render/material_db.h>
#include <luddite/render/texture_info.h>

using namespace luddite;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// Uniform index.
enum
{
    UNIFORM_MODELVIEWPROJECTION_MATRIX,
    UNIFORM_NORMAL_MATRIX,
    NUM_UNIFORMS
};
GLint uniforms[NUM_UNIFORMS];


// Attribute index.
enum
{
    ATTRIB_VERTEX,
    ATTRIB_NORMAL,
    NUM_ATTRIBUTES
};

GLfloat gCubeVertexData[216] = 
{
    // Data layout for each line below is:
    // positionX, positionY, positionZ,     normalX, normalY, normalZ,
    0.5f, -0.5f, -0.5f,        1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f,         1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f,         1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f,          1.0f, 0.0f, 0.0f,
    
    0.5f, 0.5f, -0.5f,         0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f,          0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f,          0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 1.0f, 0.0f,
    
    -0.5f, 0.5f, -0.5f,        -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,       -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,         -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,       -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        -1.0f, 0.0f, 0.0f,
    
    -0.5f, -0.5f, -0.5f,       0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, -0.5f,        0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, -0.5f,        0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, 0.5f,         0.0f, -1.0f, 0.0f,
    
    0.5f, 0.5f, 0.5f,          0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,         0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,        0.0f, 0.0f, 1.0f,
    
    0.5f, -0.5f, -0.5f,        0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f,         0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f,         0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,       0.0f, 0.0f, -1.0f,
    -0.5f, 0.5f, -0.5f,        0.0f, 0.0f, -1.0f
};

@interface SandboxViewController () {
    GLuint _program;
    
    GLKMatrix4 _modelViewProjectionMatrix;
    GLKMatrix3 _normalMatrix;
    float _rotation;
    
    GLuint _vertexArray;
    GLuint _vertexBuffer;
    
 
    luddite::GameLoop *_gameLoop;

    luddite::RenderDeviceES2 *_renderDevice;
    luddite::GBuff *_gbuffCube;
    
    luddite::Scene *_scene;
    luddite::MaterialDB *_mtlDB;
    
}

@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL;
- (void)tearDownGL;

- (BOOL)loadShaders;
- (BOOL)compileShader:(GLuint *)shader type:(GLenum)type file:(NSString *)file;
- (BOOL)linkProgram:(GLuint)prog;
- (BOOL)validateProgram:(GLuint)prog;

@end

@implementation SandboxViewController

@synthesize context = _context;
@synthesize effect = _effect;

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }    
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    // create game objects
    _gameLoop = new luddite::GameLoop();
    
    [self setupGL];
}

- (void)viewDidUnload
{    
    [super viewDidUnload];
    
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
	self.context = nil;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc. that aren't in use.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
    } else {
        return YES;
    }
    
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
    
    // Set up luddite stuff
    _renderDevice = new luddite::RenderDeviceES2();
    
    // Init luddite material db
    NSBundle *bundle = [NSBundle mainBundle];
    NSString *resPath = [[bundle resourcePath] stringByAppendingString: @"/"];    
    NSLog( @"Resource Path is %@\n", resPath );

    // Initialize shader DB
    _mtlDB = new luddite::MaterialDB();
    _mtlDB->initShaderDB([resPath UTF8String] );
    
    // Add material def files
    _mtlDB->addMaterialDefs("Sandbox.material.xml" );
    
    luddite::GBuff *gbuffCube = luddite::gbuff_cube( 1.0, vec3f( 0.0, 0.5, 0.0) );
    gbuff_setColorConstant( gbuffCube, vec4f( 1.0, 0.0, 1.0, 1.0) );
    
    luddite::GBuff *gbuffCyl = luddite::gbuff_cylinder();
    gbuff_setColorConstant( gbuffCyl, vec4f( 0.0, 1.0, 0.0, 1.0) );
        
    // Build scene graph
    luddite::SceneNode *worldRoot = new luddite::SceneNode( "worldRoot" );
    
    // Make a material
    luddite::Material *Material = _mtlDB->_materialWithKey( "Sandbox.Plastic" );
    luddite::Material *mtl = _mtlDB->getNamedMaterial( _renderDevice, "mtl.one" );
    luddite::Material *mtl2 = _mtlDB->getNamedMaterial( _renderDevice, "mtl.two" );
    luddite::Material *mtl3 = _mtlDB->getNamedMaterial( _renderDevice, "mtl.three" );


    printf("mtl %s tex[0] %p (%s)\n", mtl->m_materialName.c_str(),
            mtl->m_tex[0],
            mtl->m_tex[0]?mtl->m_tex[0]->m_filename.c_str():"none");

    printf("mtl2 %s tex[0] %p (%s)\n", mtl->m_materialName.c_str(),
            mtl2->m_tex[0],
            mtl2->m_tex[0]?mtl->m_tex[0]->m_filename.c_str():"none");

    printf("mtl3 %s tex[0] %p (%s)\n", mtl3->m_materialName.c_str(),
            mtl3->m_tex[0],
            mtl3->m_tex[0]?mtl3->m_tex[0]->m_filename.c_str():"none");



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
    [self loadShaders];
    
    self.effect = [[GLKBaseEffect alloc] init];
    self.effect.light0.enabled = GL_TRUE;
    self.effect.light0.diffuseColor = GLKVector4Make(1.0f, 0.4f, 0.4f, 1.0f);
    
    glEnable(GL_DEPTH_TEST);
    
    glGenVertexArraysOES(1, &_vertexArray);
    glBindVertexArrayOES(_vertexArray);
    
    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gCubeVertexData), gCubeVertexData, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(GLKVertexAttribPosition);
    glVertexAttribPointer(GLKVertexAttribPosition, 3, GL_FLOAT, GL_FALSE, 24, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(GLKVertexAttribNormal);
    glVertexAttribPointer(GLKVertexAttribNormal, 3, GL_FLOAT, GL_FALSE, 24, BUFFER_OFFSET(12));
    
    glBindVertexArrayOES(0);    
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
    
    glDeleteBuffers(1, &_vertexBuffer);
    glDeleteVertexArraysOES(1, &_vertexArray);
    
    self.effect = nil;
    
    if (_program) {
        glDeleteProgram(_program);
        _program = 0;
    }
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
    // game loop
    TimeInterval dt;
    while ((dt=_gameLoop->simStep()) >= 0.0)
    {
       // DO GAME SIM STEP HERE
       
       // dt will always be a fixed value here (1.0/stepTime) for stability
       // but it's a good idea to use dt anyways.
       
//        DBG::info( "Fixed timestep: %f\n", dt );
    }
        
    // DO DYNAMIC SYM STEP HERE
           
    // Good for stuff like particles that works well with a variable timestep.
    // Avoid putting gameplay-dependant stuff here
    dt = _gameLoop->dynamicStep();
//    DBG::info( "Dynamic timestep: %f\n", dt );

    
    // GLK sample code update
    float aspect = fabsf(self.view.bounds.size.width / self.view.bounds.size.height);
    GLKMatrix4 projectionMatrix = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(65.0f), aspect, 0.1f, 100.0f);
    
    
//    self.effect.transform.projectionMatrix = projectionMatrix;
    
    GLKMatrix4 baseModelViewMatrix = GLKMatrix4MakeTranslation(0.0f, 0.0f, -8.0f);
    
    baseModelViewMatrix = GLKMatrix4Rotate(baseModelViewMatrix, 40.0 * (M_PI/180.0), 1.0f, 0.0f, 0.0f);
    baseModelViewMatrix = GLKMatrix4Rotate(baseModelViewMatrix, _rotation * 0.5, 0.0f, 1.0f, 0.0f);
    
//    _renderDevice->matProjection = matrix4x4f( baseModelViewMatrix.m );
    
    // Compute the model view matrix for the object rendered with GLKit
//    GLKMatrix4 modelViewMatrix = GLKMatrix4MakeTranslation(0.0f, 0.0f, -1.5f);
//    modelViewMatrix = GLKMatrix4Rotate(modelViewMatrix, _rotation, 1.0f, 1.0f, 1.0f);
//    modelViewMatrix = GLKMatrix4Multiply(baseModelViewMatrix, modelViewMatrix);
    
//    self.effect.transform.modelviewMatrix = modelViewMatrix;
    
    // Compute the model view matrix for the object rendered with ES2
    GLKMatrix4 modelViewMatrix = GLKMatrix4MakeTranslation(0.0f, 0.0f, -1.5f);
    modelViewMatrix = GLKMatrix4Rotate(modelViewMatrix, _rotation, 1.0f, 1.0f, 1.0f);
    modelViewMatrix = GLKMatrix4Multiply(baseModelViewMatrix, modelViewMatrix);
    
    _normalMatrix = GLKMatrix3InvertAndTranspose(GLKMatrix4GetMatrix3(baseModelViewMatrix), NULL);
    
    _modelViewProjectionMatrix = GLKMatrix4Multiply(projectionMatrix, baseModelViewMatrix);
    
    // HACK: get the modelview to the renderdevice
    _renderDevice->matBaseModelView = matrix4x4f( baseModelViewMatrix.m );
    _renderDevice->matProjection    = matrix4x4f( projectionMatrix.m );
    
    _rotation += self.timeSinceLastUpdate * 0.5f;
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    glClearColor(0.65f, 0.65f, 0.65f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glBindVertexArrayOES(_vertexArray);
    
    // Render the object with GLKit
//    [self.effect prepareToDraw];    
//    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // Render the object again with ES2
    glUseProgram(_program);
    
//    glUniformMatrix4fv(uniforms[UNIFORM_MODELVIEWPROJECTION_MATRIX], 1, 0, _modelViewProjectionMatrix.m);
    glUniformMatrix3fv(uniforms[UNIFORM_NORMAL_MATRIX], 1, 0, _normalMatrix.m);
    
    // HACK
    _renderDevice->uparam_modelViewProjection = uniforms[UNIFORM_MODELVIEWPROJECTION_MATRIX];
    _renderDevice->uparam_normalMat = uniforms[UNIFORM_NORMAL_MATRIX];
    
    //glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // Draw scene
    _scene->eval( _renderDevice );
    _renderDevice->renderFrame();
    
}

#pragma mark -  OpenGL ES 2 shader compilation

- (BOOL)loadShaders
{
    GLuint vertShader, fragShader;
    NSString *vertShaderPathname, *fragShaderPathname;
    
    // Create shader program.
    _program = glCreateProgram();
    
    // Create and compile vertex shader.
    vertShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"vsh"];
    if (![self compileShader:&vertShader type:GL_VERTEX_SHADER file:vertShaderPathname]) {
        NSLog(@"Failed to compile vertex shader");
        return NO;
    }
    
    // Create and compile fragment shader.
    fragShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"fsh"];
    if (![self compileShader:&fragShader type:GL_FRAGMENT_SHADER file:fragShaderPathname]) {
        NSLog(@"Failed to compile fragment shader");
        return NO;
    }
    
    // Attach vertex shader to program.
    glAttachShader(_program, vertShader);
    
    // Attach fragment shader to program.
    glAttachShader(_program, fragShader);
    
    // Bind attribute locations.
    // This needs to be done prior to linking.
//    glBindAttribLocation(_program, ATTRIB_VERTEX, "position");
//    glBindAttribLocation(_program, ATTRIB_NORMAL, "normal");
    
    // Hack use attribs from luddite
    glBindAttribLocation(_program, VertexAttrib_POSITION, "position");
    glBindAttribLocation(_program, VertexAttrib_NORMAL, "normal");
    glBindAttribLocation(_program, VertexAttrib_COLOR, "color");
    
    // Link program.
    if (![self linkProgram:_program]) {
        NSLog(@"Failed to link program: %d", _program);
        
        if (vertShader) {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader) {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (_program) {
            glDeleteProgram(_program);
            _program = 0;
        }
        
        return NO;
    }
    
    // Get uniform locations.
    uniforms[UNIFORM_MODELVIEWPROJECTION_MATRIX] = glGetUniformLocation(_program, "modelViewProjectionMatrix");
    uniforms[UNIFORM_NORMAL_MATRIX] = glGetUniformLocation(_program, "normalMatrix");
    
    // Release vertex and fragment shaders.
    if (vertShader) {
        glDetachShader(_program, vertShader);
        glDeleteShader(vertShader);
    }
    if (fragShader) {
        glDetachShader(_program, fragShader);
        glDeleteShader(fragShader);
    }
    
    
    return YES;
}

- (BOOL)compileShader:(GLuint *)shader type:(GLenum)type file:(NSString *)file
{
    GLint status;
    const GLchar *source;
    
    source = (GLchar *)[[NSString stringWithContentsOfFile:file encoding:NSUTF8StringEncoding error:nil] UTF8String];
    if (!source) {
        NSLog(@"Failed to load vertex shader");
        return NO;
    }
    
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);
    
#if defined(DEBUG)
    GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
        NSLog(@"Shader compile log:\n%s", log);
        free(log);
    }
#endif
    
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        glDeleteShader(*shader);
        return NO;
    }
    
    return YES;
}

- (BOOL)linkProgram:(GLuint)prog
{
    GLint status;
    glLinkProgram(prog);
    
#if defined(DEBUG)
    GLint logLength;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        NSLog(@"Program link log:\n%s", log);
        free(log);
    }
#endif
    
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status == 0) {
        return NO;
    }
    
    return YES;
}

- (BOOL)validateProgram:(GLuint)prog
{
    GLint logLength, status;
    
    glValidateProgram(prog);
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        NSLog(@"Program validate log:\n%s", log);
        free(log);
    }
    
    glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
    if (status == 0) {
        return NO;
    }
    
    return YES;
}

@end
