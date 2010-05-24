/***********************************************************************
 
 Copyright (c) 2008, 2009, Memo Akten, www.memo.tv
 *** The Mega Super Awesome Visuals Company ***
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of MSA Visuals nor the names of its contributors 
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * ***********************************************************************/ 


#import "MSARootViewController.h"
#import "ofxiPhone.h"
#import "ofxiPhoneExtras.h"

@implementation MSARootViewController

@synthesize optionsViewController;

-(void) saveButtonReset {
	[saveButton setTitle:@"Save"];	
}


-(bool) isOn {
	return _isOn;
}

int numPages = 0;
CGRect scrollFrame;

-(void)addPage:(UIView*)view {
	scrollFrame.origin.x = scrollFrame.size.width * numPages++;
	view.frame = scrollFrame;
	view.backgroundColor = [UIColor clearColor];
	[scrollView addSubview:view];	
}

-(IBAction)nextPage:(id)sender {
	pageControl.currentPage++;
	[self pageChanged:nil];
}

-(IBAction)prevPage:(id)sender {
	pageControl.currentPage--;
	[self pageChanged:nil];
}

-(void)gotoPageNum:(int)i {
	pageControl.currentPage = i;
	[self pageChanged:nil];
}



-(void)updatePages {
	pageControl.numberOfPages = numPages;
	pageControl.currentPage = 0;
    scrollView.contentSize = CGSizeMake(scrollFrame.size.width * numPages, scrollFrame.size.height);
}


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
	ofLog(OF_LOG_VERBOSE, "MSARootViewController::viewDidLoad");
	
    [super viewDidLoad];
	
	self.view.backgroundColor = [UIColor colorWithWhite:0 alpha:0.7f];
	
	scrollFrame = scrollView.frame;
    scrollView.contentSize = CGSizeMake(scrollFrame.size.width * 3, scrollFrame.size.height);
	scrollView.delegate = self;
	scrollFrame.origin.y = 0;
	
	[self addPage:infoView];
//	[self addPage:optionsView1];
//	[self addPage:optionsView2];
}




- (void)scrollViewDidScroll:(UIScrollView *)sender {
    if(pageControlUsed) return;
	
    // Switch the indicator when more than 50% of the previous/next page is visible
    CGFloat pageWidth = scrollView.frame.size.width;
    int page = floor((scrollView.contentOffset.x - pageWidth / 2) / pageWidth) + 1;
	pageControl.currentPage = page;
}

- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView {
    pageControlUsed = NO;
}

- (IBAction)pageChanged:(id)sender {
    int page = pageControl.currentPage;
    CGRect frame = scrollView.frame;
    frame.origin.x = frame.size.width * page;
    frame.origin.y = 0;
    [scrollView scrollRectToVisible:frame animated:YES];
    pageControlUsed = YES;
}



// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationLandscapeLeft || interfaceOrientation == UIInterfaceOrientationLandscapeRight);
}




#define ANIMATION_TIME		0.5f
#define ANIMATION_CURVE		UIViewAnimationCurveEaseIn


-(void)openView:(bool)animate {
//	if(iPhoneGlobals.rootViewController.view.superview == nil) [iPhoneGetUIWindow() addSubview:iPhoneGlobals.rootViewController.view];
		
	if(self.view.superview == nil) {
		[self saveButtonReset];
		if(animate) {
			[UIView beginAnimations:nil context:NULL];
			[UIView setAnimationDuration:ANIMATION_TIME];
			[UIView setAnimationTransition:UIViewAnimationTransitionCurlDown forView:iPhoneGetUIWindow() cache:YES];
			[UIView setAnimationCurve: ANIMATION_CURVE];
			[self viewWillAppear:YES];
			[ofxiPhoneGetUIWindow() addSubview:self.view];
			[self viewDidAppear:YES];
			[UIView commitAnimations];
		} else {
			[ofxiPhoneGetUIWindow() addSubview:self.view];
		}

		activityIndicator.hidesWhenStopped = YES;
		[activityIndicator stopAnimating];
	}
	
	_isOn = true;
}

-(IBAction) closeView:(id)sender {
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationDuration:ANIMATION_TIME];
	[UIView setAnimationTransition:UIViewAnimationTransitionCurlUp forView:iPhoneGetUIWindow() cache:YES];
	[UIView setAnimationCurve: ANIMATION_CURVE];
	[self viewWillDisappear:YES];
	[self.view removeFromSuperview];
	[self viewDidDisappear:YES];
	[UIView commitAnimations];
	
	_isOn = false;
}


-(IBAction) savePhoto:(id)sender {
	[saveButton setTitle:@"Saving..."];
	[activityIndicator startAnimating];
	
	iPhoneScreenGrab(self);
}


-(void)saveComplete {
	UIActionSheet *alert = [[UIActionSheet alloc] initWithTitle:@"Image saved to Photo Library" delegate:self cancelButtonTitle:@"Ok" destructiveButtonTitle:nil otherButtonTitles:nil];
//	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Save complete" message:@"Image saved to Photos application" delegate:self cancelButtonTitle:@"Ok" otherButtonTitles:nil];
	
	UIImage *iconImage = [[UIImage alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"PhotoAppIcon" ofType:@"png"]];
	UIImageView *iconView = [[UIImageView alloc] initWithImage:iconImage];
	
	[alert addSubview:iconView];
	iconView.frame = CGRectMake(90, 6, 32, 32);
//	iconView.frame = CGRectMake(13, 10, 32, 32);
	
	[alert showInView:self.view];
//	[alert show];
	
	[alert release];
	[iconImage release];
	[iconView release];
	
	[activityIndicator stopAnimating];
	[self saveButtonReset];
}


-(void)alertView:(UIAlertView*)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex {
	[self closeView:nil];
}



- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}




- (void)dealloc {
    [super dealloc];
}


@end
