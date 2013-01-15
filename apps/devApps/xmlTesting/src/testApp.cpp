#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(255,255,255);

		//load a monospaced font
	//which we will use to show part of the xml structure
	TTF.loadFont("mono.ttf", 7);
    
    ofFile file;
    file.open("mySettings1.xml");
    ofBuffer buffer = file.readToBuffer();
    XML.loadFromBuffer(buffer.getText());
    
    ofXml child = XML.getChild("food[0]");
    //cout << child.getNumChildren() << endl;
    //ofXml child2 = child.getChildAt(0);
    //cout << child2.getNumChildren() << endl;
    
    
    ofXml newXML;
    // check here
    newXML.addValue("bar", "fux");
    newXML.getChildAt(0).addAttribute("Giroud", "France");
    //cout << newXML.toString() << endl;
    newXML.getChildAt(0).addValue("baz", "qux");
    cout << newXML.toString() << endl;
    newXML.getChildAt(0).addValue("flim", "flam");
    cout << newXML.toString() << endl;
    newXML.getChildAt(0).addValue("num", "2");
    newXML.getChildAt(0).getChildAt(0).addValue("num", "3");
    cout << newXML.toString() << endl;
    
    child.addChild(newXML);
    
    cout << child.toString() << endl;
    
}

//--------------------------------------------------------------
void testApp::update(){
	//we change the background color here based on the values
	//affected by the mouse position
	ofBackground((int)red,(int)green,(int)blue);

}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

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

    cout << " number before remove " <<  XML.getNumChildren() << endl;
    XML.remove("food[0]");
    cout << " number after remove " << XML.getNumChildren() << endl;
    
    ofXml name = XML.getChild("food[2]/name");
    cout << name.getValue() << endl;
    ofXml child = XML.getChildAt(0);
    
    cout << child.getNumChildren() << endl;
    
    XML.addValue("food[2]/blah", "barf");
    ofXml blah = XML.getChild("food[2]/blah");
    
    XML.addValue("foo", "blaf");
    
    cout << " number after add " << XML.getNumChildren() << endl;
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

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

