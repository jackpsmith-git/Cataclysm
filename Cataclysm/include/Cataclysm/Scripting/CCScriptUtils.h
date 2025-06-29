#pragma once

#include "Cataclysm.h"
#include "Cataclysm/Core/CCFile.h"

#include "CCScriptEngine.h"
#include "CCScriptFieldType.h"
#include "Cataclysm/Scene/CCSceneEntityManager.h"

#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/mono-debug.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace Cataclysm
{
	static std::unordered_map<std::string, CCScriptFieldType> s_ScriptFieldTypeMap =
	{
		{ "System.Single",				CCScriptFieldType::Float },
		{ "System.Double",				CCScriptFieldType::Double },
		{ "System.Boolean",				CCScriptFieldType::Bool },
		{ "System.Char",				CCScriptFieldType::Char },
		{ "System.Int16",				CCScriptFieldType::Short },
		{ "System.Int32",				CCScriptFieldType::Int },
		{ "System.Int64",				CCScriptFieldType::Long },
		{ "System.Byte",				CCScriptFieldType::Byte },
		{ "System.UInt16",				CCScriptFieldType::UShort },
		{ "System.UInt32",				CCScriptFieldType::UInt },
		{ "System.UInt64",				CCScriptFieldType::ULong },
		{ "System.String",				CCScriptFieldType::String },

		{ "Cataclysm.Math.Vec2",		CCScriptFieldType::Vec2 },
		{ "Cataclysm.Math.Vec3",		CCScriptFieldType::Vec3 },
		{ "Cataclysm.Math.Vec4",		CCScriptFieldType::Vec4 },

		{ "Cataclysm.Entity",			CCScriptFieldType::Entity },

		{ "Cataclysm.AudioSource",		CCScriptFieldType::AudioSource },
		{ "Cataclysm.BoxCollider2D",	CCScriptFieldType::BoxCollider2D },
		{ "Cataclysm.Camera",			CCScriptFieldType::Camera },
		{ "Cataclysm.CircleCollider2D",	CCScriptFieldType::CircleCollider2D },
		{ "Cataclysm.CircleRenderer",	CCScriptFieldType::CircleRenderer },
		{ "Cataclysm.Rigidbody2D",		CCScriptFieldType::Rigidbody2D },
		{ "Cataclysm.SpriteRenderer",	CCScriptFieldType::SpriteRenderer },
		{ "Cataclysm.Text",				CCScriptFieldType::Text },
		{ "Cataclysm.Transform",		CCScriptFieldType::Transform },

		{ "Cataclysm.Texture2D",		CCScriptFieldType::Texture2D },
		{ "Cataclysm.Font",				CCScriptFieldType::Font },
		{ "Cataclysm.AudioClip",		CCScriptFieldType::AudioClip }
	};

	namespace ScriptUtils
	{
		inline std::string MonoStringToString(MonoString* string)
		{
			char* cStr = mono_string_to_utf8(string);
			std::string str(cStr);
			mono_free(cStr);
			return str;
		}

		inline MonoString* StringToMonoString(const std::string& str) { return mono_string_new(CCScriptEngine::GetAppDomain(), str.c_str()); }

		inline static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath, bool loadPDB = false)
		{
			CCScopedBuffer fileData = CCFile::Read(assemblyPath);

			// We can't use this image for anything other than loading the assembly because 
			// this image doesn't have a reference to the assembly
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData.As<char>(), fileData.Size(), 1, &status, 0);

			if (status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				CC_CORE_ERROR(errorMessage);
				return nullptr;
			}

			if (loadPDB)
			{
				std::filesystem::path pdbPath = assemblyPath;
				pdbPath.replace_extension(".pdb");

				if (std::filesystem::exists(pdbPath))
				{
					CCScopedBuffer pdbFileData = CCFile::Read(pdbPath);
					mono_debug_open_image_from_memory(image, pdbFileData.As<const mono_byte>(), pdbFileData.Size());
				}
			}

			std::string pathString = assemblyPath.string();
			MonoAssembly* assembly = mono_assembly_load_from_full(image, pathString.c_str(), &status, 0);
			mono_image_close(image);

			return assembly;
		}

		inline static void PrintAssemblyTypes(MonoAssembly* assembly)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			for (int32_t i = 0; i < numTypes; i++)
			{
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				CC_CORE_TRACE("{}.{}", nameSpace, name);
			}
		}

		inline static CCScriptFieldType MonoTypeToScriptFieldType(MonoType* monoType)
		{
			std::string typeName = mono_type_get_name(monoType);

			auto it = s_ScriptFieldTypeMap.find(typeName);
			if (it == s_ScriptFieldTypeMap.end())
			{
				CC_CORE_WARN("[CCScriptEngine::MonoTypeToScriptFieldType] Cannot serialize field of type: \"{}\"", typeName);
				return CCScriptFieldType::None;
			}

			return it->second;
		}

		inline const char* ScriptFieldTypeToString(CCScriptFieldType fieldType)
		{
			switch (fieldType)
			{
			case CCScriptFieldType::Float:				return "Float";
			case CCScriptFieldType::Double:				return "Double";
			case CCScriptFieldType::Bool:				return "Bool";
			case CCScriptFieldType::Char:				return "Char";
			case CCScriptFieldType::Byte:				return "Byte";
			case CCScriptFieldType::Short:				return "Short";
			case CCScriptFieldType::Int:				return "Int";
			case CCScriptFieldType::Long:				return "Long";
			case CCScriptFieldType::UByte:				return "UByte";
			case CCScriptFieldType::UShort:				return "UShort";
			case CCScriptFieldType::UInt:				return "UInt";
			case CCScriptFieldType::ULong:				return "ULong";
			case CCScriptFieldType::Vec2:				return "Vec2";
			case CCScriptFieldType::Vec3:				return "Vec3";
			case CCScriptFieldType::Vec4:				return "Vec4";
			case CCScriptFieldType::Entity:				return "Entity";
			case CCScriptFieldType::String:				return "String";
			case CCScriptFieldType::AudioSource:		return "AudioSource";
			case CCScriptFieldType::BoxCollider2D:		return "BoxCollider2D";
			case CCScriptFieldType::Camera:				return "Camera";
			case CCScriptFieldType::CircleCollider2D:	return "CircleCollider2D";
			case CCScriptFieldType::CircleRenderer:		return "CircleRenderer";
			case CCScriptFieldType::Rigidbody2D:		return "Rigidbody2D";
			case CCScriptFieldType::SpriteRenderer:		return "SpriteRenderer";
			case CCScriptFieldType::Text:				return "Text";
			case CCScriptFieldType::Transform:			return "Transform";
			case CCScriptFieldType::Texture2D:			return "Texture2D";
			case CCScriptFieldType::Font:				return "Font";
			case CCScriptFieldType::AudioClip:			return "AudioClip";
			}

			CC_CORE_ASSERT(false, "Unknown ScriptFieldType");
			return "None";
		}

		inline CCScriptFieldType ScriptFieldTypeFromString(std::string_view fieldType)
		{
			if (fieldType == "None")				return CCScriptFieldType::None;
			if (fieldType == "Float")				return CCScriptFieldType::Float;
			if (fieldType == "Double")				return CCScriptFieldType::Double;
			if (fieldType == "Bool")				return CCScriptFieldType::Bool;
			if (fieldType == "Char")				return CCScriptFieldType::Char;
			if (fieldType == "Byte")				return CCScriptFieldType::Byte;
			if (fieldType == "Short")				return CCScriptFieldType::Short;
			if (fieldType == "Int")					return CCScriptFieldType::Int;
			if (fieldType == "Long")				return CCScriptFieldType::Long;
			if (fieldType == "UByte")				return CCScriptFieldType::UByte;
			if (fieldType == "UShort")				return CCScriptFieldType::UShort;
			if (fieldType == "UInt")				return CCScriptFieldType::UInt;
			if (fieldType == "ULong")				return CCScriptFieldType::ULong;
			if (fieldType == "Vec2")				return CCScriptFieldType::Vec2;
			if (fieldType == "Vec3")				return CCScriptFieldType::Vec3;
			if (fieldType == "Vec4")				return CCScriptFieldType::Vec4;
			if (fieldType == "Entity")				return CCScriptFieldType::Entity;
			if (fieldType == "String")				return CCScriptFieldType::String;
			if (fieldType == "AudioSource")			return CCScriptFieldType::AudioSource;
			if (fieldType == "BoxCollider2D")		return CCScriptFieldType::BoxCollider2D;
			if (fieldType == "Camera")				return CCScriptFieldType::Camera;
			if (fieldType == "CircleCollider2D")	return CCScriptFieldType::CircleCollider2D;
			if (fieldType == "CircleRenderer")		return CCScriptFieldType::CircleRenderer;
			if (fieldType == "Rigidbody2D")			return CCScriptFieldType::Rigidbody2D;
			if (fieldType == "SpriteRenderer")		return CCScriptFieldType::SpriteRenderer;
			if (fieldType == "Text")				return CCScriptFieldType::Text;
			if (fieldType == "Transform")			return CCScriptFieldType::Transform;
			if (fieldType == "Texture2D")			return CCScriptFieldType::Texture2D;
			if (fieldType == "Font")				return CCScriptFieldType::Font;
			if (fieldType == "AudioClip")			return CCScriptFieldType::AudioClip;

			CC_CORE_ASSERT(false, "Unknown ScriptFieldType");
			return CCScriptFieldType::None;
		}

		inline static bool IsReferenceType(CCScriptFieldType type)
		{
			return type == CCScriptFieldType::Entity ||
				type == CCScriptFieldType::Texture2D ||
				type == CCScriptFieldType::Font ||
				type == CCScriptFieldType::AudioClip;
		}
	}
}
