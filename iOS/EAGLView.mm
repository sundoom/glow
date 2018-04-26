//
//  EAGLView.mm
//  glow
//
//  Created by sunjun on 11-6-16.
//  Copyright 2011 sunstdio. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////

#import "EAGLView.h"

//////////////////////////////////////////////////////////////////////////

#include <Glow.h>
#include <GMath.h>
NAMESPACE_USE(Math)
#include <Render.h>
NAMESPACE_USE(Render)

#include "../xtom/Game.h"
NAMESPACE_USE(XTom)

//////////////////////////////////////////////////////////////////////////
/*
====================
GLoad
====================
*/
/*
static NSString* s_app_dir = NULL;
const GData* GLoad(const CHAR* path)
{
	NSString* name = [s_app_dir stringByAppendingPathComponent:[NSString stringWithUTF8String:path]];
	NSData *ns_data = [NSData dataWithContentsOfFile:name];
	CHECK(ns_data);
	U8* data_ptr = (U8*)[ns_data bytes];	
	U32 data_size = [ns_data length];
	GData*data = GNEW(GData(data_size+1,0));
	CHECK(data);
	::memcpy(data->Ptr(), data_ptr, data_size);
	return data;
}
*/
static Str s_app_dir;
const GData* GLoad(const CHAR* path)
{
	Str name = s_app_dir + path;
	GFile file;
	file.Open(name.c_str());
	U32 pos = file.Seek(0,FS_CUR);
	U32 size = file.Seek(0,FS_END);	
	file.Seek(pos,FS_SET);
	GData* data = GNEW(GData(size+1,0)); CHECK(data);
	CHECK(file.Read(data->Ptr(),data->Size())==size);
	file.Close();
	return data;
}

//////////////////////////////////////////////////////////////////////////

@interface EAGLView ()

@property (nonatomic, getter=isAnimating) BOOL animating;

- (BOOL)createFramebuffer;
- (void)destroyFramebuffer;

@end

@implementation EAGLView

@synthesize animating, animationFrameInterval, displayLink, animationTimer;

// You must implement this method
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

//The EAGL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
- (id)initWithCoder:(NSCoder*)coder
{
	// get the app dir
	// s_app_dir = [NSString stringWithUTF8String:getenv("GPATH")];
	s_app_dir = getenv("GPATH");
	s_app_dir += "/data/";
	
    self = [super initWithCoder:coder];
    if (self)
    {
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;

        eaglLayer.opaque = TRUE;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];

        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
		if (!context || ![EAGLContext setCurrentContext:context])
        {
            [self release];
            return nil;
        }
		
		// Create default framebuffer object.
		[self createFramebuffer];        

        animating = FALSE;
        displayLinkSupported = FALSE;
        animationFrameInterval = 1;
        displayLink = nil;
        animationTimer = nil;

        // A system version of 3.1 or greater is required to use CADisplayLink. The NSTimer
        // class is used as fallback when it isn't available.
        NSString *reqSysVer = @"3.1";
        NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
        if ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending)
            displayLinkSupported = TRUE;
            
		// init the time
		Timer::instance().time();

		// init the game
		Game::instance().init();
    }

    return self;
}

- (void)drawView:(id)sender
{
	// get the frame time
	static U32 system_time = Timer::instance().time();
	U32 current_time = Timer::instance().time();
	U32 frame_time = current_time - system_time;
	if(frame_time == 0) return;
	system_time = current_time;
	
    // This application only creates a single context which is already set current at this point.
    // This call is redundant, but needed if dealing with multiple contexts.
    [EAGLContext setCurrentContext:context];    
	
	// run the game
	Game::instance().run(frame_time*0.001);
	
    // This application only creates a single color renderbuffer which is already bound at this point.
    // This call is redundant, but needed if dealing with multiple renderbuffers.
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER];	
}

- (void)layoutSubviews
{
	[EAGLContext setCurrentContext:context];
	[self destroyFramebuffer];
	[self createFramebuffer];
    [self drawView:nil];
}

- (NSInteger)animationFrameInterval
{
    return animationFrameInterval;
}

- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
    // Frame interval defines how many display frames must pass between each time the
    // display link fires. The display link will only fire 30 times a second when the
    // frame internal is two on a display that refreshes 60 times a second. The default
    // frame interval setting of one will fire 60 times a second when the display refreshes
    // at 60 times a second. A frame interval setting of less than one results in undefined
    // behavior.
    if (frameInterval >= 1)
    {
        animationFrameInterval = frameInterval;

        if (animating)
        {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

- (void)startAnimation
{
    if (!animating)
    {
        if (displayLinkSupported)
        {
            // CADisplayLink is API new to iPhone SDK 3.1. Compiling against earlier versions will result in a warning, but can be dismissed
            // if the system version runtime check for CADisplayLink exists in -initWithCoder:. The runtime check ensures this code will
            // not be called in system versions earlier than 3.1.

            self.displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(drawView:)];
            [displayLink setFrameInterval:animationFrameInterval];
            [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        }
        else
            self.animationTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)((1.0 / 60.0) * animationFrameInterval) target:self selector:@selector(drawView:) userInfo:nil repeats:TRUE];

        self.animating = TRUE;
    }
}

- (void)stopAnimation
{
    if (animating)
    {
        if (displayLinkSupported)
        {
            [displayLink invalidate];
            self.displayLink = nil;
        }
        else
        {
            [animationTimer invalidate];
            self.animationTimer = nil;
        }

        self.animating = FALSE;
    }
}

- (void)dealloc
{
	// exit the game
	Game::instance().exit();
	
    // Tear down GL
    [self destroyFramebuffer];
	
    // Tear down context
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
	
    [context release];
    context = nil;
    
    [displayLink release];

    [super dealloc];
}

- (BOOL)createFramebuffer
{	
	// create the default framebuffer object.
    glGenFramebuffers(1, &defaultFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
 
	// create the color renderbuffer object.
	glGenRenderbuffers(1, &colorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)self.layer];
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
    
    // get the stride of the color renderbuffer
    GLint backingWidth, backingHeight;
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
	
	// create the depth renderbuffer object.
	glGenRenderbuffers(1, &depthRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
	
	// check the state of the framebuffer
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        return NO;
    }
    
    // bind the fbo
    Game::instance().bind(defaultFramebuffer);
	
	// resize the window 
	Game::instance().window(backingWidth,backingHeight);
	
	return YES;
}

// Clean up any buffers we have allocated.
- (void)destroyFramebuffer
{
	if (defaultFramebuffer)
    {
        glDeleteFramebuffers(1, &defaultFramebuffer);
        defaultFramebuffer = 0;
    }
	
    if (colorRenderbuffer)
    {
        glDeleteRenderbuffers(1, &colorRenderbuffer);
        colorRenderbuffer = 0;
    }
    
    if(depthRenderbuffer)
	{
		glDeleteRenderbuffers(1, &depthRenderbuffer);
		depthRenderbuffer = 0;
	}
}

@end
