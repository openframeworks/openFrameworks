//
//  MyGuiView.h
//  assimpExample
//
//  Created by azuremous on 29/01/2024.

#import <UIKit/UIKit.h>

#include "ofApp.h"

@interface MyGuiView : UIViewController {
    IBOutlet UILabel *displayText;
    IBOutlet UIStackView *animationSlideView;
    ofApp *myApp;
}


-(void)setStatusString:(NSString *)trackStr;

-(IBAction)animationPosition:(id)sender;
-(IBAction)toggleAnimation:(id)sender;
-(IBAction)showNextAnimation:(id)sender;
-(IBAction)toggleCamera:(id)sender;

-(IBAction)more:(id)sender;
-(IBAction)less:(id)sender;

-(IBAction)hide:(id)sender;

@end
