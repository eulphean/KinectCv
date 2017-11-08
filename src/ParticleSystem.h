#pragma once

#include "ofMain.h"
#include "Particle.h"

using namespace std;

class ParticleSystem
{
public:
  void updateWithPoly(ofPolyline poly);
  
  void draw();
  void setBoundingRectangles();
  
private:
  const int Max_Particles = 30;
  void makeNewParticle(float x, float y);
  vector<Particle> particles;
};
