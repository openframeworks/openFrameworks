//
//  CircleAppViewController.m
//  Created by lukasz karluk on 11/02/14.
//  http://julapy.com
//

#import "CircleAppViewController.h"
#import "CircleApp.h"

@implementation CircleAppViewController

- (id)initWithCoder:(NSCoder *)aDecoder {
    self = [super initWithFrame:[UIScreen mainScreen].bounds app:new CircleApp()];
    if(self != nil) {
        //
    }
    return self;
}

@end
