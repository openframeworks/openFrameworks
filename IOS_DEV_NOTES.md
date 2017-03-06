This branch PR updates ofxiOS with ARC compatibility.


Testing results:

**Legend:**   
- Green: Pass  
- Red: Fails to compile or run
- Blue: Fails but due to existing issues in master/nightly
- Yellow: Compiles but needs updates
- Orange: Needs special treatment

![Tested Examples](http://i.imgur.com/OIisA8x.png)

**TO DO**  
 - Destruction/Lifecycle of ofApp needs work
 - ofxAccelerometer needs restored/re-implemented
 - Project Generator should mark addon .cpp files as Objective-C++
 - Remove synthesized properties
 - Sets iPhone Deployment target to 9.0, if this is ok there is a lot of dead code to be removed
 

 ```
 	<key>explicitFileType</key>
				<string>sourcecode.cpp.objcpp</string>
```

**EXAMPLE CHECKLIST** 

- [ ] advancedEventsExample
- [ ] advancedGraphicsExample
- [ ] assimpExample
- [ ] audioInputExample
- [ ] audioOutputExample
- [ ] CoreLocationExample
- [ ] dirListExample
- [ ] emptyExample
- [ ] eventsExample
- [ ] fontsExample
- [ ] fontShapesExample
- [ ] graphicsExample
- [ ] imageLoaderExample
- [ ] ImagePickerExample
- [ ] iosCoreMotionCameraExample
- [ ] iosCoreMotionExample
- [ ] iosCoreMotionLegacyExample
- [ ] iosCustomSizeExample
- [ ] iosES2ShaderExample
- [ ] iosExternalDisplayExample
- [ ] iosNativeExample
- [ ] iosOrientationExample
- [ ] iosStoryboardExample
- [ ] iPhoneGuiExample
- [ ] KeyboardExample
- [ ] MapKitExample
- [ ] moviePlayerExample
- [ ] ofxGuiExample
- [ ] opencvExample
- [ ] opencvFaceExample
- [ ] oscReceiverExample
- [ ] oscSenderExample
- [ ] polygonExample
- [ ] PrimitivesExample
- [ ] soundPlayerExample
- [ ] textureExample
- [ ] vboExample
- [ ] videoGrabberExample
- [ ] xmlSettingsExample
