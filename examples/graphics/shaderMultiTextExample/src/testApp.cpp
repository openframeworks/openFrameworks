#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    
    srcImg.loadImage("A.jpg");
    dstImg.loadImage("B.jpg");
    brushImg.loadImage("brush.png");
    
    maskFbo.allocate(1024,768);
    multiTextFbo.allocate(1024,768);
    
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
    
    bBrushDown = false;
 
    maskFbo.begin();
    ofClear(0,0,0,255);
    maskFbo.end();
}

//--------------------------------------------------------------
void testApp::update(){
    int width = ofGetWindowWidth();
    int height = ofGetWindowHeight();
    
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
    multiTextShader.setUniformTexture("tex0", dstImg, 1 );
    multiTextShader.setUniformTexture("maskTex", maskFbo.getTextureReference(), 2 );
    
    // This is simply a frame where the shader can put the pixels on 
    // that«s why it have the glTexCoord2f()
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2f(width, 0); glVertex3f(width, 0, 0);
    glTexCoord2f(width, height); glVertex3f(width, height, 0);
    glTexCoord2f(0,height);  glVertex3f(0,height, 0);
    glEnd();

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