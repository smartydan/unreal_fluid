/***************************************************************
 * Copyright (C) 2023
 *    HSE SPb (Higher school of economics in Saint-Petersburg).
 ***************************************************************/

/* PROJECT   : UnrealFluidPhysics
 * AUTHORS   : Serkov Alexander, Daniil Vikulov, Daniil Martsenyuk, Vasily Lebedev
 * FILE NAME : FluidContainer.cxx
 * PURPOSE   : ${PURPOSE}
 *
 * No part of this file may be changed and used without agreement of
 * authors of this pressureSolving.
 */

#include "Fluid1Container.h"

using namespace unreal_fluid::fluid;

Fluid1Container::Fluid1Container() {
  //init fluid
};

Fluid1Container::~Fluid1Container() {
  //destroy fluid
};

std::vector<Particle> *Fluid1Container::getParticles() {
  return &particles;
}

void Fluid1Container::addExternalForces(double dt) {
  for (auto & particle : particles) {
    particle.v() += physics::G*dt;
  }
}

void Fluid1Container::advect(double dt) {
  for (auto & particle : particles) {
    particle.c() += particle.v()* dt;
  }
}

void Fluid1Container::interact(double dt) {
  unreal_fluid::fluid::CellsDistribution cellsDistribution(particles);
  auto p = cellsDistribution.nextPair();
  while (p.first != nullptr || p.second != nullptr) {
    auto &p1 = *p.first;
    auto &p2 = *p.second;
    if ((p1.c() - p2.c()).len() <= p1.r() + p2.r()) {
      collide(p1, p2);
    }
  }
}

void Fluid1Container::simulate(double dt) {
  interact(dt);
  addExternalForces(dt);
  advect(dt);
}

void Fluid1Container::collide(Particle &p1, Particle &p2) {
  auto y = p2.c() - p1.c();
  {
    double push = (p1.r() + p2.r() - p1.c().distanceTo(p2.c())) / 2;
    p2.c() += y * push;
    p1.c() -= y * push;
  }
  {
    double k = 1;//TODO добавить
    double s = -(1 + k) * (p1.v().project(y) - p2.v().project(y)) * p1.m() * p2.m() / (p1.m() + p2.m());
    p1.v() += y * (s / p1.m());
    p2.v() -= y * (s / p2.m());
  }
}
