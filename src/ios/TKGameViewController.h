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

// Called when a redraw is needed
- (void) drawScene;

@end

@interface TKGameViewController : GLKViewController <GLKViewDelegate>

// Place for subclasses to init game stuff
- (void)gameInit;

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect;

// This will do the game stuff
@property (nonatomic, strong) id<TKGameLoopDelegate> gameDelegate;

@end
