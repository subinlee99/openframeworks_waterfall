#include "ofApp.h"
#define MAXNUM 100

int **dots;
int **lines;
int select_x, select_y;
int select_d=0;
int w,h;
//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(15); // Limit the speed of our program to 15 frames per second
    
    // We still want to draw on a black background, so we need to draw
    // the background before we do anything with the brush
    ofBackground(255,255,255);
    ofSetLineWidth(4);
    w = ofGetWidth();
    h= ofGetHeight();
    
    draw_flag = 0;
    load_flag = 0;
    dot_diameter = 20.0f;
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(127,23,31);  // Set the drawing color to brown
    
    // Draw shapes for ceiling and floor
    ofDrawRectangle(0, 0, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
    ofDrawRectangle(0, 728, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
    ofSetLineWidth(5);

    if(draw_flag){
        /* COMSIL1-TODO 3 : Draw the line segment and dot in which water starts to flow in the screen.
         Note that after drawing line segment and dot, you have to make selected water start dot colored in red.
         */
        for(int i=0;i<num_of_line;i++) ofDrawLine(lines[i][0],lines[i][1],lines[i][2],lines[i][3]);
        
        for(int j=0;j<num_of_dot;j++){
            if(j == select_d) ofSetColor(255,0,0);
            else ofSetColor(0);
            ofDrawCircle(dots[j][0],dots[j][1],10);
        }
        // 2nd week portion.
        ofSetLineWidth(2);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'v') {
        // HACK: only needed on windows, when using ofSetAutoBackground(false)
        glReadBuffer(GL_FRONT);
        ofSaveScreen("savedScreenshot_"+ofGetTimestampString()+".png");
    }
    if (key == 'q'){
        // Reset flags
        draw_flag = 0;
        for(int i=0;i<num_of_line;i++){
            delete lines[i];
        }
        delete (lines);
        cout << "Memory for line segement has been freed.\n" ;
        
        for(int i=0;i<num_of_dot;i++){
            delete(dots[i]);
        }
        delete (dots);
        // Free the dynamically allocated memory exits.
        cout << "Memory for dot has been freed.\n" ;
        _Exit(0);
    }
    if (key == 'd'){
        if( !load_flag) return;
        draw_flag = 1;
        
        /* COMSIL1-TODO 2: This is draw control part.
        You should draw only after when the key 'd' has been pressed.
        */
    }
    if (key == 's'){
        // 2nd week portion.
    }
    if (key == 'e'){
        // 2nd week portion.
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if( key == 'l'){
        // Open the Open File Dialog
        ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a only txt for Waterfall");
        
        // Check whether the user opened a file
        if( openFileResult.bSuccess){
            ofLogVerbose("User selected a file");
            
            // We have a file, so let's check it and process it
            processOpenFileSelection(openFileResult);
            load_flag = 1;
        }
    }
    
    /* COMSIL1-TODO 4: This is selection dot control part.
     You can select dot in which water starts to flow by left, right direction key (<- , ->).
     */
    
    if (key == OF_KEY_RIGHT){
        select_d ++;
        if(select_d>= num_of_dot)select_d=0;
        cout << "Selcted Dot Coordinate is (" << dots[select_d][0] << ", "<<  dots[select_d][1] << ")\n";
    }
    else if (key == OF_KEY_LEFT){
        select_d --;
        if(select_d < 0)select_d= (num_of_dot-1);
        cout <<"Selcted Dot Coordinate is (" << dots[select_d][0]<<", " <<dots[select_d][1] << ")\n";
    }
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

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) { 
    //Path to the comma delimited file
    //string fileName = "input.txt";
    
    string fileName = openFileResult.getName();
    ofFile file(fileName);
    
    if( !file.exists()) {
        cout << "Target file does not exists." << endl;
        _Exit(0);
    }
    else cout << "We found the target file." << endl;
    
    ofBuffer buffer(file);
    
    /* This variable is for indicating which type of input is being received.
     IF input_type == 0, then work of getting line input is in progress.
     IF input_type == 1, then work of getting dot input is in progress.
     */
    int input_type = 0;
    
    /* COMSIL1-TODO 1 : Below code is for getting the number of line and dot, getting coordinates.
     You must maintain those information. But, currently below code is not complete.
     Also, note that all of coordinate should not be out of screen size.
     However, all of coordinate do not always turn out to be the case.
     So, You have to develop some error handling code that can detect whether coordinate is out of screen size.
    */
    
    int count_l = 0; int count_d =0;
    // Read file line by line
    for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
        string line = *it;
        // Split line into strings
        vector<string> words = ofSplitString(line, " ");
        
        if( words.size()==1){
            if(input_type==0){ // Input for the number of lines.
                num_of_line=atoi(words[0].c_str());
                cout << "The number of line is: " << num_of_line << endl;
                lines = new int*[num_of_line];
            }
            else{ // Input for the number of dots.
                num_of_dot = atoi(words[0].c_str());
                cout << "The number of dot is: "<< num_of_dot <<endl;
                dots = new int*[num_of_dot];
            }
        }
        else{
            if(input_type==0){ // Input for actual information of lines
                int coor[4];
                lines[count_l]=new int[4];
                for(int i=0;i<4;i++){
                    coor[i]=atoi(words[i].c_str());
                    if(i%2==0){
                        if(coor[i]>w or coor[i]<0)_Exit(0);
                    }
                    else{
                        if(coor[i]>h or coor[i]<0)_Exit(0);
                    }
                    lines[count_l][i] = coor[i];
                }
                count_l++;
                if(count_l== num_of_line) {
                    input_type =1;
                }
            }
            
            else{ // Input for actual information of dots.
                dots[count_d]=new int[2];
                for(int i=0;i<2;i++){
                    dots[count_d][0] = atoi(words[0].c_str());
                    dots[count_d][1] = atoi(words[1].c_str());
                    if( dots[count_d][0]>w or  dots[count_d][0]<0)_Exit(0);
                    else if(dots[count_d][1] >h or dots[count_d][1]<0)_Exit(0);
                }
    
                count_d++;

            }
        }
    }
    //initializeWaterLines();
}

void ofApp::initializeWaterLines() {
    
}


