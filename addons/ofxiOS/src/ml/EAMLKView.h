////
////  EAGLKView.h
////  iPhone+OF Static Library
////
////  Created by Dan Rosser on 11/5/22.
////
//
//
//#pragma once
//
//#import <MetalANGLE/MGLKViewController.h>
//#import <MetalANGLE/MGLKViewDelegate.h>
//#import <MetalANGLE/MGLKit.h>
//
//#import <UIKit/UIKit.h>
//#import <GLKit/GLKit.h>
//#import "ESRenderer.h"
//
///// ???: inherit MGLKViewDelegate?
//@protocol EAMGLKViewDelegate <NSObject>
//@optional
//- (void)glViewAnimationStarted;
//- (void)glViewAnimationStopped;
//- (void)glViewDraw;
//- (void)glViewResized;
//@end
//
//@interface EAMLKView : MGLKView
//{
//
//@protected
//
//    CGFloat scaleFactor;
//    CGFloat scaleFactorPref;
//
//    BOOL bUseDepth;
//    BOOL bUseMSAA;
//    BOOL bUseRetina;
//    NSInteger msaaSamples;
//    ESRendererVersion rendererVersion;
//}
//
///// TODO: need to give protocol explicity.
///// ???: can we assume EAGLKViewDelegate is inherit GLKViewDelegate?
//@property (nonatomic, weak) id delegate;
//
//- (instancetype)initWithFrame:(CGRect)frame
//          andPreferedRenderer:(ESRendererVersion)rendererVersion
//                        andAA:(bool)msaaEnabled
//                    andRetina:(bool)retinaEnabled
//               andRetinaScale:(CGFloat)retinaScale
//                  colorFormat:(MGLDrawableColorFormat)colorFormat
//                  depthFormat:(MGLDrawableDepthFormat)depthFormat
//                stencilFormat:(MGLDrawableStencilFormat)stencilFormat;
//
//
//
//- (void)setup;
//- (void)update;
//- (void)draw;
//- (void)destroy;
//
//- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
//- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
//- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
//- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
//
//- (void)touchesEstimatedPropertiesUpdated:(NSSet<UITouch *> *)toucheso;
//- (void)setMSAA:(bool)on;
//- (void)notifyAnimationStarted;
//- (void)notifyAnimationStopped;
//- (void)notifyDraw;
//- (void)notifyResized;
//
//@end
