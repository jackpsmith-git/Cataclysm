#include "ccpch.h"

#include "ContactListener2D.h"
#include <box2d/b2_contact.h>
#include <box2d/b2_body.h>

namespace Cataclysm {

	void ContactListener2D::BeginContact(b2Contact* contact)
	{
		auto bodyA = contact->GetFixtureA()->GetBody();
		auto bodyB = contact->GetFixtureB()->GetBody();

		UUID* uuidA = reinterpret_cast<UUID*>(bodyA->GetUserData().pointer);
		UUID* uuidB = reinterpret_cast<UUID*>(bodyB->GetUserData().pointer);

		if (uuidA && uuidB)
			m_Collisions.push_back({ *uuidA, *uuidB });
	}

	void ContactListener2D::EndContact(b2Contact* contact)
	{
		auto bodyA = contact->GetFixtureA()->GetBody();
		auto bodyB = contact->GetFixtureB()->GetBody();

		UUID* uuidA = reinterpret_cast<UUID*>(bodyA->GetUserData().pointer);
		UUID* uuidB = reinterpret_cast<UUID*>(bodyB->GetUserData().pointer);

		if (uuidA && uuidB)
			m_EndCollisions.push_back({ *uuidA, *uuidB });
	}
}
