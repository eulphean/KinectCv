#pragma once

#include "ofMain.h"
#include "ofxKinectV2.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "ParticleSystem.h"

using namespace std;

// This flag uses a pre-stored background subtracted
// video. Comment this line to use the Kinect, which is
// connected.
#define _USE_VIDEO

class ofApp : public ofBaseApp{

public:
  void setup();
  void update();
  void draw();

  void keyPressed(int key);

  // Application GUI.
  ofxPanel gui;

  // Kinect Gui group.
  ofxGuiGroup kinectGroup;

  // Kinect.
  ofxKinectV2 * kinect;
  ofTexture texDepth;
  ofTexture texRGB;

  // Contour Finder.
  ofxCv::ContourFinder contourFinder;

  // OpenCV UI parameters. 
  ofxGuiGroup cvGroup;
  ofxFloatSlider minArea, maxArea, threshold;

  // Particle System
  ParticleSystem particleSystem;

private:
  bool showTexture = false;
  #ifdef _USE_VIDEO
  ofVideoPlayer 		vidPlayer;
  #endif
  
  ofPolyline newPoly;
};
