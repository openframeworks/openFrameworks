//
//  TriangleAppViewController.m
//  Created by lukasz karluk on 11/02/14.
//  http://julapy.com
//

#import "TriangleAppViewController.h"
#import "TriangleApp.h"

@implementation TriangleAppViewController

- (id)initWithCoder:(NSCoder *)aDecoder {
    self = [super initWithFrame:[UIScreen mainScreen].bounds app:new TriangleApp()];
    if(self != nil) {
        //
    }
    return self;
}

@end
