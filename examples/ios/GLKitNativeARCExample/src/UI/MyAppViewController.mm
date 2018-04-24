//
//  MenuViewController.m
//  Created by lukasz karluk on 12/12/11.
// Updated for iOS8+ by Andreas Borg 11/18/15
// Updated for GLKit Dan Rosser 10/04/18
#import "MyAppViewController.h"

#import "SquareAppViewController.h"
#import "SquareApp.h"

#import "CircleAppViewController.h"
#import "CircleApp.h"

#import "TriangleAppViewController.h"
#import "TriangleApp.h"

#import "ImageAppViewController.h"
#import "ImageApp.h"



@interface MyAppViewController()
@property UIScrollView* containerView;
@end

@implementation MyAppViewController



- (UIButton*) makeButtonWithFrame:(CGRect)frame 
                          andText:(NSString*)text {
    UIFont *font;
    font = [UIFont fontWithName:@"Georgia" size:30];
    
    UILabel *label;
    label = [[ UILabel alloc] initWithFrame:CGRectMake(0, 0, frame.size.width, frame.size.height)];
    label.backgroundColor = [UIColor colorWithWhite:1 alpha:0.95];
    label.textColor = [UIColor colorWithWhite:0 alpha:1];
    label.text = [text uppercaseString];
    label.textAlignment = NSTextAlignmentCenter;
    label.font = font;
    label.userInteractionEnabled = NO;
    label.exclusiveTouch = NO;
    
    UIButton* button = [[UIButton alloc] initWithFrame:frame];
    [button setBackgroundColor:[UIColor clearColor]];
    [button addSubview:label];
    
    return button;
}

- (void)loadView {
    [super loadView];
    
  
    
    self.view.backgroundColor = [UIColor grayColor];
    
    
    CGRect screenRect = [[UIScreen mainScreen] bounds];
    
    CGRect scrollViewFrame = CGRectMake(0.f,
                                        0.f,
                                        screenRect.size.width,
                                        screenRect.size.height);
    
    self.containerView = [[UIScrollView alloc] initWithFrame:scrollViewFrame];
    self.containerView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    self.containerView.backgroundColor = [UIColor clearColor];
    self.containerView.showsHorizontalScrollIndicator = NO;
    self.containerView.showsVerticalScrollIndicator = YES;
    self.containerView.alwaysBounceVertical = YES;
    [self.view addSubview:self.containerView];

    NSArray *buttonTitles;
    buttonTitles = [NSArray arrayWithObjects: @"square", @"circle", @"triangle", @"image", nil];
    
    NSInteger buttonY = 44;     // make room for navigation bar.
    NSInteger buttonGap = 2;
    NSInteger buttonHeight = (screenRect.size.height - 44) / [buttonTitles count] - buttonGap * ([buttonTitles count] - 1);
    CGRect buttonRect = CGRectMake(0, 0, screenRect.size.width, buttonHeight);
    
    for (int i = 0; i < [buttonTitles count]; i++) {
        UIButton *button;
        button = [self makeButtonWithFrame:CGRectMake(0, buttonY, buttonRect.size.width, buttonRect.size.height)
                                   andText:[buttonTitles objectAtIndex:i]];
        [self.containerView addSubview:button ];
        
        if (i== 0)
            [button addTarget:self action:@selector(button1Pressed:) forControlEvents:UIControlEventTouchUpInside];
        else if (i==1)
            [button addTarget:self action:@selector(button2Pressed:) forControlEvents:UIControlEventTouchUpInside];
        else if (i==2)
            [button addTarget:self action:@selector(button3Pressed:) forControlEvents:UIControlEventTouchUpInside];
        else if (i==3)
            [button addTarget:self action:@selector(button4Pressed:) forControlEvents:UIControlEventTouchUpInside];
        
        buttonY += buttonRect.size.height;
        buttonY += buttonGap;
    }
    
    self.containerView.contentSize = CGSizeMake(buttonRect.size.width, buttonRect.size.height * 3);
}

