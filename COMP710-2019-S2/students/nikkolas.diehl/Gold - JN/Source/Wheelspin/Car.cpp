#include "Car.h"

#include "Wheel.h"
#include "Calc.h"
#include "backbuffer.h"
#include "sprite.h"
#include "game.h"
#include "IniParser.h"
#include "ParticleEmitter.h"
#include "logmanager.h"
#include "Colors.h"

Car::Car(b2World* world, float xPos, float yPos, float width, float length, float angle, float _M2P, float _P2M)
: m_pBody(nullptr)
, m_pSprite(nullptr)
, m_speed(50.0f)
, m_reverseSpeed(-50.0f)
, m_maxForce(100.0f)
, m_maxImpulse(0.0f)
, m_maxSpeed(0.0f)
, m_maxReverseSpeed(0.0f)
, m_maxWheelAngle(0.0f)
, m_maxPower(0.0f)
, m_pTailSmoke(nullptr)
, m_pTireTracksLeft(nullptr)
, m_pTireTracksRight(nullptr)
{
	IniParser* pIni = Game::GetIniFile();
	M2P = _M2P;
	P2M = _P2M;

	m_speed = pIni->GetValueAsFloat("car", "speed");
	m_reverseSpeed = pIni->GetValueAsFloat("car", "reverseSpeed");
	m_maxForce = pIni->GetValueAsFloat("car", "maxForce");
	m_maxImpulse = pIni->GetValueAsFloat("car", "maxImpulse");


	m_maxSpeed = pIni->GetValueAsFloat("car", "maxSpeed") * 10;
	m_maxReverseSpeed = pIni->GetValueAsFloat("car", "maxReverseSpeed");
	m_maxWheelAngle = pIni->GetValueAsFloat("car", "carSteeringAngle");
	m_maxPower = pIni->GetValueAsFloat("car", "carPower") * 10;

	// convert the max speed and and power values from KPH to Box2D units
	//m_maxSpeed = (m_maxSpeed * 1000) / 3600;
	//m_maxPower = (m_maxPower * 1000) / 3600;

	CreateCar(world, xPos, yPos, width, length, angle);

	m_pSprite = Game::GetBackBuffer()->CreateSprite("assets\\Textures\\Sprites\\car.png");

	LogManager::Title("Car Particles");
	m_pTailSmoke = new ParticleEmitter();
	m_pTailSmoke->SetDurationOfEmission(0.1f);
	m_pTailSmoke->SetLooping(false);
	m_pTailSmoke->SetLifeTime(2.0f);
	m_pTailSmoke->SetStartSpeed(2.0f);
	m_pTailSmoke->SetConeOfEmission(180.0, m_pSprite->GetAngle());
	m_pTailSmoke->SetStartColor(COL_Silver);
	m_pTailSmoke->SetEmissionRate(1);
	m_pTailSmoke->SetParticleSize(true);
	m_pTailSmoke->SetMaxParticles(200);

	m_pTireTracksLeft = new ParticleEmitter();
	m_pTireTracksLeft->SetDurationOfEmission(0.1f);
	m_pTireTracksLeft->SetLooping(false);
	m_pTireTracksLeft->SetLifeTime(3.0f);
	m_pTireTracksLeft->SetStartSpeed(0.0f);
	m_pTireTracksLeft->SetConeOfEmission(10.0, m_pSprite->GetAngle());
	m_pTireTracksLeft->SetStartColor(COL_Black);
	m_pTireTracksLeft->SetEmissionRate(3);
	m_pTireTracksLeft->SetParticleSize(10);
	m_pTireTracksLeft->SetMaxParticles(500);

	m_pTireTracksRight = new ParticleEmitter();
	m_pTireTracksRight->SetDurationOfEmission(0.1f);
	m_pTireTracksRight->SetLooping(false);
	m_pTireTracksRight->SetLifeTime(3.0f);
	m_pTireTracksRight->SetStartSpeed(0.0f);
	m_pTireTracksRight->SetConeOfEmission(10.0, m_pSprite->GetAngle());
	m_pTireTracksRight->SetStartColor(COL_Black);
	m_pTireTracksRight->SetEmissionRate(6);
	m_pTireTracksRight->SetParticleSize(10);
	m_pTireTracksRight->SetMaxParticles(1000);
}


