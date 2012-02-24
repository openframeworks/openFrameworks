#include "testApp.h"

#include "Poco/RegularExpression.h"
using Poco::RegularExpression;

// Some explination on regualre expressions
// http://gnosis.cx/publish/programming/regular_expressions.html



//--------------------------------------------------------------
void testApp::setup() {
    
    ofBackground(0);
    
    string term         = "openframeworks";
    string googleImgURL = "http://www.google.com/search?q="+term+"&tbm=isch&sout=1&tbs=isz";
    cout << "seraching for " << googleImgURL << endl;
    
    // ofHttpResponse res = ofLoadURL(googleImgURL);
    // if(res.status > 0) {
    
    //Poco::Re
    //Poco::RegExp::RegularExpression regex("<table width=\"100%\" class=\"images_table\" style=\"table-layout:fixed\">(*)</table>");
    
    rawData = ofBufferFromFile("OF.html").getText();
    //<img src=\"(.*)\">
    
    //And using this regex: (?<=img+.+src\=[\x27\x22])(?<Url>[^\x27\x22]*)(?=[\x27\x22])
    

    
    RegularExpression re("<img[^>]+>");//<a href=\"[0-9]+\">[A-Za-z]+</a>");
    RegularExpression::Match match;
   
    while ( re.match(rawData, match) != 0) {
        
        string found = rawData.substr(match.offset, match.length);
        rawData = rawData.substr(match.offset + match.length);
        
        cout << found << endl;//rawData.substr(matches[1].offset, matches[1].length) << endl;
        
    }

    //cout << re.match(ss.str(), match.offset, match) << endl;
   
    
    //while(matches.m
    
    //int nMatches = regEx.match(rawData, 0, matches);
    
    //if(nMatches) {
    
    //string m = rawData.substr(matches.offset, matches.length);
    //cout << m << endl;
    
    
    //}
    //cout << matches.size() << endl;
    
    //cout << res.data.getText() << endl;
    
    
    // }
    
}

//--------------------------------------------------------------
void testApp::update(){
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
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