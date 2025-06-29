#include "ccpch.h"
#include "CPResources.h"
#include "CPStyles.h"
#include "Resources.h"

#include <Windows.h>

namespace Cataclysm
{
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_ENTITY;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_TEXTURE_2D;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_AUDIO_CLIP;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_FONT;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_DELETE;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_COPY;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_PASTE;

	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_PAUSE;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_PLAY;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_RESET_EDITOR_CAMERA;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_ROTATE;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_ROTATE_FADED;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_SCALE;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_SCALE_FADED;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_SHOW_PHYSICS_COLLIDERS;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_SHOW_PHYSICS_COLLIDERS_FADED;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_SIMULATE;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_STEP;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_STOP;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_EMPTY;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_EMPTY_FADED;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_TRANSLATE;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_TRANSLATE_FADED;

	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_CLEAR;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_ERROR;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_WARNING;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_TRACE;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_INFO;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_ERROR_FADED;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_WARNING_FADED;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_TRACE_FADED;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_INFO_FADED;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_TIMESTAMP;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_TIMESTAMP_FADED;

	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_CATACLYSM;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_DOCUMENTATION;

	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_AUDIO_SOURCE;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_BOX_COLLIDER_2D;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_CIRCLE_COLLIDER;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_CIRCLE_RENDERER;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_RIGIDBODY_2D;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_MONO_SCRIPT;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_SPRITE_RENDERER;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_TEXT;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_TRANSFORM;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_CAMERA;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_TEXTURE_2D_32;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_AUDIO_CLIP_32;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_CSHARP_SCRIPT;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_FONT_32;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_LOCK;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_UNLOCK;

	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_DIRECTORY;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_DIRECTORY_OPEN;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_FILE;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_BACK;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_PLUS;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_X;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_CATACLYSM_WHITE;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_CSHARP_LOGO_320;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_LIST;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_TILES;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_TILES_SMALL;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_TILES_SMALL_FADED;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_LIST_FADED;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_TILES_FADED;

	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_UI_SCALE;
	CCRef<Vesuvius::VSTexture2D> CPResources::ICON_RESET;

	void CPResources::Initialize()
	{
		InitializeTextures();
	}

