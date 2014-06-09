//
//  ImageAppViewController.m
//  Created by lukasz karluk on 11/02/14.
//  http://julapy.com
//

#import "ImageAppViewController.h"
#import "ImageApp.h"

@implementation ImageAppViewController

- (id)initWithCoder:(NSCoder *)aDecoder {
    self = [super initWithFrame:[UIScreen mainScreen].bounds app:new ImageApp()];
    if(self != nil) {
        //
    }
    return self;
}

@end
