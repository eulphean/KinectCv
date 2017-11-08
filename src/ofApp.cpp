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
  
      ofPoint center;
      center.x = ofGetWidth()/2 - vidPlayer.getWidth()/2;
      center.y = ofGetHeight()/2 - vidPlayer.getHeight()/2;
  
      // Anchor the video to the center.
      vidPlayer.setAnchorPoint(center.x, center.y);
  
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
    // Clear the polyline.
    newPoly.clear();

    // Set contourFinder with sliders to get the right value for contour finding.
    contourFinder.setMinAreaRadius(minArea);
    contourFinder.setMaxAreaRadius(maxArea);
    contourFinder.setThreshold(threshold);

    // Depth image matrix that we will pass to Contour finder.
    Mat depthImgMat;
  
    float widthOffset, heightOffset;
  
    #ifdef _USE_VIDEO
  
      // Video player related logic.
      vidPlayer.update();
      // Do video related things.
      if (vidPlayer.isFrameNew()) {
        ofPixels depthPixels = vidPlayer.getPixels();
        texDepth.loadData(depthPixels);
        depthImgMat = toCv(depthPixels);
        
        // Offset distances to center the polyline.
        widthOffset = ofGetWidth()/2 - depthPixels.getWidth()/2;
        heightOffset = ofGetHeight()/2 - depthPixels.getHeight()/2;
        
        // Find contours.
        contourFinder.findContours(depthImgMat);
      }
  
    #else
  
      // Kinect related logic.
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
            
            // Offset distances to center the polyline.
            widthOffset = ofGetWidth()/2 - depthPixels.getWidth()/2;
            heightOffset = ofGetHeight()/2 - depthPixrls.getHeight()/2;
            
            // Find contours.
            contourFinder.findContours(depthImgMat);
          }
      }
  
    #endif
  
    vector <ofPolyline> polylines = contourFinder.getPolylines();
  
    // Translate algorithm for each vertex of the polyline.
    if (polylines.size() > 0) {
      vector<glm::vec3> vertices = polylines[0].getVertices();
      
      // Add the offset distances to each vertex of the polyline to
      // center it in the screen.
      for (int i = 0; i < vertices.size(); i++) {
        vertices[i].x += widthOffset;
        vertices[i].y += heightOffset;
      }
      
      // Create a new polyline with updated vertices.
      newPoly.addVertices(vertices);
      
      // Pass the newPoly to Particle System's update function.
      particleSystem.update(newPoly);
    } else {
      // Update Particle System.
      particleSystem.update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    // Don't translate this since all the vertices are internally
    // translated to provide to the particle system.
    if (newPoly.getVertices().size() > 0) {
      ofSetColor(ofColor::white);
      newPoly.draw();
    }
  
   // Clean way to center the texture getting captured with the Contour detected.
   // We want these to be in the center of the screen for our algorithm to work
   // smoothly.
   ofPushMatrix();
  
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
      ofPushMatrix();
  
        ofTranslate(-texDepth.getWidth()/2, -texDepth.getHeight()/2);
  
          if (showTexture) {
            // Depth texture.
            texDepth.draw(0, 0);
          }
  
      ofPopMatrix();
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

