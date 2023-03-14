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

#include <vector>

#include "Particle.h"

namespace unreal_fluid::fluid {
  class IFluidContainer {

  protected:
    std::vector<Particle> particles;
  public:
    IFluidContainer() = default;
    virtual ~IFluidContainer() = default;
    virtual void simulate(double dt) = 0;
    virtual std::vector<Particle> *getParticles() = 0;
  };
} // namespace unreal_fluid::fluid
