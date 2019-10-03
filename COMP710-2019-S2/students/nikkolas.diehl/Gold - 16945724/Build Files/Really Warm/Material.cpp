#define _CRTDBG_MAP_ALLOC
#include "Material.h"

Material::Material()
: dead(true)
, doNotProcess(false)
, isPhase(false)
, constrained(true)
, movementSpeedLimit(0.0)
, torque(0.0)
, mass(0.0)
, momentOfIntertia(0.0)
, restitution(0.0)
, inAir(true)
, TYPE(NOTHING)
, ID(0)
{
}
Material::~Material()
{
}
