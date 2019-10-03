#ifndef __MATERIAL_H__
#define __MATERIAL_H__

class Material
{
public:
	Material();
	~Material();
public:
	//All variables public becuase the material class itself
	//Will be private with a get method for easier access
	
	//Entity type controls
	enum EntityType {
		WALL,
		PLAYER,
		ENEMY,
		WEAPON,
		BULLET,
		PARTICLE,
		RAN_OBJECT,
		NOTHING
	};
	EntityType TYPE;

	//Control variables
	bool dead;
	bool doNotProcess;
	bool isPhase;
	bool constrained;
	bool inAir;	  //Is the given character in air
	unsigned int ID;

	//Material variables
	double movementSpeedLimit;
	double torque;
	double mass;
	double momentOfIntertia;
	double restitution; //elasticicity
};

#endif

