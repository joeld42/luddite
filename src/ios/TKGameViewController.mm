//
//  TKGameViewController.m
//  luddite
//
//  Created by Joel Davis on 1/9/14.
//  Copyright (c) 2014 Tapnik. All rights reserved.
//

#import "TKGameViewController.h"

#import <luddite/common/debug.h>
#import <luddite/game/gameloop.h>
#import <luddite/render/scene.h>

@interface TKGameViewController ()
{
    luddite::GameLoop *_gameloop;
    BOOL _didGameInit;
}

// Common ctor code for all the initWith... messages
- (void) _commonInit;

@property (strong, nonatomic) EAGLContext *context;

@end

@implementation TKGameViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        [self _commonInit];
    }
    return self;
}


- (id)initWithCoder:(NSCoder *)aDecoder
{
    if (self) {
        [self _commonInit];
    }
    return [super initWithCoder:aDecoder];
}

// TODO: add _commonInit to initWithFrame, init..

- (void) _commonInit
{
    _gameloop = new luddite::GameLoop();
    _didGameInit = NO;
}

- (void)gameInit
{
    // Stub -- subclass can init game stuff here
}

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
    
//    luddite::DBG::info( "Hello luddite world...");
    
    [self setupGL];

}

- (void)dealloc
{
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    
    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        
        NSLog( @"Freeing GL Resources bc of memory warning...");
        [self tearDownGL];
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }
    
    // Dispose of any resources that can be recreated.
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
  
    // set up GL Stuff here
    glEnable(GL_DEPTH_TEST);
    
    if (!_didGameInit)
    {
        [self gameInit];
        _didGameInit = YES;
    }
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
    
    // delete any GL stuff here
    
//    if (_program) {
//        glDeleteProgram(_program);
//        _program = 0;
//    }
}


#pragma mark - Update

- (void) update
{
    // game loop
    double dt;
    while ((dt=_gameloop->simStep()) >= 0.0)
    {
        // DO GAME SIM STEP HERE
        
        // dt will always be a fixed value here (1.0/stepTime) for stability
        // but it's a good idea to use dt anyways.
        if ([self.gameDelegate respondsToSelector:@selector(updateFixed:)])
        {
            [self.gameDelegate updateFixed: dt ];
        }
    }
    
    // DO DYNAMIC SYM STEP HERE
    
    // Good for stuff like particles that works well with a variable timestep.
    // Avoid putting gameplay-dependant stuff here
    dt = _gameloop->dynamicStep();
    if ([self.gameDelegate respondsToSelector:@selector(updateDynamic:)])
    {
        [self.gameDelegate updateDynamic: dt ];
    }
}

#pragma mark - GLKViewDelegate

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    if ([self.gameDelegate respondsToSelector:@selector(drawScene)])
    {
        [self.gameDelegate drawScene ];
    }

    
}

@end
