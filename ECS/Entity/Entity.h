//
// Created by Teo on 2026-11-24.
//

#ifndef RIDICULOENGINE_ENTITY_H
#define RIDICULOENGINE_ENTITY_H
#include <bitset>
#include <cstdint>

// An Entity is just an ID
using Entity = unsigned int;
constexpr Entity MAX_ENTITIES = 5000;

using ComponentType = uint8_t;
constexpr ComponentType MAX_COMPONENTS = 32;

// The Signature represents a bitset (modern C++ equivalent of a bitfield)
// to track which components and Entity "has".
// Components are assigned a type ID. For example, Transform could be assigned 0, RigidBody = 1, and Gravity = 2.
// An Entity that "has" those three components would have a Signature of '0b111', meaning that bits 0, 1 and 2 are set.
using Signature = std::bitset<MAX_COMPONENTS>;
#endif //RIDICULOENGINE_ENTITY_H
