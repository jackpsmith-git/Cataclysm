#pragma once

#include "Cataclysm/Core/CCCore.h"
#include "Cataclysm/Vesuvius/VSTexture.h"

namespace Cataclysm
{
	class CPResources
	{
	public:
		static void Initialize();
		static void InitializeTextures();

		static CCRef<Vesuvius::VSTexture2D> ICON_PLAY, ICON_PAUSE, ICON_STEP, ICON_SIMULATE, ICON_STOP,
			ICON_SHOW_PHYSICS_COLLIDERS, ICON_TRANSLATE, ICON_ROTATE, ICON_SCALE,
			ICON_SHOW_PHYSICS_COLLIDERS_FADED, ICON_TRANSLATE_FADED, ICON_ROTATE_FADED, ICON_SCALE_FADED, ICON_EMPTY, ICON_EMPTY_FADED,
			ICON_CLEAR, ICON_DOCUMENTATION, ICON_RESET_EDITOR_CAMERA,
			ICON_ERROR, ICON_WARNING, ICON_TRACE, ICON_INFO,
			ICON_ERROR_FADED, ICON_WARNING_FADED, ICON_TRACE_FADED, ICON_INFO_FADED, ICON_CATACLYSM,
			ICON_AUDIO_SOURCE, ICON_BOX_COLLIDER_2D, ICON_CIRCLE_COLLIDER, ICON_CIRCLE_RENDERER, ICON_ENTITY, ICON_RIGIDBODY_2D, ICON_MONO_SCRIPT, ICON_SPRITE_RENDERER,
			ICON_TEXT, ICON_TRANSFORM, ICON_CAMERA, ICON_DIRECTORY, ICON_FILE, ICON_DIRECTORY_OPEN,
			ICON_BACK, ICON_PLUS, ICON_TEXTURE_2D, ICON_X, ICON_AUDIO_CLIP, ICON_CSHARP_SCRIPT, ICON_DELETE, ICON_FONT,
			ICON_CSHARP_LOGO_320, ICON_CATACLYSM_WHITE, ICON_AUDIO_CLIP_32, ICON_TEXTURE_2D_32, ICON_FONT_32,
			ICON_LIST, ICON_TILES, ICON_LIST_FADED, ICON_TILES_FADED, ICON_TILES_SMALL, ICON_TILES_SMALL_FADED, ICON_UI_SCALE, ICON_RESET,
			ICON_TIMESTAMP, ICON_TIMESTAMP_FADED, ICON_LOCK, ICON_UNLOCK, ICON_COPY, ICON_PASTE;

		static std::vector<uint8_t> LoadEmbeddedTexture(int resourceID);

		template<typename T>
		static constexpr CCRef<Vesuvius::VSTexture2D> GetComponentIcon()
		{
			if constexpr (std::is_same_v<T, AudioSource>)
				return CPResources::ICON_AUDIO_SOURCE;
			else if constexpr (std::is_same_v<T, BoxCollider2D>)
				return CPResources::ICON_BOX_COLLIDER_2D;
			else if constexpr (std::is_same_v<T, Camera>)
				return CPResources::ICON_CAMERA;
			else if constexpr (std::is_same_v<T, CircleCollider2D>)
				return CPResources::ICON_CIRCLE_COLLIDER;
			else if constexpr (std::is_same_v<T, CircleRenderer>)
				return CPResources::ICON_CIRCLE_RENDERER;
			else if constexpr (std::is_same_v<T, MonoScript> || std::is_same_v<T, NativeScript>)
				return CPResources::ICON_MONO_SCRIPT;
			else if constexpr (std::is_same_v<T, Rigidbody2D>)
				return CPResources::ICON_RIGIDBODY_2D;
			else if constexpr (std::is_same_v<T, SpriteRenderer>)
				return CPResources::ICON_SPRITE_RENDERER;
			else if constexpr (std::is_same_v<T, Text>)
				return CPResources::ICON_TEXT;
			else if constexpr (std::is_same_v<T, Transform>)
				return CPResources::ICON_TRANSFORM;
			else
				return CPResources::ICON_TRANSFORM;
		}
	};
}
