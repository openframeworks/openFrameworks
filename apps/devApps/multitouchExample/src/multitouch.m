#include <unistd.h> 
#import <Foundation/Foundation.h> 
#include <CoreFoundation/CoreFoundation.h> 
#include <ApplicationServices/ApplicationServices.h> 

#include "testApp.h"

/* 
 Costantino Pistagna <valvoline@gmail.com> 
 
 This code will read raw input from all multi-touch devices attached to chain deviceList. 
 Either if you're dealing with a trackpad or magic mouse, you'll get the raw multi-touch taps. 
 
 touchCallback function uses the nFinger parameter to detect a multitouch event. We don't want to 
 detect the one finger gestures. 
 
 If you don't want to handle a gesture, just return null from the callback function. 
 
 Compile with: 
 
 gcc -o main main.m -F/System/Library/PrivateFrameworks -framework MultitouchSupport -lIOKit \ 
 -framework CoreFoundation -framework ApplicationServices -lobjc 
 
 If you make something usefull with this proof of concept, notify me, and i'll wrote some lines 
 on my page at: http://aladino.dmi.unict.it 
 
 */ 



/* 
 These structs are required, in order to handle some parameters returned from the 
 MultiTouchSupport.framework 
 */ 
typedef struct { 
	float x; 
	float y; 
}mtPoint; 

typedef struct { 
	mtPoint position; 
	mtPoint velocity; 
}mtReadout; 

/* 
 Some reversed engineered informations from MultiTouchSupport.framework 
 */ 
typedef struct 
{ 
	int frame; //the current frame 
	double timestamp; //event timestamp 
	int identifier; //identifier guaranteed unique for life of touch per device 
	int state; //the current state (not sure what the values mean) 
	int unknown1; //no idea what this does 
	int unknown2; //no idea what this does either 
	mtReadout normalized; //the normalized position and vector of the touch (0,0 to 1,1) 
	float size; //the size of the touch (the area of your finger being tracked) 
	int unknown3; //no idea what this does 
	float angle; //the angle of the touch -| 
	float majorAxis; //the major axis of the touch -|-- an ellipsoid. you can track the angle of each finger! 
	float minorAxis; //the minor axis of the touch -| 
	mtReadout unknown4; //not sure what this is for 
	int unknown5[2]; //no clue 
	float unknown6; //no clue 
}Touch; 

//a reference pointer for the multitouch device 
typedef void *MTDeviceRef; 

//the prototype for the callback function 
typedef int (*MTContactCallbackFunction)(int,Touch*,int,double,int); 

//returns a pointer to the default device (the trackpad?) 
MTDeviceRef MTDeviceCreateDefault(); 

//returns a CFMutableArrayRef array of all multitouch devices 
CFMutableArrayRef MTDeviceCreateList(void); 

//registers a device's frame callback to your callback function 
void MTRegisterContactFrameCallback(MTDeviceRef, MTContactCallbackFunction); 

//start sending events 
void MTDeviceStart(MTDeviceRef, int); 

//just output debug info. use it to see all the raw infos dumped to screen 
void printDebugInfos(int nFingers, Touch *data) { 
	int i; 
	for (i=0; i<nFingers; i++) { 
		Touch *f = &data[i]; 
		printf("Finger: %d, frame: %d, timestamp: %f, ID: %d, state: %d, PosX: %f, PosY: %f, VelX: %f, VelY: %f, Angle: %f, MajorAxis: %f, MinorAxis: %f\n", i, 
			   f->frame, 
			   f->timestamp, 
			   f->identifier, 
			   f->state, 
			   f->normalized.position.x, 
			   f->normalized.position.y, 
			   f->normalized.velocity.x, 
			   f->normalized.velocity.y, 
			   f->angle, 
			   f->majorAxis, 
			   f->minorAxis); 
	} 
} 

//this's a simple touchCallBack routine. handle your events here 
int touchCallback(int device, Touch *data, int nFingers, double timestamp, int frame) { 
	int i; 
	if(nFingers>=2) { //only report if two or more fingers are touching 
		printf("Device: %d ",device); 
		printDebugInfos(nFingers, data);	
		
		//use two different Touch pointers for the two fingers gestures 
		Touch *f1 = &data[0]; //first finger 
		Touch *f2 = &data[1]; //second finger 
		
		//compute the euclide distance between the two touches 
		float distAB = sqrt(((f1->normalized.position.x - f2->normalized.position.x) * \ 
							 (f1->normalized.position.x - f2->normalized.position.x) + \ 
							 (f1->normalized.position.y - f2->normalized.position.y) * \ 
							 (f1->normalized.position.y - f2->normalized.position.y)));	
		
		//if we pinch-in (zoom-in) 
		if(distAB > 0.40 && distAB < 0.41) { 
			printf("pinch-in detected\n"); 
			/* 
			 CGEventPost(kCGHIDEventTap, CGEventCreateKeyboardEvent (NULL, (CGKeyCode)55, true)); 
			 CGEventPost(kCGHIDEventTap, CGEventCreateKeyboardEvent (NULL, (CGKeyCode)69, true)); 
			 CGEventPost(kCGHIDEventTap, CGEventCreateKeyboardEvent (NULL, (CGKeyCode)69, false)); 
			 CGEventPost(kCGHIDEventTap, CGEventCreateKeyboardEvent (NULL, (CGKeyCode)55, false)); 
			 */ 
			//CGPostKeyboardEvent((CGCharCode)0, (CGKeyCode)55, true); // command (hit) 
			//			CGPostKeyboardEvent((CGCharCode)0, (CGKeyCode)69, true); // + (hit) 
			//			CGPostKeyboardEvent((CGCharCode)0, (CGKeyCode)69, false); // + (out) 
			//			CGPostKeyboardEvent((CGCharCode)0, (CGKeyCode)55, false); // command (out) 
		} else if(distAB < 0.80 && distAB > 0.79) { //if we pinch-out (zoom-out) 
			printf("pinch-out detected\n"); 
			//CGPostKeyboardEvent((CGCharCode)0, (CGKeyCode)55, true); // command (hit) 
			//			CGPostKeyboardEvent((CGCharCode)0, (CGKeyCode)78, true); // command (hit) 
			//			CGPostKeyboardEvent((CGCharCode)0, (CGKeyCode)78, false); // command (hit) 
			//			CGPostKeyboardEvent((CGCharCode)0, (CGKeyCode)55, false); // command (hit) 
		} 
	} 
	return 0; 
} 


void start(){
	int i;
NSMutableArray* deviceList = (NSMutableArray*)MTDeviceCreateList(); //grab our device list 
for(i = 0; i<[deviceList count]; i++) { //iterate available devices 
	MTRegisterContactFrameCallback([deviceList objectAtIndex:i], touchCallback); //assign callback for device 
	MTDeviceStart([deviceList objectAtIndex:i], 0); //start sending events 
} 

}