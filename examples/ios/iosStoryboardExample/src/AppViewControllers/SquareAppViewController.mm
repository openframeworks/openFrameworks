//
//  SquareAppViewController.m
//  Created by lukasz karluk on 11/02/14.
//  http://julapy.com
//

#import "SquareAppViewController.h"
#import "SquareApp.h"

@implementation SquareAppViewController

- (id)initWithCoder:(NSCoder *)aDecoder {
    self = [super initWithFrame:[UIScreen mainScreen].bounds app:new SquareApp()];
    if(self != nil) {
        //
    }
    return self;
}

@end
