#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    
    srcImg.loadImage("A.jpg");
    dstImg.loadImage("B.jpg");
    brushImg.loadImage("brush.png");
    
    int width = srcImg.getWidth();
    int height = srcImg.getHeight();
    
    maskFbo.allocate(width,height);
    multiTextFbo.allocate(width,height);
    
    ofSetWindowShape(width, height);
    
    
    // There are 3 of ways of loading a shader:
    //
    //  1 - Using just the name of the shader and ledding ofShader look for .frag and .vert: 
    //      Ex.: shader.load( "myShader");
    //
    //  2 - Giving the right file names for each one: 
    //      Ex.: shader.load( "myShader.vert","myShader.frag");
    //
    //  3 - And the third one it«s passing the shader programa on a single string;
    //
    string multiTexturingShaderProgram = "#version 120\n \
    #extension GL_ARB_texture_rectangle : enable\n \
    \
    uniform sampler2DRect tex0;\
    uniform sampler2DRect tex1;\
    uniform sampler2DRect maskTex;\
    \
    void main (void){\
        vec2 pos = gl_TexCoord[0].st;\
        \
        vec4 src = texture2DRect(tex0, pos);\
        vec4 dst = texture2DRect(tex1, pos);\
        vec4 mask = texture2DRect(maskTex, pos);\
        \
        gl_FragColor = vec4( mix(src , dst , mask.r) );\
    }";
    multiTextShader.setupShaderFromSource(GL_FRAGMENT_SHADER, multiTexturingShaderProgram);
    multiTextShader.linkProgram(); 
    

    // In order to pass the three textures and draw a new one on the FBO 
    // we are going to make a mesh that will work as a frame
    // What«s it«s importat here it«s to set the correct texCoords
    //
    frame.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    frame.addTexCoord(ofVec2f(0,0));
    frame.addVertex(ofVec3f(0,0,0));
    frame.addTexCoord(ofVec2f(width,0));
    frame.addVertex(ofVec3f(width,0,0));
    frame.addTexCoord(ofVec2f(width,height));
    frame.addVertex(ofVec3f(width,height,0));
    frame.addTexCoord(ofVec2f(0,height));
    frame.addVertex(ofVec3f(0,height,0));
    frame.addTexCoord(ofVec2f(0,0));
    frame.addVertex(ofVec3f(0,0,0));
    
    // Let«s clear the FBO«s
    // otherwise it will bring some junk with it from the memory    
    maskFbo.begin();
    ofClear(0,0,0,255);
    maskFbo.end();
    
    multiTextFbo.begin();
    ofClear(0,0,0,255);
    multiTextFbo.end();
    
    bBrushDown = false;
}

//--------------------------------------------------------------
void testApp::update(){
    
    // MASK (frame buffer object)
    //
    maskFbo.begin();
    if (bBrushDown){
        brushImg.draw(mouseX-25,mouseY-25,50,50);
    }
    maskFbo.end();
    
    // MULTITEXTURE MIXING FBO
    //
    multiTextFbo.begin();
    ofClear(0, 0, 0,0);
    multiTextShader.begin();
    multiTextShader.setUniformTexture("tex0", srcImg , 0 );
    multiTextShader.setUniformTexture("tex1", dstImg, 1 );
    multiTextShader.setUniformTexture("maskTex", maskFbo.getTextureReference(), 2 );
    
    frame.draw();

    multiTextShader.end();
    multiTextFbo.end();

}

//--------------------------------------------------------------
void testApp::draw(){
    ofSetColor(255,255);
    multiTextFbo.draw(0,0);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    maskFbo.begin();
    ofClear(0,0,0,255);
    maskFbo.end();
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    bBrushDown = true;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    bBrushDown = false;
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}