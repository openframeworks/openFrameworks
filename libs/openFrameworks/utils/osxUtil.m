#import "osxUtil.h"
#import <Cocoa/Cocoa.h>

/*********************
The OS X cocoa framework can't be accessed directly from c++.
A work around is to create an objective-c file. The file can be compiled and 
the methods called from c++ ( using extern 'C' {...} to include the header file )
**********************/


/*
 The OS X alert window requires a NSAutoReleasePool to be created and
 not having it causes a memory leak. These methods create 
 and destroy the AutoreleasePool.
*/
NSAutoreleasePool *pool = NULL;

void CreateNSAutoreleasePool()
{
	pool = [[NSAutoreleasePool alloc] init];
}

void DrainNSAutoreleasePool()
{
	[pool drain];	
}