	void CPResources::InitializeTextures()
	{
		auto entityIconData = LoadEmbeddedTexture(IDB_ICON_ENTITY);
		CPResources::ICON_ENTITY = Vesuvius::VSTexture2D::Create(entityIconData.data(), entityIconData.size());

		auto texture2DIconData = LoadEmbeddedTexture(IDB_ICON_TEXTURE_2D);
		CPResources::ICON_TEXTURE_2D = Vesuvius::VSTexture2D::Create(texture2DIconData.data(), texture2DIconData.size());

		auto audioClipIconData = LoadEmbeddedTexture(IDB_ICON_AUDIO_CLIP);
		CPResources::ICON_AUDIO_CLIP = Vesuvius::VSTexture2D::Create(audioClipIconData.data(), audioClipIconData.size());

		auto fontIconData = LoadEmbeddedTexture(IDB_ICON_FONT);
		CPResources::ICON_FONT = Vesuvius::VSTexture2D::Create(fontIconData.data(), fontIconData.size());

		auto deleteIconData = LoadEmbeddedTexture(IDB_ICON_DELETE);
		CPResources::ICON_DELETE = Vesuvius::VSTexture2D::Create(deleteIconData.data(), deleteIconData.size());

		auto resetIconData = LoadEmbeddedTexture(IDB_ICON_RESET);
		CPResources::ICON_RESET = Vesuvius::VSTexture2D::Create(resetIconData.data(), resetIconData.size());

		auto pauseIconData = LoadEmbeddedTexture(IDB_ICON_PAUSE);
		CPResources::ICON_PAUSE = Vesuvius::VSTexture2D::Create(pauseIconData.data(), pauseIconData.size());

		auto playIconData = LoadEmbeddedTexture(IDB_ICON_PLAY);
		CPResources::ICON_PLAY = Vesuvius::VSTexture2D::Create(playIconData.data(), playIconData.size());

		auto resetEditorCameraIconData = LoadEmbeddedTexture(IDB_ICON_RESET_EDITOR_CAMERA);
		CPResources::ICON_RESET_EDITOR_CAMERA = Vesuvius::VSTexture2D::Create(resetEditorCameraIconData.data(), resetEditorCameraIconData.size());

		auto rotateIconData = LoadEmbeddedTexture(IDB_ICON_ROTATE);
		CPResources::ICON_ROTATE = Vesuvius::VSTexture2D::Create(rotateIconData.data(), rotateIconData.size());

		auto rotateIconFadedData = LoadEmbeddedTexture(IDB_ICON_ROTATE_FADED);
		CPResources::ICON_ROTATE_FADED = Vesuvius::VSTexture2D::Create(rotateIconFadedData.data(), rotateIconFadedData.size());

		auto scaleIconData = LoadEmbeddedTexture(IDB_ICON_SCALE);
		CPResources::ICON_SCALE = Vesuvius::VSTexture2D::Create(scaleIconData.data(), scaleIconData.size());

		auto scaleIconFadedData = LoadEmbeddedTexture(IDB_ICON_SCALE_FADED);
		CPResources::ICON_SCALE_FADED = Vesuvius::VSTexture2D::Create(scaleIconFadedData.data(), scaleIconFadedData.size());

		auto showPhysicsCollidersIconData = LoadEmbeddedTexture(IDB_ICON_SHOW_PHYSICS_COLLIDERS);
		CPResources::ICON_SHOW_PHYSICS_COLLIDERS = Vesuvius::VSTexture2D::Create(showPhysicsCollidersIconData.data(), showPhysicsCollidersIconData.size());

		auto showPhysicsCollidersFadedData = LoadEmbeddedTexture(IDB_ICON_SHOW_PHYSICS_COLLIDERS_FADED);
		CPResources::ICON_SHOW_PHYSICS_COLLIDERS_FADED = Vesuvius::VSTexture2D::Create(showPhysicsCollidersFadedData.data(), showPhysicsCollidersFadedData.size());

		auto simulateIconData = LoadEmbeddedTexture(IDB_ICON_SIMULATE);
		CPResources::ICON_SIMULATE = Vesuvius::VSTexture2D::Create(simulateIconData.data(), simulateIconData.size());

		auto stepIconData = LoadEmbeddedTexture(IDB_ICON_STEP);
		CPResources::ICON_STEP = Vesuvius::VSTexture2D::Create(stepIconData.data(), stepIconData.size());

		auto stopIconData = LoadEmbeddedTexture(IDB_ICON_STOP);
		CPResources::ICON_STOP = Vesuvius::VSTexture2D::Create(stopIconData.data(), stopIconData.size());

		auto emptyIconData = LoadEmbeddedTexture(IDB_ICON_EMPTY);
		CPResources::ICON_EMPTY = Vesuvius::VSTexture2D::Create(emptyIconData.data(), emptyIconData.size());

		auto emptyIconFadedData = LoadEmbeddedTexture(IDB_ICON_EMPTY_FADED);
		CPResources::ICON_EMPTY_FADED = Vesuvius::VSTexture2D::Create(emptyIconFadedData.data(), emptyIconFadedData.size());

		auto translateIconData = LoadEmbeddedTexture(IDB_ICON_TRANSLATE);
		CPResources::ICON_TRANSLATE = Vesuvius::VSTexture2D::Create(translateIconData.data(), translateIconData.size());

		auto translateIconFadedData = LoadEmbeddedTexture(IDB_ICON_TRANSLATE_FADED);
		CPResources::ICON_TRANSLATE_FADED = Vesuvius::VSTexture2D::Create(translateIconFadedData.data(), translateIconFadedData.size());
		
		auto clearIcon = LoadEmbeddedTexture(IDB_ICON_CLEAR);
		CPResources::ICON_CLEAR = Vesuvius::VSTexture2D::Create(clearIcon.data(), clearIcon.size());

		auto errorIconData = LoadEmbeddedTexture(IDB_ICON_ERROR);
		CPResources::ICON_ERROR = Vesuvius::VSTexture2D::Create(errorIconData.data(), errorIconData.size());

		auto errorIconFadedData = LoadEmbeddedTexture(IDB_ICON_ERROR_FADED);
		CPResources::ICON_ERROR_FADED = Vesuvius::VSTexture2D::Create(errorIconFadedData.data(), errorIconFadedData.size());

		auto warningIconData = LoadEmbeddedTexture(IDB_ICON_WARNING);
		CPResources::ICON_WARNING = Vesuvius::VSTexture2D::Create(warningIconData.data(), warningIconData.size());

		auto warningIconFadedData = LoadEmbeddedTexture(IDB_ICON_WARNING_FADED);
		CPResources::ICON_WARNING_FADED = Vesuvius::VSTexture2D::Create(warningIconFadedData.data(), warningIconFadedData.size());

		auto traceIconData = LoadEmbeddedTexture(IDB_ICON_TRACE);
		CPResources::ICON_TRACE = Vesuvius::VSTexture2D::Create(traceIconData.data(), traceIconData.size());

		auto traceIconFadedData = LoadEmbeddedTexture(IDB_ICON_TRACE_FADED);
		CPResources::ICON_TRACE_FADED = Vesuvius::VSTexture2D::Create(traceIconFadedData.data(), traceIconFadedData.size());

		auto infoIconData = LoadEmbeddedTexture(IDB_ICON_INFO);
		CPResources::ICON_INFO = Vesuvius::VSTexture2D::Create(infoIconData.data(), infoIconData.size());

		auto infoIconFadedData = LoadEmbeddedTexture(IDB_ICON_INFO_FADED);
		CPResources::ICON_INFO_FADED = Vesuvius::VSTexture2D::Create(infoIconFadedData.data(), infoIconFadedData.size());

		auto timestampIconData = LoadEmbeddedTexture(IDB_ICON_TIMESTAMP);
		CPResources::ICON_TIMESTAMP = Vesuvius::VSTexture2D::Create(timestampIconData.data(), timestampIconData.size());

		auto timestampIconFadedData = LoadEmbeddedTexture(IDB_ICON_TIMESTAMP_FADED);
		CPResources::ICON_TIMESTAMP_FADED = Vesuvius::VSTexture2D::Create(timestampIconFadedData.data(), timestampIconFadedData.size());

		auto cataclysmIconData = LoadEmbeddedTexture(IDB_ICON_CATACLYSM);
		CPResources::ICON_CATACLYSM = Vesuvius::VSTexture2D::Create(cataclysmIconData.data(), cataclysmIconData.size());

		auto documentationIconData = LoadEmbeddedTexture(IDB_ICON_DOCUMENTATION);
		CPResources::ICON_DOCUMENTATION = Vesuvius::VSTexture2D::Create(documentationIconData.data(), documentationIconData.size());

		auto audioSourceIconData = LoadEmbeddedTexture(IDB_ICON_AUDIO_SOURCE);
		CPResources::ICON_AUDIO_SOURCE = Vesuvius::VSTexture2D::Create(audioSourceIconData.data(), audioSourceIconData.size());

		auto boxColliderIconData = LoadEmbeddedTexture(IDB_ICON_BOX_COLLIDER);
		CPResources::ICON_BOX_COLLIDER_2D = Vesuvius::VSTexture2D::Create(boxColliderIconData.data(), boxColliderIconData.size());

		auto circleColliderIconData = LoadEmbeddedTexture(IDB_ICON_CIRCLE_COLLIDER);
		CPResources::ICON_CIRCLE_COLLIDER = Vesuvius::VSTexture2D::Create(circleColliderIconData.data(), circleColliderIconData.size());

		auto circleRendererIconData = LoadEmbeddedTexture(IDB_ICON_CIRCLE_RENDERER);
		CPResources::ICON_CIRCLE_RENDERER = Vesuvius::VSTexture2D::Create(circleRendererIconData.data(), circleRendererIconData.size());

		auto rigidbody2DIconData = LoadEmbeddedTexture(IDB_ICON_RIGIDBODY_2D);
		CPResources::ICON_RIGIDBODY_2D = Vesuvius::VSTexture2D::Create(rigidbody2DIconData.data(), rigidbody2DIconData.size());

		auto monoScriptIconData = LoadEmbeddedTexture(IDB_ICON_MONO_SCRIPT);
		CPResources::ICON_MONO_SCRIPT = Vesuvius::VSTexture2D::Create(monoScriptIconData.data(), monoScriptIconData.size());

		auto spriteRendererIconData = LoadEmbeddedTexture(IDB_ICON_SPRITE_RENDERER);
		CPResources::ICON_SPRITE_RENDERER = Vesuvius::VSTexture2D::Create(spriteRendererIconData.data(), spriteRendererIconData.size());

		auto textIconData = LoadEmbeddedTexture(IDB_ICON_TEXT);
		CPResources::ICON_TEXT = Vesuvius::VSTexture2D::Create(textIconData.data(), textIconData.size());

		auto transformIconData = LoadEmbeddedTexture(IDB_ICON_TRANSFORM);
		CPResources::ICON_TRANSFORM = Vesuvius::VSTexture2D::Create(transformIconData.data(), transformIconData.size());

		auto cameraIconData = LoadEmbeddedTexture(IDB_ICON_CAMERA);
		CPResources::ICON_CAMERA = Vesuvius::VSTexture2D::Create(cameraIconData.data(), cameraIconData.size());

		auto audioClip32IconData = LoadEmbeddedTexture(IDB_ICON_AUDIO_CLIP_32);
		CPResources::ICON_AUDIO_CLIP_32 = Vesuvius::VSTexture2D::Create(audioClip32IconData.data(), audioClip32IconData.size());

		auto texture2D32IconData = LoadEmbeddedTexture(IDB_ICON_TEXTURE_2D_32);
		CPResources::ICON_TEXTURE_2D_32 = Vesuvius::VSTexture2D::Create(texture2D32IconData.data(), texture2D32IconData.size());

		auto font32IconData = LoadEmbeddedTexture(IDB_ICON_FONT_32);
		CPResources::ICON_FONT_32 = Vesuvius::VSTexture2D::Create(font32IconData.data(), font32IconData.size());

		auto csharpScriptIconData = LoadEmbeddedTexture(IDB_ICON_CSHARP_SCRIPT);
		CPResources::ICON_CSHARP_SCRIPT = Vesuvius::VSTexture2D::Create(csharpScriptIconData.data(), csharpScriptIconData.size());

		auto directoryIconData = LoadEmbeddedTexture(IDB_ICON_DIRECTORY);
		CPResources::ICON_DIRECTORY = Vesuvius::VSTexture2D::Create(directoryIconData.data(), directoryIconData.size());

		auto directoryOpenIconData = LoadEmbeddedTexture(IDB_ICON_DIRECTORY_OPEN);
		CPResources::ICON_DIRECTORY_OPEN = Vesuvius::VSTexture2D::Create(directoryOpenIconData.data(), directoryOpenIconData.size());

		auto fileIconData = LoadEmbeddedTexture(IDB_ICON_FILE);
		CPResources::ICON_FILE = Vesuvius::VSTexture2D::Create(fileIconData.data(), fileIconData.size());

		auto backIconData = LoadEmbeddedTexture(IDB_ICON_BACK);
		CPResources::ICON_BACK = Vesuvius::VSTexture2D::Create(backIconData.data(), backIconData.size());

		auto plusIconData = LoadEmbeddedTexture(IDB_ICON_PLUS);
		CPResources::ICON_PLUS = Vesuvius::VSTexture2D::Create(plusIconData.data(), plusIconData.size());

		auto csharpLogo320IconData = LoadEmbeddedTexture(IDB_ICON_CSHARP_LOGO_320);
		CPResources::ICON_CSHARP_LOGO_320 = Vesuvius::VSTexture2D::Create(csharpLogo320IconData.data(), csharpLogo320IconData.size());

		auto xIconData = LoadEmbeddedTexture(IDB_ICON_X);
		CPResources::ICON_X = Vesuvius::VSTexture2D::Create(xIconData.data(), xIconData.size());

		auto cataclysmWhiteIconData = LoadEmbeddedTexture(IDB_ICON_CATACLYSM_WHITE);
		CPResources::ICON_CATACLYSM_WHITE = Vesuvius::VSTexture2D::Create(cataclysmWhiteIconData.data(), cataclysmWhiteIconData.size());

		auto listIconData = LoadEmbeddedTexture(IDB_ICON_LIST);
		CPResources::ICON_LIST = Vesuvius::VSTexture2D::Create(listIconData.data(), listIconData.size());

		auto tilesIconData = LoadEmbeddedTexture(IDB_ICON_TILES);
		CPResources::ICON_TILES = Vesuvius::VSTexture2D::Create(tilesIconData.data(), tilesIconData.size());

		auto tilesSmallIconData = LoadEmbeddedTexture(IDB_ICON_TILES_SMALL);
		CPResources::ICON_TILES_SMALL = Vesuvius::VSTexture2D::Create(tilesSmallIconData.data(), tilesSmallIconData.size());

		auto tilesSmallFadedIconData = LoadEmbeddedTexture(IDB_ICON_TILES_SMALL_FADED);
		CPResources::ICON_TILES_SMALL_FADED = Vesuvius::VSTexture2D::Create(tilesSmallFadedIconData.data(), tilesSmallFadedIconData.size());

		auto listFadedIconData = LoadEmbeddedTexture(IDB_ICON_LIST_FADED);
		CPResources::ICON_LIST_FADED = Vesuvius::VSTexture2D::Create(listFadedIconData.data(), listFadedIconData.size());

		auto tilesFadedIconData = LoadEmbeddedTexture(IDB_ICON_TILES_FADED);
		CPResources::ICON_TILES_FADED = Vesuvius::VSTexture2D::Create(tilesFadedIconData.data(), tilesFadedIconData.size());

		auto uiScaleIconData = LoadEmbeddedTexture(IDB_ICON_UI_SCALE);
		CPResources::ICON_UI_SCALE = Vesuvius::VSTexture2D::Create(uiScaleIconData.data(), uiScaleIconData.size());

		auto lockIconData = LoadEmbeddedTexture(IDB_ICON_LOCK);
		CPResources::ICON_LOCK = Vesuvius::VSTexture2D::Create(lockIconData.data(), lockIconData.size());

		auto unlockIconData = LoadEmbeddedTexture(IDB_ICON_UNLOCK);
		CPResources::ICON_UNLOCK = Vesuvius::VSTexture2D::Create(unlockIconData.data(), unlockIconData.size());

		auto copyIconData = LoadEmbeddedTexture(IDB_ICON_COPY);
		CPResources::ICON_COPY = Vesuvius::VSTexture2D::Create(copyIconData.data(), copyIconData.size());

		auto pasteIconData = LoadEmbeddedTexture(IDB_ICON_PASTE);
		CPResources::ICON_PASTE = Vesuvius::VSTexture2D::Create(pasteIconData.data(), pasteIconData.size());

		CPStyles::SetUIScale(1.0f);
	}

	std::vector<uint8_t> CPResources::LoadEmbeddedTexture(int resourceID)
	{
		HMODULE hModule = GetModuleHandle(nullptr);
		HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(resourceID), RT_RCDATA);
		if (!hResource)
			return {};

		HGLOBAL hLoadedResource = LoadResource(hModule, hResource);
		if (hLoadedResource == 0)
		{
			CC_CORE_ERROR("[CPResources::LoadEmbeddedTexture] Resource not found!");
			return {};
		}
		else
		{
			void* pLockedResource = LockResource(hLoadedResource);
			size_t resourceSize = SizeofResource(hModule, hResource);

			std::vector<uint8_t> data(resourceSize);
			memcpy(data.data(), pLockedResource, resourceSize);
			return data;
		}
	}
}
