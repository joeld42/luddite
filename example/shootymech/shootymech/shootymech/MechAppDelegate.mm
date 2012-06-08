//
//  MechAppDelegate.m
//  shootymech
//
//  Created by Joel Davis on 6/7/12.
//  Copyright (c) 2012 Joel Davis. All rights reserved.
//

#import <luddite/common/debug.h>

#import "MechAppDelegate.h"
#import "MechViewController.h"

using namespace luddite;

@implementation MechAppDelegate

@synthesize window = _window;
@synthesize viewController = _viewController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    DBG::info( "application:didFinishLaunchingWithOptions\n" );
    
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    // Override point for customization after application launch.
    self.viewController = [[MechViewController alloc] initWithNibName:@"MechViewController" bundle:nil];
    self.window.rootViewController = self.viewController;
    [self.window makeKeyAndVisible];
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
    
    DBG::info( "applicationWillResignActive\n" );
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    DBG::info( "applicationDidEnterBackground\n" );
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    DBG::info( "applicationWillEnterForeground\n" );
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    DBG::info( "applicationDidBecomeActive\n" );
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    DBG::info( "applicationWillTerminate\n" );
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

@end
