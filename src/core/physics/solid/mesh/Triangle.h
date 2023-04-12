/***************************************************************
 * Copyright (C) 2023
 *    HSE SPb (Higher school of economics in Saint-Petersburg).
 ***************************************************************/

/* PROJECT   : UnrealFluidPhysics
 * AUTHORS   : Serkov Alexander, Daniil Vikulov, Daniil Martsenyuk, Vasily Lebedev
 * FILE NAME : Triangle.h
 * PURPOSE   : triangle class used in mesh
 *
 * No part of this file may be changed and used without agreement of
 * authors of this project.
 */
#pragma once

#include "../../../../utils/math/MathHeaders"

namespace unreal_fluid::physics::solid {
    struct Triangle {
        vec3f v1, v2, v3;

        Triangle(vec3f v1, vec3f v2, vec3f v3);
        ~Triangle() = default;


    };
}