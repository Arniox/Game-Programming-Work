#include "Wheel.h"
#include "backbuffer.h"
#include "sprite.h"
#include "Calc.h"
#include "game.h"
#include "IniParser.h"

Wheel::Wheel(b2World* world, b2Body* carBody, float xPos, float yPos, float wheelWidth, float wheelHeight, bool revolving, bool powered, float _M2P, float _P2M)
: m_pBody(nullptr)
, m_pSprite(nullptr)
, m_pCarBody(nullptr)
{
	m_pCarBody = carBody;

	M2P = _M2P;
	P2M = _P2M;
	m_pSprite = Game::GetBackBuffer()->CreateSprite("assets\\Textures\\Sprites\\wheel.png");
	
	CreateWheel(world, xPos, yPos, wheelWidth, wheelHeight, revolving, powered);
}


Wheel::~Wheel()
{
	if (m_pSprite != nullptr)
	{
		delete m_pSprite;
		m_pSprite = nullptr;
	}
}

void
Wheel::CreateWheel(b2World* world, float xPos, float yPos, float wheelWidth, float wheelHeight, bool revolving, bool powered)
{
	// Set the wheel position
	//m_position.Set(static_cast<float32>(xPos), static_cast<float32>(yPos));

	// Create the car body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = m_pCarBody->GetWorldPoint(b2Vec2(static_cast<float32>(xPos), static_cast<float32>(yPos)));
	bodyDef.angle = m_pCarBody->GetAngle();
	m_pBody = world->CreateBody(&bodyDef);

	// Create the car shape
	b2PolygonShape shape;
	shape.SetAsBox(wheelWidth / 2, wheelHeight / 2);
	b2FixtureDef fixDef;
	fixDef.density = 1.0f;
	fixDef.isSensor = true; // Test setting the wheel to not collide;
	fixDef.shape = &shape;
	m_pBody->CreateFixture(&fixDef);

	// Attach the wheel to the car
	// If the wheel is used for steering, create a revolving joint
	if (revolving)
	{
		b2RevoluteJointDef jointDef;
		jointDef.Initialize(m_pCarBody, m_pBody, m_pBody->GetWorldCenter());
		jointDef.enableMotor = false;

		world->CreateJoint(&jointDef);
	}
	else
	{
		b2PrismaticJointDef jointDef;
		jointDef.Initialize(m_pCarBody, m_pBody, m_pBody->GetWorldCenter(), b2Vec2(1,0));
		jointDef.enableLimit = true;
		jointDef.lowerTranslation = 0.0f;
		jointDef.upperTranslation = 0.0f;

		world->CreateJoint(&jointDef);
	}
}

void
Wheel::Init(float speed, float reverseSpeed, float maxForce, float maxImpulse)
{
	m_speed = speed;
	m_reverseSpeed = reverseSpeed;
	m_maxForce = maxForce;
	m_maxImpulse = maxImpulse;
}

void
Wheel::Draw(BackBuffer& backBuffer)
{
	m_pSprite->SetX(static_cast<int>(m_pBody->GetPosition().x * M2P - (m_pSprite->GetWidth() / 2)));
	m_pSprite->SetY(static_cast<int>(m_pBody->GetPosition().y * M2P - (m_pSprite->GetHeight() / 2)));
	m_pSprite->SetAngle(Calc::RadToDeg(m_pBody->GetAngle()));
	m_pSprite->Draw(backBuffer);
}

void
Wheel::Process(int carControl, float acceleration)
{
	UpdateFriction();
	ProcessAcceleration(carControl, acceleration);
	ProcessTurning(carControl);
}

void
Wheel::ProcessAcceleration(int carControl, float acceleration)
{
	float targetSpeed = 0.0f;

	// Should the car be moving forward or backward?
	if (carControl & CAR_FORWARD)
		targetSpeed = m_speed * acceleration;
	else if (carControl & CAR_BACKWARD)
		targetSpeed = m_reverseSpeed * acceleration;

	// Find the current speed 
	b2Vec2 forwardNorm = m_pBody->GetWorldVector(forwardVec);
	float speed = b2Dot(GetForwardVelocity(), forwardNorm);

	// Apply the force
	float force = 0.0f;

	if (targetSpeed > speed)
		force = m_maxForce;
	else if (targetSpeed < speed)
		force -= m_maxForce;
	else
		return;

	m_pBody->ApplyForce(force * forwardNorm, m_pBody->GetWorldCenter(), true);
}

void
Wheel::ProcessTurning(int carControl)
{
	float turnSpeed = 0.0f;

	// Should the car be moving forward or backward?
	if (carControl & CAR_LEFT)
		turnSpeed = -15.0f;
	else if (carControl & CAR_RIGHT)
		turnSpeed = 15.0f;

	m_pBody->ApplyTorque(turnSpeed, true);
}

b2Vec2
Wheel::GetLateralVelocity()
{
	b2Vec2 latVel = m_pBody->GetWorldVector(b2Vec2(1, 0));
	return b2Dot(latVel, m_pBody->GetLinearVelocity()) * latVel;
}

b2Vec2
Wheel::GetForwardVelocity() {
	b2Vec2 currentForwardNormal = m_pBody->GetWorldVector(b2Vec2(0, 1));
	return b2Dot(currentForwardNormal, m_pBody->GetLinearVelocity()) * currentForwardNormal;
}


void
Wheel::UpdateFriction()
{
	//lateral linear velocity
	float maxImpulse = 2.5f;

	b2Vec2 impulse = m_pBody->GetMass() * -GetLateralVelocity();

	if (impulse.Length() > maxImpulse)
		impulse *= maxImpulse / impulse.Length();

	m_pBody->ApplyLinearImpulse(impulse, m_pBody->GetWorldCenter(), true);

	//angular velocity
	m_pBody->ApplyAngularImpulse(0.1f * m_pBody->GetInertia() * -m_pBody->GetAngularVelocity(), true);

	//forward linear velocity
	b2Vec2 currentForwardNormal = GetForwardVelocity();
	float currentForwardSpeed = currentForwardNormal.Normalize();
	float dragForceMagnitude = -2 * currentForwardSpeed;

	m_pBody->ApplyForce(dragForceMagnitude * currentForwardNormal, m_pBody->GetWorldCenter(), true);
}

void
Wheel::SetAngle(float32 angle)
{
	angle = Calc::DegToRadf(angle);
	m_pBody->SetTransform(m_pBody->GetPosition(), m_pCarBody->GetAngle() + angle);
}

b2Vec2
Wheel::GetVelocity()
{
	return m_pCarBody->GetLocalVector(m_pCarBody->GetLinearVelocityFromLocalPoint(m_pBody->GetPosition()));;
}

b2Vec2
Wheel::GetDirection()
{
	float angle = m_pBody->GetAngle();
	float x = 0.0f;
	float y = GetVelocity().y;
	y = (y > 0.0f) ? 1.0f : -1.0f;

	return b2Vec2(
		x * cosf(angle) - y * sinf(angle),
		x * sinf(angle) + y * cosf(angle)
	);
}

void
Wheel::RemoveLateralVelocity()
{
	m_pBody->SetLinearVelocity(GetVelocityVector());
}

b2Vec2 Wheel::GetVelocityVector()
{
	b2Vec2 vel = m_pBody->GetLinearVelocity();
	b2Vec2 dir = GetDirection();
	float dot = (vel.x * dir.x) + (vel.y * dir.y);
	return b2Vec2(dir.x * dot, dir.y * dot);
}