Car::~Car()
{
	if (m_pSprite != nullptr)
	{
		delete m_pSprite;
		m_pSprite = nullptr;
	}

	for (int i = 0; i < NUM_WHEELS; ++i)
	{
		delete m_wheels[i];
		m_wheels[i] = nullptr;
	}

	delete m_pTailSmoke;
	m_pTailSmoke = nullptr;

	delete m_pTireTracksLeft;
	m_pTireTracksLeft = nullptr;

	delete m_pTireTracksRight;
	m_pTireTracksLeft = nullptr;
}

void
Car::Process(float deltaTime, int carControl, float acceleration, float turnAmount)
{
	bool accelerating = false;

	int i = 0;
	for (i = 0; i < NUM_WHEELS; ++i)
	{
		m_wheels[i]->RemoveLateralVelocity();
	}

	float angle = m_maxWheelAngle * turnAmount;

	m_pFrontLeftWheel->SetAngle(angle);
	m_pFrontRightWheel->SetAngle(angle);


	b2Vec2 speed = b2Vec2(0,0);
	if ((carControl & CAR_FORWARD))
	{
		speed.y = 1.0f;
		accelerating = true;
	}
	else if (carControl & CAR_BACKWARD)
	{
		accelerating = true;
		// Brake faster than we reverse
		if (GetVelocity().y < 0.0f)
			speed.y = -0.3f;
		else
		speed.y = -2.5f;
	}
	else
		speed = b2Vec2(0, 0);

	speed *= m_maxPower;

	m_pBackLeftWheel->m_pBody->ApplyForce(
		m_pFrontLeftWheel->m_pBody->GetWorldVector(speed),
		m_pFrontLeftWheel->m_pBody->GetWorldCenter(), true);
	
	m_pBackRightWheel->m_pBody->ApplyForce(
		m_pFrontRightWheel->m_pBody->GetWorldVector(speed),
		m_pFrontRightWheel->m_pBody->GetWorldCenter(), true);

	float curSpeed = GetSpeed();

	// Limit the speed to stop the car slowing down near infinitely fast
	// Also stops jittering of car sprite at very low speeds
	if (curSpeed < 5.0f && !accelerating)
	{
		SetSpeed(0.0f);
	}
	else if (curSpeed < 0.0f && curSpeed > -2.0f && !accelerating)
	{
		SetSpeed(0.0f);
	}

	ProcessParticles(deltaTime, speed);
}

void 
Car::ProcessParticles(float deltaTime, b2Vec2 speed)
{
	float carRDeg = m_pSprite->GetAngle() - 90.0f;
	float carRRad = Calc::DegToRadf(carRDeg);
	double carHalfWidth = m_pSprite->GetWidth() / 2;
	double carHalfLength = m_pSprite->GetHeight() / 2;

	if (m_pBody->GetLinearVelocity().y != 0) {
		m_pTailSmoke->Emission(); 
		m_pTireTracksLeft->Emission();
		m_pTireTracksRight->Emission();

		m_pTailSmoke->SetConeOfEmission(m_pTailSmoke->GetConeSize(), carRDeg);
		m_pTireTracksLeft->SetConeOfEmission(m_pTireTracksLeft->GetConeSize(), carRDeg);
		m_pTireTracksRight->SetConeOfEmission(m_pTireTracksRight->GetConeSize(), carRDeg);
	}

	m_pTailSmoke->Process(
		deltaTime,
		static_cast<int>((m_pBody->GetPosition().x * M2P) + (carHalfLength * cos(carRRad))),
		static_cast<int>((m_pBody->GetPosition().y * M2P) + (carHalfLength * sin(carRRad)))
	);
	m_pTireTracksLeft->Process(
		deltaTime,
		static_cast<int>((m_pBody->GetPosition().x * M2P) + (carHalfLength * cos(carRRad)) - (carHalfWidth - 10.0)),
			static_cast<int>((m_pBody->GetPosition().y * M2P) + (carHalfLength * sin(carRRad)))
	);
	m_pTireTracksRight->Process(
		deltaTime,
		static_cast<int>((m_pBody->GetPosition().x * M2P) + (carHalfLength * cos(carRRad)) + (carHalfWidth - 10.0)),
			static_cast<int>((m_pBody->GetPosition().y * M2P) + (carHalfLength * sin(carRRad)))
	);
}

