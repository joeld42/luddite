//
//  SandboxAppDelegate.h
//  sandbox
//
//  Created by Joel Davis on 6/7/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#import <UIKit/UIKit.h>

@class SandboxViewController;

@interface SandboxAppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;

@property (strong, nonatomic) SandboxViewController *viewController;

@end
