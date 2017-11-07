#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(ofColor::black);
    ofSetVerticalSync(true);
  
    #ifdef _USE_VIDEO
  
      // Load a video.
      vidPlayer.load("fingers.mov");
      vidPlayer.play();
      vidPlayer.setLoopState(OF_LOOP_NORMAL);
  
    #else
  
      // Check if we have a Kinect device connected.
      ofxKinectV2 tmp;
      vector <ofxKinectV2::KinectDeviceInfo> deviceList = tmp.getDeviceList();
    
      if (deviceList.size() > 0) {
        cout << "Success: Kinect found.";
      } else {
        cout << "Failure: Kinect not found.";
        return;
      }
    
      kinectGroup.setup("Kinect", "settings.xml", 10, 100);
    
      // Setup Kinect. [Assumption] Only a single Kinect will be
      // connected to the system.
      kinect = new ofxKinectV2();
      kinect->open(deviceList[0].serial);
      kinectGroup.add(kinect->params);
    
      kinectGroup.loadFromFile("settings.xml");
      gui.add(&kinectGroup);
  
    #endif
  
    // GUI setup.
    gui.setup();
  
    // openCv GUI.
    cvGroup.setup("openCv");
    cvGroup.add(minArea.setup("Min area", 10, 1, 100));
    cvGroup.add(maxArea.setup("Max area", 200, 1, 500));
    cvGroup.add(threshold.setup("Threshold", 128, 0, 255));
  
    // Add the groups to main GUI.
    gui.add(&cvGroup);
}

//--------------------------------------------------------------
void ofApp::update(){
    particleSystem.update();
  
    // Depth image matrix that we will pass to Contour finder.
    Mat depthImgMat;
  
    #ifdef _USE_VIDEO
  
      vidPlayer.update();
      // Do video related things.
      if (vidPlayer.isFrameNew()) {
        ofPixels depthPixels = vidPlayer.getPixels();
        depthImgMat = toCv(depthPixels);
        
        // Find contours.
        contourFinder.findContours(depthImgMat);
      }
  
    #else
  
    // Do Kinect related things.
      if (kinect != nullptr) {
          // Update Kinect.
          kinect->update();
          // Only do work when Kinect captures a new frame.
          if (kinect->isFrameNew()) {
            // Depth and RGB image from the Kinect.
            // TODO: If not using RGB image, comment that out.
            texDepth.loadData(kinect->getDepthPixels());
            texRGB.loadData(kinect->getRgbPixels());
            
            // Depth pixels.
            ofPixels depthPixels = kinect->getDepthPixels();
            // Convert depth pixels into OpenGL Mat data type. This is required for
            // ofxCv to process the depth pixels and find countours.
            depthImgMat = toCv(depthPixels);
            
            // Find contours.
            contourFinder.findContours(depthImgMat);
            
            // Get the bounding rectangles. 
            //vector<Rect> boundingRect = contourFinder.getBoundingRects();
            
          }
      }
  
    #endif
  
    // Tweak the countours with sliders to get the right value for the setup.
    contourFinder.setMinAreaRadius(minArea);
    contourFinder.setMaxAreaRadius(maxArea);
    contourFinder.setThreshold(threshold);
}

//--------------------------------------------------------------
void ofApp::draw(){
   ofPushMatrix();
      ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
      /*if (showTexture) {
        // Depth texture.
        texDepth.draw(0, 0);
      }*/
    
      // Contours detected.
      contourFinder.draw();
    ofPopMatrix();
  
    // Draw the particle system.
    particleSystem.draw();
  
    // Draw GUI.
    gui.draw();
}

void ofApp::keyPressed(int key) {
  switch (key) {
    case 49: {
      showTexture = !showTexture;
      break;
    }
    
    default: {
      break;
    }
  }
}

