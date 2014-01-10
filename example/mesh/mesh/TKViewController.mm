//
//  TKViewController.m
//  mesh
//
//  Created by Joel Davis on 1/6/14.
//  Copyright (c) 2014 Tapnik. All rights reserved.
//

#import <luddite/common/debug.h>

#import "TKViewController.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace luddite;

@interface TKViewController ()
{
}
@property (strong, nonatomic) EAGLContext *context;


//- (BOOL)loadShaders;
//- (BOOL)compileShader:(GLuint *)shader type:(GLenum)type file:(NSString *)file;
//- (BOOL)linkProgram:(GLuint)prog;
//- (BOOL)validateProgram:(GLuint)prog;
@end

@implementation TKViewController

@synthesize meshGame=_meshGame;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        NSLog( @"TKViewController init here...");
    }
    return self;
}

- (void) gameInit
{
    self.meshGame = [[MeshGame alloc] init];
    self.gameDelegate = _meshGame;
}

- (void)viewDidLoad
{

    [super viewDidLoad];
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

    // Dispose of any resources that can be recreated.
}

//- (void)setupGL
//{
//    [EAGLContext setCurrentContext:self.context];
//    
//    // Set up GL stuff here
//    glEnable(GL_DEPTH_TEST);
//}
//

#pragma mark - GLKView and GLKViewController delegate methods

//- (void)update
//{
//    float aspect = fabsf(self.view.bounds.size.width / self.view.bounds.size.height);
//    GLKMatrix4 projectionMatrix = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(65.0f), aspect, 0.1f, 100.0f);
//    
//    self.effect.transform.projectionMatrix = projectionMatrix;
//    
//    GLKMatrix4 baseModelViewMatrix = GLKMatrix4MakeTranslation(0.0f, 0.0f, -4.0f);
//    baseModelViewMatrix = GLKMatrix4Rotate(baseModelViewMatrix, _rotation, 0.0f, 1.0f, 0.0f);
//    
//    // Compute the model view matrix for the object rendered with GLKit
//    GLKMatrix4 modelViewMatrix = GLKMatrix4MakeTranslation(0.0f, 0.0f, -1.5f);
//    modelViewMatrix = GLKMatrix4Rotate(modelViewMatrix, _rotation, 1.0f, 1.0f, 1.0f);
//    modelViewMatrix = GLKMatrix4Multiply(baseModelViewMatrix, modelViewMatrix);
//    
//    self.effect.transform.modelviewMatrix = modelViewMatrix;
//    
//    // Compute the model view matrix for the object rendered with ES2
//    modelViewMatrix = GLKMatrix4MakeTranslation(0.0f, 0.0f, 1.5f);
//    modelViewMatrix = GLKMatrix4Rotate(modelViewMatrix, _rotation, 1.0f, 1.0f, 1.0f);
//    modelViewMatrix = GLKMatrix4Multiply(baseModelViewMatrix, modelViewMatrix);
//    
//    _normalMatrix = GLKMatrix3InvertAndTranspose(GLKMatrix4GetMatrix3(modelViewMatrix), NULL);
//    
//    _modelViewProjectionMatrix = GLKMatrix4Multiply(projectionMatrix, modelViewMatrix);
//    
//    _rotation += self.timeSinceLastUpdate * 0.5f;
//}

//- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
//{
//    glClearColor(0.65f, 0.65f, 0.65f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    
//    glBindVertexArrayOES(_vertexArray);
//    
//    // Render the object with GLKit
//    [self.effect prepareToDraw];
//    
//    glDrawArrays(GL_TRIANGLES, 0, 36);
//    
//    // Render the object again with ES2
//    glUseProgram(_program);
//    
//    glUniformMatrix4fv(uniforms[UNIFORM_MODELVIEWPROJECTION_MATRIX], 1, 0, _modelViewProjectionMatrix.m);
//    glUniformMatrix3fv(uniforms[UNIFORM_NORMAL_MATRIX], 1, 0, _normalMatrix.m);
//    
//    glDrawArrays(GL_TRIANGLES, 0, 36);
//}

