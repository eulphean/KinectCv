#include "ofApp.h"

using namespace cv;
using namespace ofxCv;

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(30, 30, 30);

    // Check if we have a Kinect device connected.
    ofxKinectV2 tmp;
    vector <ofxKinectV2::KinectDeviceInfo> deviceList = tmp.getDeviceList();
  
    if (deviceList.size() > 0) {
      cout << "Success: Kinect found.";
    } else {
      cout << "Failure: Kinect not found.";
      return;
    }
  
    // GUI setup.
    gui.setup();
  
    kinectGroup.setup("Kinect", "settings.xml", 10, 100);
  
    // Setup Kinect. [Assumption] Only a single Kinect will be
    // connected to the system.
    kinect = new ofxKinectV2();
    kinect->open(deviceList[0].serial);
    kinectGroup.add(kinect->params);
  
    kinectGroup.loadFromFile("settings.xml");
  
    // openCv GUI.
    cvGroup.setup("openCv");
    cvGroup.add(minArea.setup("Min area", 10, 1, 100));
    cvGroup.add(maxArea.setup("Max area", 200, 1, 500));
    cvGroup.add(threshold.setup("Threshold", 128, 0, 255));
  
    // Add the groups to main GUI.
    gui.add(&cvGroup);
    gui.add(&kinectGroup);
}

//--------------------------------------------------------------
void ofApp::update(){
    particleSystem.update();
  
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
      Mat depthImgMat = toCv(depthPixels);
      
      // Tweak the countours with sliders to get the right value for the setup.
      contourFinder.findContours(depthImgMat);
      contourFinder.setMinAreaRadius(minArea);
      contourFinder.setMaxAreaRadius(maxArea);
      contourFinder.setThreshold(threshold);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Depth texture.
    texDepth.draw(0, 0);
  
    // Contours detected.
    contourFinder.draw();
  
    // Draw the particle system.
    particleSystem.draw();
  
    // Draw GUI.
    gui.draw();
}

