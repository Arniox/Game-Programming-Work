#include "Material.h"

Material::Material()
: dead(true)
, isKinematic(true)
, isPhase(false)
, movementSpeedLimit(0.0)
, torque(0.0)
, mass(0.0)
, momentOfIntertia(0.0)
, restitution(0.0)
, inAir(true)
{
}
Material::~Material()
{
}
