/***************************************************************
* Copyright (C) 2023
*    HSE SPb (Higher school of economics in Saint-Petersburg).
***************************************************************/

/* PROJECT   : ultimate_py_project
* AUTHORS   : Serkov Alexander, Daniil Vikulov, Daniil Martsenyuk, Vasily Lebedev
* FILE NAME : InteractionSolver.cxx
* PURPOSE   : solve interactions
*
* No part of this file may be changed and used without agreement of
* authors of this project.
*/

#include "InteractionSolver.h"

using namespace unreal_fluid::physics::fluid;
using unreal_fluid::physics::CollisionSolver;

unreal_fluid::physics::fluid::InteractionSolver::InteractionSolver() {
  coreNumber = std::thread::hardware_concurrency();
  assert(coreNumber > 0);
  distributors.resize(coreNumber);
}

void InteractionSolver::forces(Particle *p1, Particle *p2) {
}

void InteractionSolver::task(unreal_fluid::physics::fluid::CellsDistributor &distributor, std::vector<Particle *> &particles, double k, bool *ready) {
  distributor.update(particles);
  for (auto &bigParticle: distributor.big_particles) {
    for (auto &particle: particles) {
      double dist = (particle->position - bigParticle->position).len();
      //      if (dist > particle->R + bigParticle->R) continue;
      //      if (dist > particle->radius + bigParticle->radius) {
      //        f(particle, bigParticle); /// TODO
      //      } else
      if (dist <= particle->radius + bigParticle->radius)
        CollisionSolver::particleWithParticleCollision(particle, bigParticle, k);
    }
  }

  for (auto p = distributor.nextPair(); p != CellsDistributor::terminator; p = distributor.nextPair()) {
    double dist = (p.first->position - p.second->position).len();
    //    if (dist > p.first->R + p.second->R) continue;
    //    if (dist > p.first->radius + p.second->radius) {
    //      //        f(particle, bigParticle); /// TODO
    //    } else
    if (dist <= p.first->radius + p.second->radius)
      CollisionSolver::particleWithParticleCollision(p.first, p.second, k);
  }
  *ready = true;
}

void InteractionSolver::interact(std::vector<Particle *> &particles, double k) {
  if (particles.size() > 100) {
    std::vector<std::vector<Particle *>> subParticles(coreNumber);
    std::vector<bool *> threadsFinished(coreNumber);

    for (int pos = 0; pos < coreNumber; ++pos) {
      threadsFinished[pos] = new bool;
      *threadsFinished[pos] = false;
    }

    for (auto &particle : particles) {
      int id = abs(int(particle->position.x * particle->velocity.y)) * 239 % coreNumber;
      subParticles[id].push_back(particle);
    }

    //    size_t beginIndex, endIndex = 0;
    //
    //    for (int i = 0; i < coreNumber; ++i) {
    //      beginIndex = endIndex;
    //      endIndex = (i + 1) * particles.size() / coreNumber;
    //      endIndex = std::min(endIndex, particles.size());
    //      for (size_t j = beginIndex; j < endIndex; ++j)
    //        subParticles[i].push_back(particles[j]);
    //    }

    //LOG_INFO("Detaching");
    for (int i = 0; i < coreNumber; ++i) {
      bool *ready = threadsFinished[i];
      interactionTasks.emplace_back(task, std::ref(distributors[i]), std::ref(subParticles[i]), k, ready);
      interactionTasks.back().detach();
    }

    bool everybodyFinished = false;
    //LOG_INFO("Started");
    while (!everybodyFinished) {
      everybodyFinished = true;
      for (int i = 0; i < coreNumber; ++i)
        everybodyFinished &= *threadsFinished[i];
    }
    //LOG_INFO("Joined");
  } else {
      LOG_FATAL(particles.size());
    bool *ready = new bool;
    *ready = false;
    task(distributors[0], particles, k, ready);
  }
}
