#include "ofApp.h"

ofMesh mesh;
ofEasyCam cam;
ofShader effectSrc;
ofShader effectDst;
ofFbo fboSrc;
ofFbo fboDst;

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableAlphaBlending();
    
    srcImg.loadImage("A.jpg");
    brushImg.loadImage("brush.png");
    
    int width = ofGetScreenWidth();
    int height = ofGetScreenHeight();
    
    maskFbo.allocate(width,height);
    fbo.allocate(width,height);
    fboSrc.allocate(width,height);
    fboDst.allocate(width,height);


    string shaderProgram = "#version 120\n \
		#extension GL_ARB_texture_rectangle : enable\n \
		\
		uniform sampler2DRect tex0;\
		uniform sampler2DRect maskTex;\
		\
		void main (void){\
		vec2 pos = gl_TexCoord[0].st;\
		\
		vec3 src = texture2DRect(tex0, pos).rgb;\
		float mask = texture2DRect(maskTex, pos).r;\
		\
		gl_FragColor = vec4( src , mask);\
		}";
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
    shader.linkProgram();

    effectSrc.load("material.vert","material.frag");
    effectDst.load("material.vert","material5.frag");

    maskFbo.begin();
    ofClear(0,0,0,255);
    maskFbo.end();
    
    fbo.begin();
    ofClear(0,0,0,255);
    fbo.end();
    
    mesh.load("mesh.ply");
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    float scale = ofGetHeight() / 2;

    fboDst.begin();
	ofClear(0,0,0,255);
	cam.begin();
	ofEnableDepthTest();
        	effectDst.begin();
                effectDst.setUniform1f("time", ofGetElapsedTimef());
       	        ofScale(scale, scale, scale);
	        mesh.draw();
	        effectDst.end();
	ofDisableDepthTest();
        cam.end();
    fboDst.end();

    fboSrc.begin();
	ofClear(0,0,0,0);
	cam.begin();
	ofEnableDepthTest();
        	effectSrc.begin();
		//effectSrc.setUniformTexture("texture", srcImg, 1);
                //effectSrc.setUniformTexture("texture1", srcImg, 2);
                effectSrc.setUniform1f("time", ofGetElapsedTimef());
	        ofScale(scale, scale, scale);
	        mesh.draw();
	        effectSrc.end();
	ofDisableDepthTest();
        cam.end();
    fboSrc.end();


    maskFbo.begin();
        brushImg.draw(mouseX-25,mouseY-25,50,50);
    maskFbo.end();
    
    fbo.begin();
    ofClear(0, 0, 0, 0); 
    
    shader.begin();
    shader.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1 );
    
    fboSrc.draw(0,0);

    shader.end();
    fbo.end();

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255,255);
    
    fboDst.draw(0,0);
    //fboSrc.draw(0,0);

    
    fbo.draw(0,0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == 'c'){
	    maskFbo.begin();
	    ofClear(0,0,0,255);
	    maskFbo.end();
	}
	if(key == 'f'){
		ofToggleFullscreen();
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
