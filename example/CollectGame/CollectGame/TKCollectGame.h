//
//  TKCollectGame.h
//  CollectGame
//
//  Created by Joel Davis on 1/14/14.
//  Copyright (c) 2014 Tapnik. All rights reserved.
//

#import "TKCollectGame.h"

#import <Foundation/Foundation.h>
#import <luddite/TKGameViewController.h>

@interface TKCollectGame : NSObject <TKGameLoopDelegate>

- (id)init;

- (void)setupMeshScene;

@property (nonatomic, assign) CGSize viewport;

#pragma mark - TKGameLoopDelegate

- (void) updateFixed: (double)dt;
- (void) updateDynamic: (double)dt;

@end