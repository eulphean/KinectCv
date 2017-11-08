#include "ParticleSystem.h"

void ParticleSystem::updateWithPoly(ofPolyline poly) {
    for (int i = 0; i < Max_Particles; i++)
    {
      makeNewParticle(ofRandom(ofGetWidth()), 0);
    }

    ofRectangle screenRect(0, 0, ofGetWidth(), ofGetHeight());
  
    glm::vec3 mouse(ofGetMouseX(), ofGetMouseY(), 0);

    // Get an iterator that points to the first element in our vector.
    auto iter = particles.begin();
  
    // Iterate through all particles.
    while (iter != particles.end())
    {
        // We want to accelerate downwards.
        iter->acceleration.y = 0.1;
      
        float distance, forceStrength;
        glm::vec3 forceDirection;
      
        if (poly.getVertices().size() > 0) {
          // Change the force direction of the particle if it's inside the polyline.
          if (poly.inside(iter->position)) {
            glm::vec3 closestPoint = poly.getClosestPoint(iter->position);
            
            distance = glm::distance(closestPoint, iter->position);
            forceDirection = glm::normalize(iter->position - closestPoint) * -1;
            forceStrength = ofMap(distance, 0, 10, 0, 5, true);
          }
        } else { // Use mouse pointer if there is no polyline.
          distance = glm::distance(mouse, iter->position);
          forceDirection = glm::normalize(mouse - iter->position) * -1;
          forceStrength = ofMap(distance, 0, 200, 0.1, 0, true);
        }
      
        // Update force.
        iter->force = forceDirection * forceStrength;
      
        // Updating the particle properties. 
        iter->update();

        // Is the current particle on the screen?
        if (screenRect.inside(iter->position) == false)
        {
            iter = particles.erase(iter);
        }
        else
        {
            iter++;
        }
    }
}

void ParticleSystem::draw() {
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    for (Particle& p: particles)
    {
        p.draw();
    }

    ofSetColor(255);
    ofDrawBitmapString("Num Particles: " + ofToString(particles.size()), 14, 14);
}

void ParticleSystem::makeNewParticle(float x, float y)
{
    Particle p;
    // Set initial positions.
    p.position.x = x;
    p.position.y = y;
    p.position.z = 0;

    // Set initial velocities.
    p.velocity.x = ofRandom(-2, 2);
    p.velocity.y = ofRandom(-5, 5);
    p.velocity.z = 0;

    // Set inital accelerations.
    p.acceleration.x = 0;
    p.acceleration.y = 0.1;
    p.acceleration.z = 0;

    // Set angular velocity.
    p.angularVelocity.x = 0;
    p.angularVelocity.y = 0;
    p.angularVelocity.z = ofRandom(-2, 2);

    // Set color.
    p.color = ofColor(ofRandom(180, 200), 100);

    // Add a copy to our vector.
    particles.push_back(p);

}
