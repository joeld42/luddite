//
//  MeshGameDelegate.m
//  mesh
//
//  Created by Joel Davis on 1/9/14.
//  Copyright (c) 2014 Tapnik. All rights reserved.
//

#import "MeshGameDelegate.h"

@implementation MeshGameDelegate

- (void) updateFixed: (double)dt
{
    NSLog( @"MeshGameDelegate: updateFixed: %lf", dt );
}

- (void) updateDynamic: (double)dt
{
    NSLog( @"MeshGameDelegate: updateDynamic: %lf", dt );        
}


@end
