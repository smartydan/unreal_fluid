/***************************************************************
 * Copyright (C) 2023
 *    HSE SPb (Higher school of economics in Saint-Petersburg).
 ***************************************************************/

/* PROJECT   : UnrealFluidPhysics
 * AUTHORS   : Serkov Alexander, Daniil Vikulov, Daniil Martsenyuk, Vasily Lebedev
 * FILE NAME : FluidContainer.h
 * PURPOSE   : ${PURPOSE}
 *
 * No part of this file may be changed and used without agreement of
 * authors of this pressureSolving.
 */

#pragma once

#include <unordered_map>
#include "../../PhysicsHeaders.h"
#include "../IFluidContainer.h"
#include "CellsDistribution.h"

///Fluid1Container - Fluid simulating class, which implements ASS collision
namespace unreal_fluid::fluid {
  class Fluid1Container : IFluidContainer {
  private:
    double k;

  public:
    void simulate(double dt) override;

  private:
    unreal_fluid::physics::PhysObject::Type getType() override;
    void * getData() override;
    void collide(Particle &p1, Particle &p2);
    void advect(double dt);
    void addExternalForces(double dt);
    void interact(double dt);
  };
} // namespace unreal_fluid::fluid

// end of FluidContainer.h