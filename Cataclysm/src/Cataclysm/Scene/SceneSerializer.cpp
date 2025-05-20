#include "ccpch.h"
#include "SceneSerializer.h"

// cataclysm
#include "Cataclysm/ECS/Entity.h"
#include "Cataclysm/ECS/Components.h"
#include "Cataclysm/Audio/AudioEngine.h"
#include "Cataclysm/Scripting/ScriptEngine.h"
#include "Cataclysm/Core/UUID.h"
#include "Cataclysm/Project/Project.h"

// std
#include <fstream>

// yaml
#include <yaml-cpp/yaml.h>

namespace YAML
{
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<Cataclysm::UUID>
	{
		static Node encode(const Cataclysm::UUID& uuid)
		{
			Node node;
			node.push_back((uint64_t)uuid);
			return node;
		}

		static bool decode(const Node& node, Cataclysm::UUID& uuid)
		{
			uuid = node.as<uint64_t>();
			return true;
		}
	};
}

namespace Cataclysm
{
#define WRITE_SCRIPT_FIELD(FieldType, Type)				\
	case ScriptFieldType::FieldType:					\
		out << scriptField.GetValue<Type>();			\
		break;

#define READ_SCRIPT_FIELD(FieldType, Type)				\
	case ScriptFieldType::FieldType:					\
	{													\
		Type data = scriptField["Data"].as<Type>();		\
		fieldInstance.SetValue(data);					\
		break;											\
	}													

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	static std::string RigidBody2DBodyTypeToString(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
		case Rigidbody2DComponent::BodyType::Static:    return "Static";
		case Rigidbody2DComponent::BodyType::Dynamic:   return "Dynamic";
		case Rigidbody2DComponent::BodyType::Kinematic: return "Kinematic";
		}