- (void)button1Pressed:(id)sender {
    SquareAppViewController *viewController;
    viewController = [[SquareAppViewController alloc] initWithFrame:[[UIScreen mainScreen] bounds]
                                                                 app:new SquareApp() sharegroup:nil];
    
    [self.navigationController pushViewController:viewController animated:YES];
    self.navigationController.navigationBar.topItem.title = @"SquareApp";
}

- (void)button2Pressed:(id)sender {
    CircleAppViewController *viewController;
    viewController = [[CircleAppViewController alloc] initWithFrame:[[UIScreen mainScreen] bounds]
                                                                 app:new CircleApp() sharegroup:nil];
    
    [self.navigationController pushViewController:viewController animated:YES];
    self.navigationController.navigationBar.topItem.title = @"CircleApp";
}

- (void)button3Pressed:(id)sender {
    TriangleAppViewController *viewController;
    viewController = [[TriangleAppViewController alloc] initWithFrame:[[UIScreen mainScreen] bounds]
                                                                   app:new TriangleApp() sharegroup:nil];
    
    [self.navigationController pushViewController:viewController animated:YES];
    self.navigationController.navigationBar.topItem.title = @"TriangleApp";
}

- (void)button4Pressed:(id)sender {
    ImageAppViewController *viewController;
    viewController = [[ImageAppViewController alloc] initWithFrame:[[UIScreen mainScreen] bounds]
                                                                app:new ImageApp() sharegroup:nil];
    
    [self.navigationController pushViewController:viewController animated:YES];
    self.navigationController.navigationBar.topItem.title = @"ImageApp";
}
// iOS7-
//borg
- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
    return NO;
/*
    BOOL bRotate = NO;
    bRotate = bRotate || (toInterfaceOrientation == UIInterfaceOrientationPortrait);
    bRotate = bRotate || (toInterfaceOrientation == UIInterfaceOrientationPortraitUpsideDown);
    return bRotate;
    */
}



// iOS8+ version of willAnimateRotationToInterfaceOrientation
//borg
- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator {
    //this only reports accurately when this is actually in view. If you enter an app,
    //then rotate and exit, it will not be updating correctly..hence still needs
    //separate updateLayout checking active rotation
    cout<<"MyAppViewController - w "<<size.width<<" h "<<size.height<<endl;;
    
    [self updateLayout];
    
}
-(void)viewWillAppear:(BOOL)animated{
    [self updateLayout];
}

-(void)updateLayout{
    string currentOrientation;

    CGRect rect;
    int longSide = MAX([[UIScreen mainScreen] bounds].size.width,[[UIScreen mainScreen] bounds].size.height);
    int shortSide = MIN([[UIScreen mainScreen] bounds].size.width,[[UIScreen mainScreen] bounds].size.height);
    if (UIDeviceOrientationIsLandscape([UIDevice currentDevice].orientation)){
         // code for landscape orientation
        currentOrientation = "Landscape";
        rect = CGRectMake(0,0,longSide,shortSide);
    }else{
        currentOrientation = "Portrait";
        rect = CGRectMake(0,0,shortSide,longSide);
    }
    
    //UIDeviceOrientationIsLandscape not same results as ofGetOrientation()
    
    cout<<"MyAppViewController updateLayout - w "<<rect.size.width<<" h "<<rect.size.height<<", currentOrientation "<<currentOrientation<<endl;
    
    self.view.frame = rect;
    self.containerView.frame = rect;
    
    //this is not elegant...create bespoke classes instead
    for(UIView* btn in self.containerView.subviews){
        if([btn isKindOfClass:[UIButton class]]){
            btn.frame = CGRectMake(btn.frame.origin.x, btn.frame.origin.y, rect.size.width, btn.frame.size.height);
            for(UIView* label in btn.subviews){
                if([label isKindOfClass:[UILabel class]]){
                    label.frame = CGRectMake(label.frame.origin.x, label.frame.origin.y, rect.size.width, label.frame.size.height);
                }
            }
        }
    
    }
    self.containerView.contentSize = CGSizeMake(rect.size.width, self.containerView.contentSize.height);
}

@end
