#pragma once

#include "ofMain.h"
#include "Particle.h"

using namespace std;

class ParticleSystem
{
public:
  void update();
  // Overloaded update() with a polyline passed for the contour.
  void update(ofPolyline poly);
  
  void draw();
  void setBoundingRectangles();
  
private:
  const int Max_Particles = 30;
  void makeNewParticle(float x, float y);
  vector<Particle> particles;
};
