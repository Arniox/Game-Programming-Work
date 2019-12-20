#pragma once

#include "State.h"
class BackBuffer;
class InputManager;
class DebugDraw;

#include "Box2D/Box2D.h"

class Test2 :
	public State
{
public:
	Test2();
	~Test2();
	void Proccess(float deltaTime);
	void Draw(BackBuffer & backBuffer);
	void ProcessControls();
	void ResetWorld();
	void SetupWorld();
	void AddRect(int x, int y, int w, int h, bool dyn);
	void AddBall(int x, int y, float radius, float restitution);

	void SliderCrank()
	{
		b2Body* ground = NULL;
		{
			b2BodyDef bd;
			ground = m_pWorld->CreateBody(&bd);

			b2EdgeShape shape;
			shape.Set(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));
			ground->CreateFixture(&shape, 0.0f);
		}

		{
			b2Body* prevBody = ground;

			// Define crank.
			{
				b2PolygonShape shape;
				shape.SetAsBox(0.5f, 2.0f);

				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(0.0f, 7.0f);
				b2Body* body = m_pWorld->CreateBody(&bd);
				body->CreateFixture(&shape, 2.0f);

				b2RevoluteJointDef rjd;
				rjd.Initialize(prevBody, body, b2Vec2(0.0f, 5.0f));
				rjd.motorSpeed = 1.0f * static_cast<float>(M_PI);
				rjd.maxMotorTorque = 10000.0f;
				rjd.enableMotor = true;
				m_joint1 = (b2RevoluteJoint*)m_pWorld->CreateJoint(&rjd);

				prevBody = body;
			}

			// Define follower.
			{
				b2PolygonShape shape;
				shape.SetAsBox(0.5f, 4.0f);

				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(0.0f, 13.0f);
				b2Body* body = m_pWorld->CreateBody(&bd);
				body->CreateFixture(&shape, 2.0f);

				b2RevoluteJointDef rjd;
				rjd.Initialize(prevBody, body, b2Vec2(0.0f, 9.0f));
				rjd.enableMotor = false;
				m_pWorld->CreateJoint(&rjd);

				prevBody = body;
			}

			// Define piston
			{
				b2PolygonShape shape;
				shape.SetAsBox(1.5f, 1.5f);

				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.fixedRotation = true;
				bd.position.Set(0.0f, 17.0f);
				b2Body* body = m_pWorld->CreateBody(&bd);
				body->CreateFixture(&shape, 2.0f);

				b2RevoluteJointDef rjd;
				rjd.Initialize(prevBody, body, b2Vec2(0.0f, 17.0f));
				m_pWorld->CreateJoint(&rjd);

				b2PrismaticJointDef pjd;
				pjd.Initialize(ground, body, b2Vec2(0.0f, 17.0f), b2Vec2(0.0f, 1.0f));

				pjd.maxMotorForce = 1000.0f;
				pjd.enableMotor = true;

				m_joint2 = (b2PrismaticJoint*)m_pWorld->CreateJoint(&pjd);
			}

			// Create a payload
			{
				b2PolygonShape shape;
				shape.SetAsBox(1.5f, 1.5f);

				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				bd.position.Set(0.0f, 23.0f);
				b2Body* body = m_pWorld->CreateBody(&bd);
				body->CreateFixture(&shape, 2.0f);
			}
		}
	}

private:
	BackBuffer* m_pBackBuffer;
	DebugDraw* m_pDebugDrawer;
	b2RevoluteJoint* m_joint1;
	b2PrismaticJoint* m_joint2;
	InputManager* m_pInput;
	b2World* m_pWorld;
	int m_width;
	int m_height;
	bool m_debug;
	bool m_clearWorld;
	float M2P = 64.0f;
	float P2M = 1.0f / M2P;
};