void
Car::Draw(BackBuffer& backBuffer)
{
	//Draw particles first
	m_pTireTracksLeft->Draw(backBuffer);
	m_pTireTracksRight->Draw(backBuffer);
	m_pTailSmoke->Draw(backBuffer);

	// Draw the back wheels before the car so they clip under the body
	m_pBackLeftWheel->Draw(backBuffer);
	m_pBackRightWheel->Draw(backBuffer);

	// Draw the car
	m_pSprite->SetX(static_cast<int>(m_pBody->GetPosition().x * M2P - (m_pSprite->GetWidth() / 2)));
	m_pSprite->SetY(static_cast<int>(m_pBody->GetPosition().y * M2P - (m_pSprite->GetHeight() / 2)));
	m_pSprite->SetAngle(Calc::RadToDeg(m_pBody->GetAngle()));
	m_pSprite->Draw(backBuffer);

	// Draw the front wheels after the car so the clip over the wheel axle
	m_pFrontLeftWheel->Draw(backBuffer);
	m_pFrontRightWheel->Draw(backBuffer);

}

float 
Car::GetSpeed()
{
	return m_pBody->GetLinearVelocity().Length();
}

void 
Car::SetSpeed(float speed)
{
	b2Vec2 vel = m_pBody->GetLinearVelocity();
	vel.Normalize();
	vel.x = vel.x * speed;
	vel.y = vel.y * speed;
	m_pBody->SetLinearVelocity(vel);
}

b2Vec2
Car::GetVelocity()
{
	return m_pBody->GetLocalVector(m_pBody->GetLinearVelocityFromLocalPoint(b2Vec2(0,0)));
}

void
Car::SetPos(float x, float y, float angle, float P2M)
{
	b2Vec2 newPos = b2Vec2(x * P2M, y * P2M);
	m_pBody->SetTransform(newPos, m_pBody->GetAngle());

	for (int i = 0; i < 4; ++i)
	{
		b2Body* w = m_wheels[i]->m_pBody;
		b2Vec2 relativePos = m_pBody->GetPosition() - w->GetPosition();
		w->SetTransform(newPos + relativePos, w->GetAngle());
		//w->SetTransform(newPos, w->GetAngle());
	}
}

void
Car::CreateCar(b2World* world, float xPos, float yPos, float width, float length, float angle)
{

	// Create the car body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2Vec2(xPos, yPos);
	bodyDef.angle = angle;
	bodyDef.linearDamping = 0.15f; // "Friction" of the car on the ground
	bodyDef.bullet = true; // TEST setting car to bullet, better collision detection
	bodyDef.angularDamping = 0.3f;
	m_pBody = world->CreateBody(&bodyDef);

	// Create the car shape
	b2PolygonShape shape;
	shape.SetAsBox(width / 2, length / 2);
	b2FixtureDef fixDef;
	fixDef.density = 1.0f;
	fixDef.friction = 0.3f;
	fixDef.restitution = 0.4f;
	fixDef.shape = &shape;
	m_pBody->CreateFixture(&fixDef);

	// Add the wheels
	// Front left Wheel
	m_pFrontLeftWheel = new Wheel(world, m_pBody, width / 2, 4.0f, 0.4f, 0.8f, true, false, M2P, P2M);
	m_wheels[0] = m_pFrontLeftWheel;
	// Front right Wheel
	m_pFrontRightWheel = new Wheel(world, m_pBody, -width / 2, 4.0f, 0.4f, 0.8f, true, false, M2P, P2M);
	m_wheels[1] = m_pFrontRightWheel;
	// Back left Wheel
	m_pBackLeftWheel = new Wheel(world, m_pBody, width / 2, -5.5f, 0.4f, 0.8f, false, true, M2P, P2M);
	m_wheels[2] = m_pBackLeftWheel;
	// Back right Wheel
	m_pBackRightWheel = new Wheel(world, m_pBody, -width / 2, -5.5f, 0.4f, 0.8f, false, true, M2P, P2M);
	m_wheels[3] = m_pBackRightWheel;
}


