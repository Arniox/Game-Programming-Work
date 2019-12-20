#include "DebugDraw.h"

#include "backbuffer.h"
#include "logmanager.h"
#include "IniParser.h"


DebugDraw::DebugDraw(float _M2P, float _P2M)
: m_pWorld(nullptr)
, m_pBackBuffer(nullptr)
{
	M2P = _M2P;
	P2M = _P2M;
}


DebugDraw::~DebugDraw()
{
	//delete[] m_points;
}

bool
DebugDraw::Init(b2World* world, BackBuffer* backBuffer)
{
	m_pWorld = world;
	m_pBackBuffer = backBuffer;

	if (m_pWorld != nullptr && m_pBackBuffer != nullptr)
		return true;
	return false;
}

void
DebugDraw::Draw()
{
	b2Body* body = m_pWorld->GetBodyList();

	while (body)
	{
		b2Shape::Type type = body->GetFixtureList()->GetShape()->m_type;

		if (type == b2Shape::e_polygon)
		{
			b2PolygonShape* p = static_cast<b2PolygonShape*>(body->GetFixtureList()->GetShape());
			int numPoints = p->m_count;

			for (int i = 0; i < numPoints; ++i)
			{
				m_points[i] = p->m_vertices[i];
				RotateTranslate(m_points[i], body->GetPosition(), body->GetAngle());
			}
			if (body->GetType() == b2BodyType::b2_dynamicBody)
			{
				if (body->IsAwake())
					m_pBackBuffer->SetDrawColour(0xE5, 0xB2, 0xB2);
				else
					m_pBackBuffer->SetDrawColour(0x99, 0x99, 0x99);
			}
			else
			{
				m_pBackBuffer->SetDrawColour(0x7F, 0xE5, 0x7F);
			}
			

			DrawPolygon(m_points, numPoints);
		}
		else if (type == b2Shape::e_circle)
		{
			b2CircleShape* c = static_cast<b2CircleShape*>(body->GetFixtureList()->GetShape());
			float radius = c->m_radius * M2P;

			if (body->IsAwake())
				m_pBackBuffer->SetDrawColour(0xF3, 0x89, 0x89);
			else
				m_pBackBuffer->SetDrawColour(0x7F, 0x7F, 0x7F);

			m_pBackBuffer->DrawCircle(static_cast<int>(body->GetPosition().x * M2P), static_cast<int>(body->GetPosition().y * M2P), static_cast<int>(radius));

			m_pBackBuffer->DrawLine(
				static_cast<int>(body->GetPosition().x * M2P),
				static_cast<int>(body->GetPosition().y * M2P),
				static_cast<int>(((body->GetPosition().x + (c->m_radius * cosf(body->GetAngle()))) * M2P)),
				static_cast<int>(((body->GetPosition().y + (c->m_radius * sinf(body->GetAngle()))) * M2P))
			);
		}
		else if (type == b2Shape::e_edge)
		{
			b2EdgeShape* e = static_cast<b2EdgeShape*>(body->GetFixtureList()->GetShape());
			m_pBackBuffer->SetDrawColour(0x00, 0xff, 0x00);
			m_pBackBuffer->DrawLine(
				static_cast<int>(e->m_vertex0.x * M2P),
				static_cast<int>(e->m_vertex0.y * M2P),
				static_cast<int>(e->m_vertex1.x * M2P),
				static_cast<int>(e->m_vertex1.y * M2P)
			);
		}

		body = body->GetNext();

		
	}

	b2Joint* joint = m_pWorld->GetJointList();
	while (joint)
	{
		b2Vec2 a = joint->GetBodyA()->GetPosition();
		b2Vec2 b = joint->GetBodyB()->GetPosition();

		

		m_pBackBuffer->SetDrawColour(0x7F, 0xCC, 0xCC);

		m_pBackBuffer->DrawLine(
			static_cast<int>(a.x * M2P),
			static_cast<int>(a.y * M2P),
			static_cast<int>(b.x * M2P),
			static_cast<int>(b.y * M2P)
		);

		joint = joint->GetNext();
	}
}

void
DebugDraw::DrawPolygon(b2Vec2* points, int numPoints)
{
	for (int i = 0; i < numPoints; ++i)
	{
		m_pBackBuffer->DrawLine(
			static_cast<int>(points[i].x * M2P),
			static_cast<int>(points[i].y * M2P),
			static_cast<int>(points[(i + 1) > numPoints - 1 ? 0 : (i + 1)].x * M2P),
			static_cast<int>(points[(i + 1) > numPoints - 1 ? 0 : (i + 1)].y * M2P)
		);
	}
}

void
DebugDraw::RotateTranslate(b2Vec2& vector, const b2Vec2& center, float angle)
{
	b2Vec2 tmp;
	tmp.x = vector.x * cosf(angle) - vector.y * sinf(angle);
	tmp.y = vector.x * sinf(angle) + vector.y * cosf(angle);
	vector = tmp + center;
}
