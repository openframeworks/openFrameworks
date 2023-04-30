/*
 *  ofxiOSKeyboard.cpp
 *  iPhone UIKeyboard Example
 *
 *  Created by Zach Gage on 3/1/09.
 *  Copyright 2009 stfj. All rights reserved.
 *
 */

#include "ofxiOSKeyboard.h"
#include "ofxiOSExtras.h"
#include "ofAppiOSWindow.h"
#include "ofAppRunner.h"
#include "ofLog.h"

using std::string;

//C++ class implementations

//--------------------------------------------------------------
ofxiOSKeyboard::ofxiOSKeyboard(int _x, int _y, int _w, int _h)
{
	keyboard = [[ofxiOSKeyboardDelegate alloc] 
				init:	_x 
				y:		_y 
				width:	_w 
				height:	_h];
	x=_x;
	y=_y;
	w = _w;
	h = _h;
}

//--------------------------------------------------------------
ofxiOSKeyboard::~ofxiOSKeyboard()
{
}


//--------------------------------------------------------------
void ofxiOSKeyboard::setVisible(bool visible)
{
	if(visible)
	{
		[keyboard showText];
	}
	else
	{
		[keyboard hideText];
	}
	
}
//--------------------------------------------------------------
void ofxiOSKeyboard::makeSecure()
{
	[keyboard makeSecure];
}
//--------------------------------------------------------------
void ofxiOSKeyboard::setMaxChars(int max)
{
	[keyboard setFieldLength:max];
}

//--------------------------------------------------------------
void ofxiOSKeyboard::setPosition(int _x, int _y)
{
	x=_x;
	y=_y;
	[keyboard setFrame: CGRectMake(x,y,w,h)];
}

//--------------------------------------------------------------
void ofxiOSKeyboard::setSize(int _w, int _h)
{
	w = _w;
	h = _h;
	[keyboard setFrame: CGRectMake(x,y,w,h)];
}

//--------------------------------------------------------------
void ofxiOSKeyboard::setFontSize(int ptSize)
{
	[keyboard setFontSize: ptSize];
}

//--------------------------------------------------------------
void ofxiOSKeyboard::setFontColor(int r, int g, int b, int a)
{
	[keyboard setFontColorRed: r 
						green: g 
						 blue: b 
						alpha: a];
}

//--------------------------------------------------------------
void ofxiOSKeyboard::setBgColor(int r, int g, int b, int a)
{
	[keyboard setBgColorRed: r 
					  green: g 
					   blue: b 
					  alpha: a];
}

//--------------------------------------------------------------
void ofxiOSKeyboard::setText(string _text)
{
	NSString * text = [NSString stringWithUTF8String:_text.c_str()];
	[keyboard setText:text];
}

//--------------------------------------------------------------
void ofxiOSKeyboard::setPlaceholder(string _text)
{
	NSString * text = [NSString stringWithUTF8String:_text.c_str()];
	[keyboard setPlaceholder:text];
}

//--------------------------------------------------------------
string ofxiOSKeyboard::getText()
{
	if([keyboard getText] == nil)
	{
		return "";
	}
	else
	{
		return string([keyboard getText]);
	}
}

//--------------------------------------------------------------
string ofxiOSKeyboard::getLabelText()
{
	if([keyboard getLabelText] == nil)
	{
		return "";
	}
	else
	{
		return string([keyboard getLabelText]);
	}
}

void ofxiOSKeyboard::openKeyboard()
{
	[keyboard openKeyboard];
}

bool ofxiOSKeyboard::isKeyboardShowing()
{
	return [keyboard isKeyboardShowing];
}

void ofxiOSKeyboard::updateOrientation()
{
	[keyboard updateOrientation];
}

UITextField * ofxiOSKeyboard::getKeyboardTextField() {
    return [keyboard getTextField];
}

// CLASS IMPLEMENTATIONS--------------objc------------------------
//----------------------------------------------------------------
@implementation ofxiOSKeyboardDelegate


- (void)textFieldDidBeginEditing:(UITextField *)textField
{
	open = true;
}

- (void)makeSecure
{
	[_textField setSecureTextEntry:YES];
}

//--------------------------------------------------------------
- (void)textFieldDidEndEditing:(UITextField*)textField {
    open = false;
}

//--------------------------------------------------------------
// Terminates the editing session
- (BOOL)textFieldShouldReturn:(UITextField*)textField
{
	//Terminate editing
	[textField resignFirstResponder];
	
	return YES;
}

//--------------------------------------------------------------
- (instancetype) init:(int)x y:(int)y width:(int)w height:(int)h
{
	if(self = [super init])
	{			
		_textField = [[UITextField alloc] initWithFrame:CGRectMake(0, 0, w, h)];
		
		_x=x;
		_y=y;
		_w=w;
		_h=h;
		
		_xOriginal = x;
		_yOriginal = y;
		
        if(!ofxiOSGetOFWindow()->doesHWOrientation()) {
            switch (ofGetOrientation())
            {
                // TODO: Move all these positions transformations to setFrame
                case OF_ORIENTATION_90_LEFT:
                    _x = ofGetWidth() - _xOriginal - _w;
                    _y = ofGetHeight() - _yOriginal;
                    break;
                    
                case OF_ORIENTATION_90_RIGHT:
                    _x = ofGetWidth() - _xOriginal - _w;
                    _y = ofGetHeight() - _yOriginal;
                    break;
                    
                case OF_ORIENTATION_180:
                    _x = ofGetWidth() - _xOriginal - _w;
                    _y = ofGetHeight() - _yOriginal;
                    break;
                    
                case OF_ORIENTATION_DEFAULT:
                    _x = _xOriginal;
                    _y = _yOriginal;
                    break;
            }
        }
		
		
		[self setFrame:CGRectMake(_x,_y,_w,_h)];
		
		[_textField setDelegate:self];
		[_textField setBackgroundColor:[UIColor colorWithWhite:0.0 alpha:0.0]];
		[_textField setTextColor:[UIColor whiteColor]];
		[_textField setFont:[UIFont fontWithName:@"Helvetica" size:16]];
		[_textField setPlaceholder:@""];	
		
		fieldLength = -1;
	}
	return self;
}

