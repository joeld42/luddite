//
//  TKViewController.m
//  mesh
//
//  Created by Joel Davis on 1/6/14.
//  Copyright (c) 2014 Tapnik. All rights reserved.
//

#import <luddite/common/debug.h>

#import "TKMeshExampleViewController.h"

using namespace luddite;

@interface TKMeshExampleViewController ()
@property (strong, nonatomic) EAGLContext *context;
@end

@implementation TKMeshExampleViewController

@synthesize meshGame=_meshGame;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        NSLog( @"TKMeshExampleViewController init here...");
    }
    return self;
}

- (UIStatusBarStyle) preferredStatusBarStyle {
    return UIStatusBarStyleLightContent;
}

- (void) gameInit
{
    self.meshGame = [[MeshGame alloc] init];
    self.gameDelegate = _meshGame;
    
    // Set the view
    _meshGame.viewport = self.view.bounds.size;
}

- (void)viewDidLoad
{

    [super viewDidLoad];
    
    UIPanGestureRecognizer *panRecognizer = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(dbgAdjust:)];
    [self.view addGestureRecognizer:panRecognizer];

}

- (void)dbgAdjust:(UIPanGestureRecognizer*)gr
{
    CGPoint pos = [gr locationInView: self.view ];
    NSLog( @"pan: %f %f", pos.x, pos.y );
    
    float amt = (pos.y / 240.0) - 1.0;
    _meshGame.zval = amt*20;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

    // Dispose of any resources that can be recreated.
}

- (void) didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
    _meshGame.viewport = self.view.bounds.size;
}
@end
