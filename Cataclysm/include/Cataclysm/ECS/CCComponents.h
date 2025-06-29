#pragma once

#include "CCIDComponent.h"
#include "CCTagComponent.h"

#include "CCTransform.h"
#include "CCSpriteRenderer.h"
#include "CCCircleRenderer.h"
#include "CCCamera.h"
#include "CCMonoScript.h"
#include "CCNativeScript.h"
#include "CCRigidbody2D.h"
#include "CCBoxCollider2D.h"
#include "CCCircleCollider2D.h"
#include "CCText.h"
#include "CCAudioSource.h"

namespace Cataclysm
{
	template<typename... Component>
	struct ComponentGroup
	{
	};

	using AllComponents =
		ComponentGroup<Transform, SpriteRenderer,
		CircleRenderer, Camera, MonoScript,
		NativeScript, Rigidbody2D, BoxCollider2D,
		CircleCollider2D, Text, AudioSource>; 
}
