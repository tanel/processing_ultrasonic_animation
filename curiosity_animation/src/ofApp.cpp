#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //Set screen frame rate
    ofSetFrameRate( 60 );
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    //Set white background
    ofBackground( 255, 255, 255 );
    //Draw rubber as a blue line
    ofSetColor( 0, 0, 255 );                 //Set blue color
    ofLine( 0, 0, 200, 200 );  //Draw line
    //Draw ball as a red circle
    ofSetColor( 255, 0, 0 );
    ofFill();
    ofCircle( 0, 100, 20 );
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