- (void) updateOrientation
{
	_textField.transform = CGAffineTransformMakeRotation(0.0f);
	
    if(!ofxiOSGetOFWindow()->doesHWOrientation()) {
        switch (ofGetOrientation())
        {
                // TODO: Move all these positions transformations to setFrame
            case OF_ORIENTATION_90_LEFT:
                _x = ofGetWidth() - _xOriginal - _w;
                _y = ofGetHeight() - _yOriginal;
                break;
                
            case OF_ORIENTATION_90_RIGHT:
                _x = ofGetWidth() - _xOriginal - _w;
                _y = ofGetHeight() - _yOriginal;
                break;
                
            case OF_ORIENTATION_180:
                _x = ofGetWidth() - _xOriginal - _w;
                _y = ofGetHeight() - _yOriginal;
                break;
                
            case OF_ORIENTATION_DEFAULT:
                _x = _xOriginal;
                _y = _yOriginal;
                break;
        }
    }
	
	
	[self setFrame:CGRectMake(_x,_y,_w,_h)];
	
	if(open)
	{
		[_textField resignFirstResponder];
		[self openKeyboard];
	}
}

//--------------------------------------------------------------
- (void)dealloc {
    _textField = nil;
}

//--------------------------------------------------------------
- (const char *) getText {
    return [[_textField text] UTF8String];
}

//--------------------------------------------------------------
- (const char*) getLabelText {
    // this method is deprecated, use getText instead.
    return [self getText];
}

//--------------------------------------------------------------
- (void) showText
{
	[ofxiOSGetGLParentView() addSubview:_textField];
}

//--------------------------------------------------------------
- (void) hideText
{
	[_textField endEditing:YES];
	[_textField removeFromSuperview];
}

//--------------------------------------------------------------
- (void) setText: (NSString *)text
{
	[_textField setText:text];
}

//--------------------------------------------------------------
- (void) setPlaceholder: (NSString *)text
{
	[_textField setPlaceholder:text];
}

//--------------------------------------------------------------
- (void) setFontSize: (int)size
{
	[_textField setFont:[UIFont fontWithName:@"Helvetica" size:size]];
}

//--------------------------------------------------------------
- (void) setFontColorRed: (int)r green: (int)g blue:(int)b alpha:(int)a
{
	[_textField setTextColor:[UIColor 
							  colorWithRed:	(float)r/255 
							  green:		(float)g/255 
							  blue:			(float)b/255 
							  alpha:		(float)a/255]];
}

//--------------------------------------------------------------
- (void) setBgColorRed: (int)r green: (int)g blue:(int)b alpha:(int)a
{
	[_textField setBackgroundColor:[UIColor 
									colorWithRed:	(float)r/255 
									green:			(float)g/255 
									blue:			(float)b/255 
									alpha:			(float)a/255]];
}

//--------------------------------------------------------------
- (bool) isKeyboardShowing
{
	return open;
}

//--------------------------------------------------------------
- (void) setFrame: (CGRect) rect
{
	
	CGSize s = [[[UIApplication sharedApplication] keyWindow] bounds].size;		
	
    int x, y, w, h;
    
    if(!ofxiOSGetOFWindow()->doesHWOrientation()) {
        switch (ofGetOrientation())
        {
            case OF_ORIENTATION_90_LEFT:
                _textField.transform = CGAffineTransformMakeRotation(M_PI_2);
                x = rect.origin.y-rect.size.height;
                y = s.height-rect.size.width-rect.origin.x;
                w = rect.size.height;
                h = rect.size.width;
                break;
                
            case OF_ORIENTATION_90_RIGHT:
                _textField.transform = CGAffineTransformMakeRotation(-M_PI_2);
                x = s.width-rect.origin.y;
                y = rect.origin.x;
                w = rect.size.height;
                h = rect.size.width;
                break;
                
            case OF_ORIENTATION_180:
                _textField.transform = CGAffineTransformMakeRotation(M_PI);
                x = rect.origin.x;
                y = rect.origin.y-rect.size.height;
                w = rect.size.width;
                h = rect.size.height;
                break;
                
            case OF_ORIENTATION_DEFAULT:
                x = rect.origin.x;
                y = rect.origin.y;
                w = rect.size.width;
                h = rect.size.height;
                break;
        }
    } else {
        x = rect.origin.x;
        y = rect.origin.y;
        w = rect.size.width;
        h = rect.size.height;
    }
    
    
    [_textField setFrame: CGRectMake(x,y,w,h)];
}

- (void) setFieldLength: (int)len
{
	fieldLength = len;
}

//--------------------------------------------------------------
- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
    NSMutableString *newValue = [textField.text mutableCopy];
    [newValue replaceCharactersInRange:range withString:string];
	
	ofLogVerbose("ofxiOSKeyboard") << "shouldChangeCharactersInRange: " << [newValue length] << " " << fieldLength;
	
	if(fieldLength != -1)
	{
		if ([newValue length] > fieldLength)
		{
			return NO;
		}
	}
	
    return YES;
}

//--------------------------------------------------------------
- (void) openKeyboard
{
	[_textField becomeFirstResponder];
}
//--------------------------------------------------------------
- (UITextField *)getTextField {
    return _textField;
}

@end
