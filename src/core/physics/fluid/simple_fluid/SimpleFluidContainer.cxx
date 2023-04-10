/***************************************************************
 * Copyright (C) 2023
 *    HSE SPb (Higher school of economics in Saint-Petersburg).
 ***************************************************************/

/* PROJECT   : UnrealFluidPhysics
 * AUTHORS   : Serkov Alexander, Daniil Vikulov, Daniil Martsenyuk, Vasily Lebedev
 * FILE NAME : SimpleFluidContainer.cxx
 * PURPOSE   : Simple fluid simulation class, which implements ASS collision
 *
 * No part of this file may be changed and used without agreement of
 * authors of this project.
 */

#include <random>

#include "SimpleFluidContainer.h"
#include "../CollisionPairs.h"

using namespace unreal_fluid::physics::fluid;

SimpleFluidContainer::SimpleFluidContainer(FluidDescriptor descriptor) : distributor(*new CellsDistributor())  {
  k = 0.1;
  // addParticle({0.01, -0.01, -0.01}, {0, 0, 0}, 0.3, 1000000);
  /// TODO : write constructor implementation
}

SimpleFluidContainer::~SimpleFluidContainer() {
    for (auto &particle: particles) {
        delete particle;
    }
}

void SimpleFluidContainer::addExternalForces(double dt) {
    for (auto &particle: particles) {
        particle->velocity += G * dt;
    }
}

void SimpleFluidContainer::advect(double dt) {
    for (auto particle: particles) {
        particle->position += particle->velocity * dt;
    }
    {
        for (auto p : particles) {
            double yMin = -1;
            if (p->position.y - p->radius < yMin) {
                double push = yMin - p->position.y + p->radius;
                p->position.y += push;
                p->velocity.y = -k*p->velocity.y;
            }
        }
    }//TODO this is the temporary measure to prevent particles from falling down. Solids should be used
}

void SimpleFluidContainer::interact() {
  distributor.update(particles);
  std::pair<Particle *, Particle *> p = distributor.nextPair();

  for (auto bigParticle: distributor.getBigParticles()) {
    for (auto particle: particles) {
      if (particle->position != bigParticle->position && (particle->position - bigParticle->position).len() <= particle->radius + bigParticle->radius)
        collide(particle, bigParticle);
    }
  }

  while (p != CellsDistributor::terminator) {
    collide(p.first, p.second);
    p = distributor.nextPair();
  }
}

void SimpleFluidContainer::simulate(double dt) {
    for (int i = 0; i < 10; ++i) {
        addParticle({double(rand() % 100) / 100000, 1, double(rand() % 100) / 100000}, {0, 0, 0}, 0.02, 1);
        //addParticle({1, double(rand() % 100) / 100000, double(rand() % 100) / 100000}, {-0.5, 0, 0}, 0.03, 2);
    }
    interact();
    addExternalForces(dt);
    advect(dt);
}

void SimpleFluidContainer::collide(Particle *p1, Particle *p2) const {
  vec3 positionDiff = p1->position - p2->position;

  if (positionDiff.len2() == 0) return;

  double distance = positionDiff.len();
  vec3 positionDiffDirection = positionDiff / distance;

  double pushValue =
          (p1->radius + p2->radius - distance) /
          (p1->mass + p2->mass);

  if (pushValue < 0) return;

  vec3 pushVector = positionDiffDirection * pushValue;

  p1->position += pushVector * p2->mass;
  p2->position -= pushVector * p1->mass;

  double momentumValue =
          (1 + k) *
          (p1->velocity.dot(positionDiffDirection) - p2->velocity.dot(positionDiffDirection)) /
          (p1->mass + p2->mass);
  vec3 momentum = positionDiffDirection * momentumValue;

  p1->velocity -= momentum * p2->mass;
  p2->velocity += momentum * p1->mass;
}

void *SimpleFluidContainer::getData() {
    return &particles;
}

unreal_fluid::physics::IPhysicalObject::Type SimpleFluidContainer::getType() {
    return physics::IPhysicalObject::Type::SIMPLE_FLUID_CONTAINER;
}

void SimpleFluidContainer::addParticle(vec3 position, vec3 velocity, double radius, double mass) {
    auto particle = new Particle;
    particle->position = position;
    particle->mass = mass;
    particle->radius = radius;
    particle->velocity = velocity;
    particles.push_back(particle);
}

// end of FluidContainer.cxx
