#pragma once

#include "Cataclysm/Scene/CCSceneCamera.h"

namespace Cataclysm
{
	struct Camera
	{
		CCSceneCamera SceneCamera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		Camera() = default;
		Camera(const Camera&) = default;

		void Reset()
		{
			Primary = true;
			FixedAspectRatio = false;

			SceneCamera.SetProjectionType(Cataclysm::CCSceneCamera::CCProjectionType::Orthographic);

			SceneCamera.SetOrthographicSize(10.0f);
			SceneCamera.SetOrthographicNearClip(-1.0f);
			SceneCamera.SetOrthographicFarClip(1.0f);

			SceneCamera.SetPerspectiveVerticalFOV(45.0f);
			SceneCamera.SetPerspectiveNearClip(0.01f);
			SceneCamera.SetPerspectiveFarClip(1000.0f);
		}
	};
}
