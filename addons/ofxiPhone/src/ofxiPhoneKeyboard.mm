/*
 *  ofxiPhoneKeyboard.cpp
 *  iPhone UIKeyboard Example
 *
 *  Created by Zach Gage on 3/1/09.
 *  Copyright 2009 stfj. All rights reserved.
 *
 */

#import "ofxiPhoneKeyboard.h"

//C++ class implementations

//--------------------------------------------------------------
ofxiPhoneKeyboard::ofxiPhoneKeyboard(int _x, int _y, int _w, int _h)
{
	keyboard = [[ofxiPhoneKeyboardDelegate alloc] 
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
ofxiPhoneKeyboard::~ofxiPhoneKeyboard()
{
	[keyboard release];
}


//--------------------------------------------------------------
void ofxiPhoneKeyboard::setVisible(bool visible)
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
void ofxiPhoneKeyboard::makeSecure()
{
	[keyboard makeSecure];
}
//--------------------------------------------------------------
void ofxiPhoneKeyboard::setMaxChars(int max)
{
	[keyboard setFieldLength:max];
}

//--------------------------------------------------------------
void ofxiPhoneKeyboard::setPosition(int _x, int _y)
{
	x=_x;
	y=_y;
	[keyboard setFrame: CGRectMake(x,y,w,h)];
}

//--------------------------------------------------------------
void ofxiPhoneKeyboard::setSize(int _w, int _h)
{
	w = _w;
	h = _h;
	[keyboard setFrame: CGRectMake(x,y,w,h)];
}

//--------------------------------------------------------------
void ofxiPhoneKeyboard::setFontSize(int ptSize)
{
	[keyboard setFontSize: ptSize];
}

//--------------------------------------------------------------
void ofxiPhoneKeyboard::setFontColor(int r, int g, int b, int a)
{
	[keyboard setFontColorRed: r 
						green: g 
						 blue: b 
						alpha: a];
}

//--------------------------------------------------------------
void ofxiPhoneKeyboard::setBgColor(int r, int g, int b, int a)
{
	[keyboard setBgColorRed: r 
					  green: g 
					   blue: b 
					  alpha: a];
}

//--------------------------------------------------------------
void ofxiPhoneKeyboard::setText(string _text)
{
	NSString * text = [[[NSString alloc] initWithCString: _text.c_str()] autorelease];
	[keyboard setText:text];
}

//--------------------------------------------------------------
void ofxiPhoneKeyboard::setPlaceholder(string _text)
{
	NSString * text = [[[NSString alloc] initWithCString: _text.c_str()] autorelease];
	[keyboard setPlaceholder:text];
}

//--------------------------------------------------------------
string ofxiPhoneKeyboard::getText()
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

void ofxiPhoneKeyboard::openKeyboard()
{
	[keyboard openKeyboard];
}

bool ofxiPhoneKeyboard::isKeyboardShowing()
{
	return [keyboard isKeyboardShowing];
}

void ofxiPhoneKeyboard::updateOrientation()
{
	[keyboard updateOrientation];
}



// CLASS IMPLEMENTATIONS--------------objc------------------------
//----------------------------------------------------------------
@implementation ofxiPhoneKeyboardDelegate


- (void)textFieldDidBeginEditing:(UITextField *)textField
{
	open = true;
}

- (void)makeSecure
{
	[_textField setSecureTextEntry:YES];
}

//--------------------------------------------------------------
- (void)textFieldDidEndEditing:(UITextField*)textField 
{
	delete[] _ctext;
	_ctext = new char[[[textField text] length]+1];
	[[textField text] getCString:_ctext maxLength:[[textField text] length]+1 encoding:NSASCIIStringEncoding];
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
- (id) init:(int)x y:(int)y width:(int)w height:(int)h
{
	if(self = [super init])
	{			
		_textField = [[UITextField alloc] initWithFrame:CGRectMake(0, 0, w, h)];
		
		switch (ofxiPhoneGetOrientation()) 
		{
			case OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT:
				_textField.transform = CGAffineTransformMakeRotation(-M_PI_2);
				break;
				
			case OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT:
				
				_textField.transform = CGAffineTransformMakeRotation(M_PI_2);
				break;
				
				/*case OFXIPHONE_ORIENTATION_UPSIDEDOWN:
				 _textField.transform = CGAffineTransformMakeRotation(M_PI / 2.0);
				 break;*/
				
			default:
				break;
		}
		
		[self setFrame:CGRectMake(x,y,w,h)];
		
		[_textField setDelegate:self];
		[_textField setBackgroundColor:[UIColor colorWithWhite:0.0 alpha:0.0]];
		[_textField setTextColor:[UIColor whiteColor]];
		[_textField setFont:[UIFont fontWithName:@"Helvetica" size:16]];
		[_textField setPlaceholder:@""];	
		
		_x=x;
		_y=y;
		_w=w;
		_h=h;
		fieldLength = -1;
	}
	return self;
}

- (void) updateOrientation
{
	_textField.transform = CGAffineTransformMakeRotation(0.0f);
	[self setFrame:CGRectMake(0,0,_w,_h)];
	
	switch (ofxiPhoneGetOrientation()) 
	{
		case OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT:
			_textField.transform = CGAffineTransformMakeRotation(-M_PI_2);
			break;
			
		case OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT:
			
			_textField.transform = CGAffineTransformMakeRotation(M_PI_2);
			break;
			
		case OFXIPHONE_ORIENTATION_PORTRAIT:
		case OFXIPHONE_ORIENTATION_UPSIDEDOWN:
			_textField.transform = CGAffineTransformMakeRotation(0.0f);
			break;
			
		default:
			break;
	}
	[self setFrame:CGRectMake(_x,_y,_w,_h)];
	
	if(open)
	{
		[_textField resignFirstResponder];
		[self openKeyboard];
	}
}

//--------------------------------------------------------------
- (void)dealloc 
{ 
	[_textField release];
	delete[] _ctext;
	
	[super dealloc];
}

//--------------------------------------------------------------
- (char *) getText
{
	return _ctext;
}

//--------------------------------------------------------------
- (void) showText
{
	[ofxiPhoneGetUIWindow() addSubview:_textField];
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
	
	switch (ofxiPhoneGetOrientation()) 
	{
		case OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT:
			[_textField setFrame: CGRectMake(rect.origin.y-rect.size.height, s.height-rect.size.width-rect.origin.x, rect.size.height, rect.size.width)];
			break;
			
		case OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT:
			[_textField setFrame: CGRectMake(s.width-rect.origin.y , rect.origin.x, rect.size.height, rect.size.width)];
			break;
			
			/*case OFXIPHONE_ORIENTATION_UPSIDEDOWN:
			 _textField = [[UITextField alloc] initWithFrame:CGRectMake(x+320, y, w, h)];
			 _textField.transform = CGAffineTransformMakeRotation(M_PI / 2.0);
			 break;*/
			
		default:
			[_textField setFrame: CGRectMake(rect.origin.x , rect.origin.y-rect.size.height, rect.size.width, rect.size.height)];
			break;
	}
}

- (void) setFieldLength: (int)len
{
	fieldLength = len;
}

//--------------------------------------------------------------
- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
    NSMutableString *newValue = [[textField.text mutableCopy] autorelease];
    [newValue replaceCharactersInRange:range withString:string];
	
	cout<<[newValue length]<<" "<<fieldLength;
	
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
@end