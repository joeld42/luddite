//
//  TKCollectViewController.m
//  CollectGame
//
//  Created by Joel Davis on 1/14/14.
//  Copyright (c) 2014 Tapnik. All rights reserved.
//

#import <luddite/common/debug.h>

#import "TKCollectViewController.h"
#import "TKCollectGame.h"

using namespace luddite;

@interface TKCollectViewController ()
@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) TKCollectGame *game;
@end

@implementation TKCollectViewController

@synthesize game=_game;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        NSLog( @"TKCollectViewController init here...");
    }
    return self;
}

- (void) gameInit
{
    self.game = [[TKCollectGame alloc] init];
    self.gameDelegate = _game;
    
    // Set the view
    _game.viewport = self.view.bounds.size;
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

- (void) didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
    _game.viewport = self.view.bounds.size;
}
@end