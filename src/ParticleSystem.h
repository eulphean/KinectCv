#pragma once

#include "ofMain.h"
#include "Particle.h"

using namespace std;

class ParticleSystem
{
public:
  void update();
  void draw();
  
private:
  void makeNewParticle(float x, float y);
  vector<Particle> particles;
};
