#include "ofApp.h"
#include <vector>

using namespace std;

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	
	int num = 1500;
	p.assign(num, Particle());
	currentMode = PARTICLE_MODE_ATTRACT;

	currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse"; 

	resetParticles();
}

//--------------------------------------------------------------
void ofApp::resetParticles(){

	//these are the attraction points used in the fourth demo 
	attractPoints.clear();
	for(int i = 0; i < 4; i++){
		attractPoints.push_back( glm::vec3( ofMap(i, 0, 4, 100, ofGetWidth()-100) , ofRandom(100, ofGetHeight()-100) , 0) );
	}
	
	attractPointsWithMovement = attractPoints;
	
	for(unsigned int i = 0; i < p.size(); i++){
		p[i].setMode(currentMode);		
		p[i].setAttractPoints(&attractPointsWithMovement);;
		p[i].reset();
	}	
}

//--------------------------------------------------------------
void ofApp::update(){
	if (!isPaused){
	for(unsigned int i = 0; i < p.size(); i++){
		p[i].setMode(currentMode);
		p[i].update();
	}
	
	//lets add a bit of movement to the attract points
	for(unsigned int i = 0; i < attractPointsWithMovement.size(); i++){
		attractPointsWithMovement[i].x = attractPoints[i].x + ofSignedNoise(i * 10, ofGetElapsedTimef() * 0.7) * 12.0;
		attractPointsWithMovement[i].y = attractPoints[i].y + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 12.0;
	}
	}	
		if (isReplaying){
		time++;
		if( time == 100 && keypress <= recordV.size()){
			time = 0;
			keyPressed(recordV[keypress]);
			keypress++;
		}
		isRecording = false;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor(24,84,26), ofColor(10,10,10));
	if(isRecording == true){ // Draw Circular Recorder
		ofSetColor(220,20,60); //makes the color crimson red
		ofDrawCircle(ofGetWidth()-38,38,35);
	}
	if(isReplaying == true){ // Draw Triangle Playback
		ofSetColor(0,255,0); // makes the color green
		ofDrawTriangle(ofGetWidth()-50,0,ofGetWidth(),25,ofGetWidth()-50,50);
	}


	for(unsigned int i = 0; i < p.size(); i++){
		p[i].draw();
	}
	
	
	ofSetColor(190);
	if( currentMode == PARTICLE_MODE_NEAREST_POINTS ){
		for(unsigned int i = 0; i < attractPoints.size(); i++){
			ofNoFill();
			ofDrawCircle(attractPointsWithMovement[i], 10);
			ofFill();
			ofDrawCircle(attractPointsWithMovement[i], 4);
		}
	}

	ofSetColor(230);	
	ofDrawBitmapString(currentModeStr + "\n\nSpacebar to reset. \nKeys 1-4 to change mode. \nKey A to toggle freeze."
		"\nKey S to halve Velocity(hold for continous effect) \nKey T to double Velocity(hold for continous effect)" 
		"\nKey D to double particle size \nKey M to halve particle size"
		"\nKey R to Record keys pressed \nKey P to Playback keys pressed \nKey C to cancel Playback", 10,40); // Instructions on keys to press
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (isRecording && key != 'r' && key !='R' && key != 'p' && key !='p' && key != 'c' && key !='C'){ // Recorder
		recordV.push_back(key); // Insert x key into the end of the vector recordV
	}

	if( key == '1'){
		currentMode = PARTICLE_MODE_ATTRACT;
		currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse"; 		
	}
	if( key == '2'){
		currentMode = PARTICLE_MODE_REPEL;
		currentModeStr = "2 - PARTICLE_MODE_REPEL: repels from mouse"; 				
	}
	if( key == '3'){
		currentMode = PARTICLE_MODE_NEAREST_POINTS;
		currentModeStr = "3 - PARTICLE_MODE_NEAREST_POINTS: hold 'f' to disable force";					
	}
	if( key == '4'){
		currentMode = PARTICLE_MODE_NOISE;
		currentModeStr = "4 - PARTICLE_MODE_NOISE: snow particle simulation"; 						
		resetParticles();
	}
	if( key == 'A' || key == 'a'){
		currentModeStr = "A - PARTICLE_MODE_FREEZE: stops particle movement"; 
		isPaused = !isPaused;
	}			
	if( key == ' ' ){
		resetParticles();
	}
	
	if( key == 'D' || key == 'd'){
		for(unsigned int i = 0; i < p.size(); i++){
			p[i].setScale(p[i].getScale() * 2); // Setter(Getter * 2); To get the rough value of Scale to multiply by 2.
		}
	}

	if( key == 'M' || key == 'm'){
		for(unsigned int i = 0; i < p.size(); i++){
			p[i].setScale(p[i].getScale() / 2); //Setter(Getter / 2); To get the rough value of Scale to divide by 2.
		}
	}
	if( key == 'T' || key == 't'){ //double velocity 
	for(unsigned int i = 0; i < p.size(); i++){
			p[i].vel /= (0.5);		
		}
	}

	if( key == 'S' || key == 's'){ // half velocity 
	for(unsigned int i = 0; i < p.size(); i++){
			p[i].vel /= 2;
		}

	}

	if (key == 'R'||key=='r' ){ // Starts recording each key pressed
		isReplaying = false;
		if (isRecording == false){
			isRecording = !isRecording;
			currentModeStr = "KEYS ARE BEING RECORDED - Press R / r in order to stop Recording";
		}
		else {
			isRecording = !isRecording;
			currentModeStr = "Done Recording - Do whatever you want!";
		}
	}

	if (key == 'P'||key=='p'){ // Starts playback of each key that was pressed during record
		time = 0; // Index resetters for playback
		keypress = 0;
		isReplaying = !isReplaying;
		currentModeStr = "PLAYBACK STARTING - Press C / c in order to cancel Playback or P / p two times to play again";
		isRecording = false;
		
	}

	if (key == 'C' || key == 'c'){ // Cancel the playback when C/c is pressed
	 	isReplaying = false;
		currentModeStr = "Playback Cancelled - Do whatever you want!";
		recordV.clear();
	}
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
