//
//  glowAppDelegate.h
//  glow
//
//  Created by sunjun on 11-6-16.
//  Copyright 2011 sunstdio. All rights reserved.
//

#import <UIKit/UIKit.h>

@class EAGLView;

@interface glowAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    EAGLView *glView;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet EAGLView *glView;

@end

