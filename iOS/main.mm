//
//  main.mm
//  glow
//
//  Created by sunjun on 11-6-16.
//  Copyright 2011 sunstdio. All rights reserved.
//

#import <UIKit/UIKit.h>

int main(int argc, char *argv[]) 
{
	{
		char cwd[256];
		strcpy(cwd, argv[0]);
		int len = strlen(cwd);
		for(int i = len-1; i >= 0; i--) 
		{
			if(cwd[i] == '/') 
			{
				cwd[i] = 0;
				break;
			}
			cwd[i] = 0;
		}
		setenv("GPATH", cwd, 1);
	}
	    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, nil);
    [pool release];
    return retVal;
}
