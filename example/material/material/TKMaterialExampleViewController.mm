//
//  ViewController.m
//  material
//
//  Created by Joel Davis on 6/28/15.
//  Copyright (c) 2015 Joel Davis. All rights reserved.
//

#import "TKMaterialExampleViewController.h"

#include "MaterialGame.h"

@interface TKMaterialExampleViewController () <TKGameLoopDelegate>
{
    MaterialGame *m_mtlGame;
}

@end

@implementation TKMaterialExampleViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
     [self initScene];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)initScene
{
    m_mtlGame = new MaterialGame();
    self.gameDelegate = self;
    
    // Setup scene and load resources
    m_mtlGame->setupScene();
    
    // Set the view
    m_mtlGame->setViewport( self.view.bounds.size );
}

#pragma mark - TKGameLoopDelegate

- (void)updateFixed: (double)dt
{
    if (m_mtlGame) {
        m_mtlGame->updateFixed(dt);
    }
}

- (void)updateDynamic: (double)dt
{
    if (m_mtlGame) {
        m_mtlGame->updateDynamic(dt);
    }
}

- (void) drawScene
{
//    [self.imgui newFrame];
    
    if (m_mtlGame) {
//        if ([ImGuiHelper isActive])
//        {
//            m_pscenic->dbgInterface();
//        }
        m_mtlGame->drawScene();
    } else {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear( GL_COLOR_BUFFER_BIT );
    }
    
    // Now render Imgui
//    [self.imgui render];
}


@end