		CC_CORE_ASSERT(false, "Unknown body type");
		return {};
	}

	static Rigidbody2DComponent::BodyType RigidBody2DBodyTypeFromString(const std::string& bodyTypeString)
	{
		if (bodyTypeString == "Static")    return Rigidbody2DComponent::BodyType::Static;
		if (bodyTypeString == "Dynamic")   return Rigidbody2DComponent::BodyType::Dynamic;
		if (bodyTypeString == "Kinematic") return Rigidbody2DComponent::BodyType::Kinematic;

		CC_CORE_ASSERT(false, "Unknown body type");
		return Rigidbody2DComponent::BodyType::Static;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{

	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		CC_CORE_WARN("[SceneSerializer::Serialize] Saving in progress. Do not close app or edit scene data!");

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "New Scene";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID)
			{
				Entity entity = { entityID, m_Scene.get() };
				if (!entity)
					return;

				CC_CORE_ASSERT("Serialization Failed!", entity.HasComponent<IDComponent>());

				out << YAML::BeginMap;
				out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();
				out << YAML::Key << "Enabled" << YAML::Value << entity.GetComponent<IDComponent>().Enabled;

				/// TAG COMPONENT
				if (entity.HasComponent<TagComponent>())
				{
					out << YAML::Key << "TagComponent";
					out << YAML::BeginMap;

					auto& tag = entity.GetComponent<TagComponent>().Tag;
					out << YAML::Key << "Tag" << YAML::Value << tag;

					out << YAML::EndMap;
				}

				// TRANSFORM COMPONENT
				if (entity.HasComponent<TransformComponent>())
				{
					out << YAML::Key << "TransformComponent";
					out << YAML::BeginMap;

					auto& tc = entity.GetComponent<TransformComponent>();
					out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
					out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
					out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

					out << YAML::EndMap;
				}

				// CAMERA COMPONENT
				if (entity.HasComponent<CameraComponent>())
				{
					out << YAML::Key << "CameraComponent";
					out << YAML::BeginMap;

					auto& cameraComponent = entity.GetComponent<CameraComponent>();
					auto& camera = cameraComponent.Camera;

					out << YAML::Key << "Camera" << YAML::Value;
					out << YAML::BeginMap;
					out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
					out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
					out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
					out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
					out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
					out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
					out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
					out << YAML::EndMap;

					out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
					out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

					out << YAML::EndMap;
				}

				// RIGIDBODY 2D COMPONENT
				if (entity.HasComponent<Rigidbody2DComponent>())
				{
					out << YAML::Key << "Rigidbody2DComponent";
					out << YAML::BeginMap;

					auto& rb2dComponent = entity.GetComponent<Rigidbody2DComponent>();
					out << YAML::Key << "BodyType" << YAML::Value << RigidBody2DBodyTypeToString(rb2dComponent.Type);
					out << YAML::Key << "FixedRotation" << YAML::Value << rb2dComponent.FixedRotation;
					out << YAML::Key << "GravityScale" << YAML::Value << rb2dComponent.GravityScale;
					out << YAML::Key << "Mass" << YAML::Value << rb2dComponent.Mass;

					out << YAML::EndMap;
				}

				// BOX COLLIDER 2D COMPONENT
				if (entity.HasComponent<BoxCollider2DComponent>())
				{
					out << YAML::Key << "BoxCollider2DComponent";
					out << YAML::BeginMap;

					auto& bc2dComponent = entity.GetComponent<BoxCollider2DComponent>();
					out << YAML::Key << "Offset" << YAML::Value << bc2dComponent.Offset;
					out << YAML::Key << "Size" << YAML::Value << bc2dComponent.Size;
					out << YAML::Key << "Density" << YAML::Value << bc2dComponent.Density;
					out << YAML::Key << "Friction" << YAML::Value << bc2dComponent.Friction;
					out << YAML::Key << "Restitution" << YAML::Value << bc2dComponent.Restitution;
					out << YAML::Key << "RestitutionThreshold" << YAML::Value << bc2dComponent.RestitutionThreshold;
					out << YAML::Key << "IsTrigger" << YAML::Value << bc2dComponent.IsTrigger;

					out << YAML::EndMap;
				}

				// CIRCLE COLLIDER 2D COMPONENT
				if (entity.HasComponent<CircleCollider2DComponent>())
				{
					out << YAML::Key << "CircleCollider2DComponent";
					out << YAML::BeginMap;

					auto& cc2dComponent = entity.GetComponent<CircleCollider2DComponent>();
					out << YAML::Key << "Offset" << YAML::Value << cc2dComponent.Offset;
					out << YAML::Key << "Radius" << YAML::Value << cc2dComponent.Radius;
					out << YAML::Key << "Density" << YAML::Value << cc2dComponent.Density;
					out << YAML::Key << "Friction" << YAML::Value << cc2dComponent.Friction;
					out << YAML::Key << "Restitution" << YAML::Value << cc2dComponent.Restitution;
					out << YAML::Key << "RestitutionThreshold" << YAML::Value << cc2dComponent.RestitutionThreshold;
					out << YAML::Key << "IsTrigger" << YAML::Value << cc2dComponent.IsTrigger;

					out << YAML::EndMap;
				}

				// MONOSCRIPT COMPONENT
				if (entity.HasComponent<MonoScriptComponent>())
				{
					auto& scriptComponent = entity.GetComponent<MonoScriptComponent>();

					out << YAML::Key << "ScriptComponent";
					out << YAML::BeginMap;
					out << YAML::Key << "ClassName" << YAML::Value << scriptComponent.ClassName;

					// Fields
					Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(scriptComponent.ClassName);
					const auto& fields = entityClass->GetFields();
					if (fields.size() > 0)
					{
						out << YAML::Key << "ScriptFields" << YAML::Value;
						auto& entityFields = ScriptEngine::GetScriptFieldMap(entity);
						out << YAML::BeginSeq;
						for (const auto& [name, field] : fields)
						{
							if (entityFields.find(name) == entityFields.end())
								continue;

							out << YAML::BeginMap;
							out << YAML::Key << "Name" << YAML::Value << name;
							out << YAML::Key << "Type" << YAML::Value << Utils::ScriptFieldTypeToString(field.Type);

							out << YAML::Key << "Data" << YAML::Value;
							ScriptFieldInstance& scriptField = entityFields.at(name);

							switch (field.Type)
							{
								WRITE_SCRIPT_FIELD(Float, float);
								WRITE_SCRIPT_FIELD(Double, double);
								WRITE_SCRIPT_FIELD(Bool, bool);
								WRITE_SCRIPT_FIELD(Char, char);
								WRITE_SCRIPT_FIELD(Byte, int8_t);
								WRITE_SCRIPT_FIELD(Short, int16_t);
								WRITE_SCRIPT_FIELD(Int, int32_t);
								WRITE_SCRIPT_FIELD(Long, int64_t);
								WRITE_SCRIPT_FIELD(UByte, uint8_t);
								WRITE_SCRIPT_FIELD(UShort, uint16_t);
								WRITE_SCRIPT_FIELD(UInt, uint32_t);
								WRITE_SCRIPT_FIELD(ULong, uint64_t);
								WRITE_SCRIPT_FIELD(Vec2, glm::vec2);
								WRITE_SCRIPT_FIELD(Vec3, glm::vec3);
								WRITE_SCRIPT_FIELD(Vec4, glm::vec4);
								WRITE_SCRIPT_FIELD(Entity, UUID);
							}
							out << YAML::EndMap;
						}
						out << YAML::EndSeq;
					}

					out << YAML::EndMap;
				}

				// SPRITE RENDERER COMPONENT
				if (entity.HasComponent<SpriteRendererComponent>())
				{
					out << YAML::Key << "SpriteRendererComponent";
					out << YAML::BeginMap;

					auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
					out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;

					if (spriteRendererComponent.Texture)
					{
						std::string sub = "Textures\\";
						std::string path = spriteRendererComponent.Texture->GetPath();
						size_t position = path.find(sub);

						out << YAML::Key << "TexturePath" << YAML::Value << path.substr(position);
					}

					out << YAML::Key << "TilingFactor" << YAML::Value << spriteRendererComponent.TilingFactor;

					out << YAML::EndMap;
				}

				// CIRCLE RENDERER COMPONENT
				if (entity.HasComponent<CircleRendererComponent>())
				{
					out << YAML::Key << "CircleRendererComponent";
					out << YAML::BeginMap;
					auto& circleRendererComponent = entity.GetComponent<CircleRendererComponent>();
					out << YAML::Key << "Color" << YAML::Value << circleRendererComponent.Color;
					out << YAML::Key << "Thickness" << YAML::Value << circleRendererComponent.Thickness;
					out << YAML::Key << "Fade" << YAML::Value << circleRendererComponent.Fade;
					out << YAML::EndMap;
				}

				// TEXT COMPONENT
				if (entity.HasComponent<TextComponent>())
				{
					out << YAML::Key << "TextComponent";
					out << YAML::BeginMap; // TextComponent
					auto& textComponent = entity.GetComponent<TextComponent>();
					out << YAML::Key << "TextString" << YAML::Value << textComponent.TextString;
					out << YAML::Key << "FontPath" << YAML::Value << textComponent.FontPath;
					out << YAML::Key << "Color" << YAML::Value << textComponent.Color;
					out << YAML::Key << "Kerning" << YAML::Value << textComponent.Kerning;
					out << YAML::Key << "LineSpacing" << YAML::Value << textComponent.LineSpacing;
					out << YAML::EndMap; // TextComponent
				}

				// AUDIO COMPONENT
				if (entity.HasComponent<AudioSourceComponent>())
				{
					out << YAML::Key << "AudioSourceComponent";
					out << YAML::BeginMap;
					auto& audioSourceComponent = entity.GetComponent<AudioSourceComponent>();
					out << YAML::Key << "FilePath" << YAML::Value << audioSourceComponent.FilePath;
					out << YAML::Key << "Loop" << YAML::Value << audioSourceComponent.Loop;
					out << YAML::Key << "PlayOnStart" << YAML::Value << audioSourceComponent.PlayOnStart;
					out << YAML::Key << "Volume" << YAML::Value << audioSourceComponent.Volume;
					out << YAML::EndMap;
				}

				if (m_Scene->GetParent(entity.GetUUID()) != 0)
				{
					UUID parentID = m_Scene->GetEntityParentMap()[entity.GetUUID()];
					out << YAML::Key << "Parent" << YAML::Value << parentID;
				}

				out << YAML::EndMap;

			});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();

		CC_CORE_INFO("[SceneSerializer::Serialize] Scene saved!");
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		// Not implemented
		// CC_CORE_ASSERT(false);
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath);
		}
		catch (YAML::ParserException e)
		{
			CC_CORE_ERROR("Failed to load .ccscene file '{0}'\n		{1}", filepath, e.what());
			return false;
		}

		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		// CC_CORE_TRACE("[SceneSerilaizer::Deserialize] Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			std::vector<std::pair<UUID, UUID>> pendingParentLinks;

			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>(); // TODO

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				// CC_CORE_TRACE("[SceneSerializer::Deserialize] Deserialized entity '{1}' ({0})", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntityWithUUID(uuid, name);

				auto& idComponent = deserializedEntity.GetComponent<IDComponent>();
				idComponent.Enabled = entity["Enabled"].as<bool>();

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto& cameraProps = cameraComponent["Camera"];
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto scriptComponent = entity["ScriptComponent"];
				if (scriptComponent)
				{
					auto& sc = deserializedEntity.AddComponent<MonoScriptComponent>();
					sc.ClassName = scriptComponent["ClassName"].as<std::string>();

					auto scriptFields = scriptComponent["ScriptFields"];
					if (scriptFields)
					{
						Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(sc.ClassName);
						if (entityClass)
						{
							const auto& fields = entityClass->GetFields();
							auto& entityFields = ScriptEngine::GetScriptFieldMap(deserializedEntity);

							for (auto scriptField : scriptFields)
							{
								std::string name = scriptField["Name"].as<std::string>();
								std::string typeString = scriptField["Type"].as<std::string>();
								ScriptFieldType type = Utils::ScriptFieldTypeFromString(typeString);
								ScriptFieldInstance& fieldInstance = entityFields[name];

								CC_CORE_ASSERT(fields.find(name) != fields.end(), "Field not found!");

								if (fields.find(name) == fields.end())
									continue;
								
								fieldInstance.Field = fields.at(name);

								switch (type)
								{
									READ_SCRIPT_FIELD(Float, float);
									READ_SCRIPT_FIELD(Double, double);
									READ_SCRIPT_FIELD(Bool, bool);
									READ_SCRIPT_FIELD(Char, char);
									READ_SCRIPT_FIELD(Byte, int8_t);
									READ_SCRIPT_FIELD(Short, int16_t);
									READ_SCRIPT_FIELD(Int, int32_t);
									READ_SCRIPT_FIELD(Long, int64_t);
									READ_SCRIPT_FIELD(UByte, uint8_t);
									READ_SCRIPT_FIELD(UShort, uint16_t);
									READ_SCRIPT_FIELD(UInt, uint32_t);
									READ_SCRIPT_FIELD(ULong, uint64_t);
									READ_SCRIPT_FIELD(Vec2, glm::vec2);
									READ_SCRIPT_FIELD(Vec3, glm::vec3);
									READ_SCRIPT_FIELD(Vec4, glm::vec4);
									READ_SCRIPT_FIELD(Entity, UUID);
								}
							}
						}
					}
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					src.Color = spriteRendererComponent["Color"].as<glm::vec4>();

					if (spriteRendererComponent["TexturePath"])
					{
						std::string texturePath = spriteRendererComponent["TexturePath"].as<std::string>();
						auto path = Project::GetAssetFileSystemPath(texturePath);
						src.Texture = Texture2D::Create(path.string());

					}

					if (spriteRendererComponent["TilingFactor"])
						src.TilingFactor = spriteRendererComponent["TilingFactor"].as<float>();
				}

				auto circleRendererComponent = entity["CircleRendererComponent"];
				if (circleRendererComponent)
				{
					auto& crc = deserializedEntity.AddComponent<CircleRendererComponent>();
					crc.Color = circleRendererComponent["Color"].as<glm::vec4>();
					crc.Thickness = circleRendererComponent["Thickness"].as<float>();
					crc.Fade = circleRendererComponent["Fade"].as<float>();
				}

				auto rigidbody2DComponent = entity["Rigidbody2DComponent"];
				if (rigidbody2DComponent)
				{
					auto& rb2d = deserializedEntity.AddComponent<Rigidbody2DComponent>();
					rb2d.Type = RigidBody2DBodyTypeFromString(rigidbody2DComponent["BodyType"].as<std::string>());
					rb2d.FixedRotation = rigidbody2DComponent["FixedRotation"].as<bool>();
					rb2d.GravityScale = rigidbody2DComponent["GravityScale"].as<float>();
					rb2d.Mass = rigidbody2DComponent["Mass"].as<float>();
				}

				auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
				if (boxCollider2DComponent)
				{
					auto& bc2d = deserializedEntity.AddComponent<BoxCollider2DComponent>();
					bc2d.Offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
					bc2d.Size = boxCollider2DComponent["Size"].as<glm::vec2>();
					bc2d.Density = boxCollider2DComponent["Density"].as<float>();
					bc2d.Friction = boxCollider2DComponent["Friction"].as<float>();
					bc2d.Restitution = boxCollider2DComponent["Restitution"].as<float>();
					bc2d.RestitutionThreshold = boxCollider2DComponent["RestitutionThreshold"].as<float>();
					bc2d.IsTrigger = boxCollider2DComponent["IsTrigger"].as<bool>();
				}

				auto circleCollider2DComponent = entity["CircleCollider2DComponent"];
				if (circleCollider2DComponent)
				{
					auto& cc2d = deserializedEntity.AddComponent<CircleCollider2DComponent>();
					cc2d.Offset = circleCollider2DComponent["Offset"].as<glm::vec2>();
					cc2d.Radius = circleCollider2DComponent["Radius"].as<float>();
					cc2d.Density = circleCollider2DComponent["Density"].as<float>();
					cc2d.Friction = circleCollider2DComponent["Friction"].as<float>();
					cc2d.Restitution = circleCollider2DComponent["Restitution"].as<float>();
					cc2d.RestitutionThreshold = circleCollider2DComponent["RestitutionThreshold"].as<float>();
					cc2d.IsTrigger = circleCollider2DComponent["IsTrigger"].as<bool>();
				}

				auto textComponent = entity["TextComponent"];
				if (textComponent)
				{
					auto& tc = deserializedEntity.AddComponent<TextComponent>();
					tc.TextString = textComponent["TextString"].as<std::string>();
					tc.Color = textComponent["Color"].as<glm::vec4>();
					tc.Kerning = textComponent["Kerning"].as<float>();
					tc.LineSpacing = textComponent["LineSpacing"].as<float>();
					tc.FontPath = textComponent["FontPath"].as<std::string>();
					tc.FontAsset = Font::GetFont(tc.FontPath);
				}

				auto audioSourceComponent = entity["AudioSourceComponent"];
				if (audioSourceComponent)
				{
					auto& asc = deserializedEntity.AddComponent<AudioSourceComponent>();
					asc.FilePath = audioSourceComponent["FilePath"].as<std::string>();
					asc.Loop = audioSourceComponent["Loop"].as<bool>();
					asc.PlayOnStart = audioSourceComponent["PlayOnStart"].as<bool>();
					asc.Volume = audioSourceComponent["Volume"].as<float>();

					if (!asc.FilePath.empty())
					{
						asc.AudioClip = AudioEngine::LoadClip(asc.FilePath);
						if (asc.AudioClip)
						{
							asc.AudioClip->SetLooping(asc.Loop);
							asc.AudioClip->SetVolume(asc.Volume);
						}
					}
				}

				if (entity["Parent"])
				{
					UUID parentID = entity["Parent"].as<UUID>();
					pendingParentLinks.push_back({ uuid, parentID });
				}
			}

			for (auto& [childUUID, parentUUID] : pendingParentLinks)
			{
				m_Scene->SetParentOnLoad(childUUID, parentUUID);
			}

			for (auto& [child, parent] : m_Scene->GetEntityParentMap())
			{
				m_Scene->GetEntityChildrenMap().insert({ parent, child });
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		// Not implemented
		// CC_CORE_ASSERT(false);
		return false;
	}
}
