/***************************************************************
 * Copyright (C) 2023
 *    HSE SPb (Higher school of economics in Saint-Petersburg).
 ***************************************************************/

/* PROJECT   : ultimate_py_project
 * AUTHORS   : Serkov Alexander, Daniil Vikulov, Daniil Martsenyuk, Vasily Lebedev
 * FILE NAME : CellsDistribution.cxx
 * PURPOSE   : distribute particles
 *
 * No part of this file may be changed and used without agreement of
 * authors of this project.
 */

#include "CellsDistribution.h"

using namespace unreal_fluid::physics::fluid;

std::pair<Particle *, Particle *> CellsDistribution::nextPair() {
  while (cell.size() < 2 && current_cell < cells.size()) {
    current_cell++;

    cell = cells[cells_keys[current_cell]];
  }

  if (current_cell == cells.size())
    return {nullptr, nullptr};

  if (second >= cell.size()) {
    first++;
    second = first + 1;
  }

  while (first >= cell.size() - 1 && current_cell < cells.size()) {
    current_cell++;
    first = 0;
    second = 1;

    cell = cells[cells_keys[current_cell]];
  }

  if (current_cell == cells.size())
    return {nullptr, nullptr};

  return {cell[first], cell[second++]};
}

CellsDistribution::CellsDistribution(std::vector<Particle*> &particles) {
  uint64_t id;
  auto dx = 3 * particles.front()->radius; /// TODO what should these constants be?
  for (auto &particle: particles) {
    ++counter;
    auto [x, y, z] = particle->position;
    x /= dx;
    y /= dx;
    z /= dx;
    id = ((uint64_t) x << 32) + ((uint64_t) y << 16) + (uint64_t) z;
    cells[id].push_back(particle);
  }
  int k = 0;
  for (auto const& [key, value]: cells) {
    cells_keys[k] = key;
    ++k;
  }
  assert(k > 0);
  cell = cells[cells_keys[0]];
  current_cell = 0;
  first = 0;
  second = 1;
}

// end of CellsDistribution.cxx
