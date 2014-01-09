//
//  TKGameViewController.h
//  luddite
//
//  Created by Joel Davis on 1/9/14.
//  Copyright (c) 2014 Tapnik. All rights reserved.
//

#import <GLKit/GLKit.h>

@protocol TKGameLoopDelegate <NSObject>

- (void) updateFixed: (double)dt;
- (void) updateDynamic: (double)dt;

@optional

// Called before/after drawing the scene graph
- (void) drawScene;
- (void) drawSceneFinished;

// TODO: hud stuff
//- (void) drawHUD;

@end

@interface TKGameViewController : GLKViewController <GLKViewDelegate>

#pragma mark - GLKViewDelegate

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect;

// This will do the game stuff
@property (nonatomic, strong) id<TKGameLoopDelegate> gameDelegate;

@end
