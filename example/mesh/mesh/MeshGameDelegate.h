//
//  MeshGameDelegate.h
//  mesh
//
//  Created by Joel Davis on 1/9/14.
//  Copyright (c) 2014 Tapnik. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <luddite/TKGameViewController.h>

@interface MeshGameDelegate : NSObject <TKGameLoopDelegate>

- (void) updateFixed: (double)dt;
- (void) updateDynamic: (double)dt;

// Called before/after drawing the scene graph
//- (void) drawScene;
//- (void) drawSceneFinished;

// TODO: hud stuff
//- (void) drawHUD;


@end
