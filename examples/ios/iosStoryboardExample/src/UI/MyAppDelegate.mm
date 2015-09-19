//
//  MyAppDelegate.m
//  Created by lukasz karluk on 11/02/14.
//  http://julapy.com
//

#import "MyAppDelegate.h"

@implementation MyAppDelegate

@synthesize navigationController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    [super applicationDidFinishLaunching:application];
    
    self.navigationController = [[[UINavigationController alloc] init] autorelease];
    [self.window setRootViewController:self.navigationController];
    
    UIStoryboard * storyboard = [UIStoryboard storyboardWithName:@"MyApp" bundle:nil];
    [self.navigationController pushViewController:[storyboard instantiateInitialViewController] animated:YES];
    
    //--- style the UINavigationController
    self.navigationController.navigationBar.barStyle = UIBarStyleBlackTranslucent;
    self.navigationController.navigationBar.topItem.title = @"Home";
    
    return YES;
}

- (void) dealloc {
    self.navigationController = nil;
    [super dealloc];
}

@end