//#pragma mark -  OpenGL ES 2 shader compilation
//
//- (BOOL)loadShaders
//{
//    GLuint vertShader, fragShader;
//    NSString *vertShaderPathname, *fragShaderPathname;
//    
//    // Create shader program.
//    _program = glCreateProgram();
//    
//    // Create and compile vertex shader.
//    vertShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"vsh"];
//    if (![self compileShader:&vertShader type:GL_VERTEX_SHADER file:vertShaderPathname]) {
//        NSLog(@"Failed to compile vertex shader");
//        return NO;
//    }
//    
//    // Create and compile fragment shader.
//    fragShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"fsh"];
//    if (![self compileShader:&fragShader type:GL_FRAGMENT_SHADER file:fragShaderPathname]) {
//        NSLog(@"Failed to compile fragment shader");
//        return NO;
//    }
//    
//    // Attach vertex shader to program.
//    glAttachShader(_program, vertShader);
//    
//    // Attach fragment shader to program.
//    glAttachShader(_program, fragShader);
//    
//    // Bind attribute locations.
//    // This needs to be done prior to linking.
//    glBindAttribLocation(_program, GLKVertexAttribPosition, "position");
//    glBindAttribLocation(_program, GLKVertexAttribNormal, "normal");
//    
//    // Link program.
//    if (![self linkProgram:_program]) {
//        NSLog(@"Failed to link program: %d", _program);
//        
//        if (vertShader) {
//            glDeleteShader(vertShader);
//            vertShader = 0;
//        }
//        if (fragShader) {
//            glDeleteShader(fragShader);
//            fragShader = 0;
//        }
//        if (_program) {
//            glDeleteProgram(_program);
//            _program = 0;
//        }
//        
//        return NO;
//    }
//    
//    // Get uniform locations.
//    uniforms[UNIFORM_MODELVIEWPROJECTION_MATRIX] = glGetUniformLocation(_program, "modelViewProjectionMatrix");
//    uniforms[UNIFORM_NORMAL_MATRIX] = glGetUniformLocation(_program, "normalMatrix");
//    
//    // Release vertex and fragment shaders.
//    if (vertShader) {
//        glDetachShader(_program, vertShader);
//        glDeleteShader(vertShader);
//    }
//    if (fragShader) {
//        glDetachShader(_program, fragShader);
//        glDeleteShader(fragShader);
//    }
//    
//    return YES;
//}
//
//- (BOOL)compileShader:(GLuint *)shader type:(GLenum)type file:(NSString *)file
//{
//    GLint status;
//    const GLchar *source;
//    
//    source = (GLchar *)[[NSString stringWithContentsOfFile:file encoding:NSUTF8StringEncoding error:nil] UTF8String];
//    if (!source) {
//        NSLog(@"Failed to load vertex shader");
//        return NO;
//    }
//    
//    *shader = glCreateShader(type);
//    glShaderSource(*shader, 1, &source, NULL);
//    glCompileShader(*shader);
//    
//#if defined(DEBUG)
//    GLint logLength;
//    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
//    if (logLength > 0) {
//        GLchar *log = (GLchar *)malloc(logLength);
//        glGetShaderInfoLog(*shader, logLength, &logLength, log);
//        NSLog(@"Shader compile log:\n%s", log);
//        free(log);
//    }
//#endif
//    
//    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
//    if (status == 0) {
//        glDeleteShader(*shader);
//        return NO;
//    }
//    
//    return YES;
//}
//
//- (BOOL)linkProgram:(GLuint)prog
//{
//    GLint status;
//    glLinkProgram(prog);
//    
//#if defined(DEBUG)
//    GLint logLength;
//    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
//    if (logLength > 0) {
//        GLchar *log = (GLchar *)malloc(logLength);
//        glGetProgramInfoLog(prog, logLength, &logLength, log);
//        NSLog(@"Program link log:\n%s", log);
//        free(log);
//    }
//#endif
//    
//    glGetProgramiv(prog, GL_LINK_STATUS, &status);
//    if (status == 0) {
//        return NO;
//    }
//    
//    return YES;
//}
//
//- (BOOL)validateProgram:(GLuint)prog
//{
//    GLint logLength, status;
//    
//    glValidateProgram(prog);
//    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
//    if (logLength > 0) {
//        GLchar *log = (GLchar *)malloc(logLength);
//        glGetProgramInfoLog(prog, logLength, &logLength, log);
//        NSLog(@"Program validate log:\n%s", log);
//        free(log);
//    }
//    
//    glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
//    if (status == 0) {
//        return NO;
//    }
//    
//    return YES;
//}

@end
