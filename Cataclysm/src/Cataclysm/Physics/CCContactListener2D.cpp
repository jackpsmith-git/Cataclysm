#include "ccpch.h"
#include "Cataclysm/Physics/CCContactListener2D.h"

#include <box2d/b2_contact.h>
#include <box2d/b2_body.h>

namespace Cataclysm {

	void CCContactListener2D::BeginContact(b2Contact* contact)
	{
		auto bodyA = contact->GetFixtureA()->GetBody();
		auto bodyB = contact->GetFixtureB()->GetBody();

		CCUUID* uuidA = reinterpret_cast<CCUUID*>(bodyA->GetUserData().pointer);
		CCUUID* uuidB = reinterpret_cast<CCUUID*>(bodyB->GetUserData().pointer);

		if (uuidA && uuidB)
			m_Collisions.push_back({ *uuidA, *uuidB });
	}

	void CCContactListener2D::EndContact(b2Contact* contact)
	{
		auto bodyA = contact->GetFixtureA()->GetBody();
		auto bodyB = contact->GetFixtureB()->GetBody();

		CCUUID* uuidA = reinterpret_cast<CCUUID*>(bodyA->GetUserData().pointer);
		CCUUID* uuidB = reinterpret_cast<CCUUID*>(bodyB->GetUserData().pointer);

		if (uuidA && uuidB)
			m_EndCollisions.push_back({ *uuidA, *uuidB });
	}
}
