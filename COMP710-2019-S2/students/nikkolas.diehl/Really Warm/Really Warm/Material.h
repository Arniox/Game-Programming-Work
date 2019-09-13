#pragma once
class Material
{
public:
	Material();
	~Material();
public:
	//All variables public becuase the material class itself
	//Will be private with a get method for easier access
	
	//Control variables
	bool dead;
	bool isKinematic;
	bool isPhase; //Does this entity phase through everything else?
	bool inAir;	  //Is the given character in air

	//Material variables
	double movementSpeedLimit;
	double torque;
	double mass;
	double momentOfIntertia;
	double restitution; //elasticicity
};

