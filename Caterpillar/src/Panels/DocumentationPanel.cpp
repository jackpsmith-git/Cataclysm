#include "ccpch.h"
#include "DocumentationPanel.h"

#include "Cataclysm.h"

#include <imgui/imgui.h>

namespace Cataclysm
{
	DocumentationPanel::DocumentationPanel()
	{

	}

	std::vector<DocEntry> DocumentationPanel::s_Documentation = {
	{
		"Cataclysm.AudioSourceComponent",
		"CCML/Source/Cataclysm/ECS/AudioSourceComponent.cs",
		"Cataclysm AudioSource Component.",
		{},
		{
			{ "Play()", "void", "Plays the audio clip" },
			{ "Pause()", "void", "Pauses the audio clip" },
			{ "Stop()", "void", "Stops the audio clip" }
		}
	},
	{
		"Cataclysm.BoxCollider2DComponent",
		"CCML/Source/Cataclysm/ECS/BoxCollider2DComponent.cs",
		"Cataclysm BoxCollider2D Component.",
		{
			{ "Density", "float", "Collider density" },
			{ "Friction", "float", "Collider friction" },
			{ "IsTrigger", "bool", "If set to true, ignores all physics interactions" },
			{ "Offset", "Cataclysm.Vec2", "Collider offset" },
			{ "Restitution", "float", "Surface restitution" },
			{ "RestitutionThreshold", "float", "Surface restitution threshold" },
			{ "Size", "Cataclysm.Vec2", "Collider size" }
		},
		{
			{ "Reset()", "void", "Resets all properties to default values" }
		}
	},
	{
		"Cataclysm.CameraComponent",
		"CCML/Source/Cataclysm/ECS/CameraComponent.cs",
		"Cataclysm Camera Component.",
		{
			{ "FixedAspectRatio", "bool", "" },
			{ "OrthographicFarClip", "float", "" },
			{ "OrthographicNearClip", "float", "" },
			{ "OrthographicSize", "float", "" },
			{ "PerspectiveFarClip", "float", "" },
			{ "PerspectiveNearClip", "float", "" },
			{ "PerspectiveVerticalFOV", "float", "" },
			{ "Primary", "bool", "Is primary scene camera" }
		},
		{
			{ "Reset()", "void", "Resets all properties to default values." }
		}
	},
	{
		"Cataclysm.CircleCollider2DComponent",
		"CCML/Source/Cataclysm/ECS/CircleCollider2DComponent.cs",
		"Cataclysm CircleCollider2D Component.",
		{
			{ "Density", "float", "Collider density" },
			{ "Friction", "float", "Collider friction" },
			{ "Offset", "Cataclysm.Vec2", "Collider offset" },
			{ "Radius", "float", "Collider radius" },
			{ "Restitution", "float", "Surface restitution" },
			{ "RestitutionThreshold", "float", "Surface restitution threshold" },
			{ "IsTrigger", "bool", "If set to true, ignores all physics interactions" }
		},
		{
			{ "Reset()", "void", "Resets all properties to default values" }
		}
	},
	{
		"Cataclysm.CircleRendererComponent",
		"CCML/Source/Cataclysm/ECS/CircleRendererComponent.cs",
		"Cataclysm CircleRenderer Component.",
		{
			{ "Color", "Cataclysm.Vec4", "Vertex color" },
			{ "Fade", "float", "Border fade" },
			{ "Thickness", "float", "Border thickness" }
		},
		{
			{ "Reset()", "void", "Resets all properties to default values." }
		}
	},
	{
		"Cataclysm.Component",
		"CCML/Source/Cataclysm/ECS/Component.cs",
		"Cataclysm Component.",
		{
			{ "Entity", "Cataclysm.Entity", "Parent entity of the component" }
		},
		{}
	},
	{
		"Cataclysm.Debug",
		"CCML/Source/Cataclysm/Debug.cs",
		"Static class containing Cataclysm debug methods.",
		{},
		{
			{ "Error(string message)", "static void", "Logs a message to the console as an error and pauses the active scene if it exists and is running" },
			{ "Error(object obj)", "static void", "Logs an object to the console as an error and pauses the active scene if it exists and is running" },
			{ "Info(string message)", "static void", "Logs a message to the console as info" },
			{ "Info(object obj)", "static void", "Logs an object to the console as info" },
			{ "Log(string message)", "static void", "Logs a message to the console as a trace" },
			{ "Log(object obj)", "static void", "Logs an object to the console as a trace" },
			{ "Warn(string message)", "static void", "Logs a message to the console as a warning" },
			{ "Warn(object obj)", "static void", "Logs an object to the console as a warning" }
		}
	},
	{
		"Cataclysm.Entity",
		"CCML/Source/Cataclysm/ECS/Entity.cs",
		"Cataclysm Entity.",
		{
			{ "Enabled", "bool", "Returns true if the entity is enabled in the hierarchy, otherwise returns false" },
			{ "ID", "readonly ulong", "UUID" },
			{ "Rotation", "Cataclysm.Vec3", "The entity’s local rotation in Euler angles" },
			{ "Scale", "Cataclysm.Vec3", "The entity’s local scale" },
			{ "Transform", "Cataclysm.TransformComponent", "The entity’s TransformComponent" },
			{ "Translation", "Cataclysm.Vec3", "The entity’s local position in 3D coordinates" }
		},
		{
			{ "AddComponent<T>() where T : Component", "T", "Adds a new component of type T to the entity, if it does not already exist." },
			{ "As<T>() where T : Entity", "T", "Returns a class of type T that extends Cataclysm.Entity and is attached to the entity, if it exists" },
			{ "Equals(Entity other)", "bool", "Returns true if the entities have the same ID, otherwise returns false" },
			{ "Equals(object obj)", "override bool", "Returns true if the object is an entity with the same ID, otherwise returns false" },
			{ "GetComponent<T>() where T : Component", "T", "Checks if the entity has a component of type T and returns it." },
			{ "GetHashCode()", "override int", "Returns the entity's ID as an integer" },
			{ "GetParent()", "Cataclysm.Entity", "Returns the parent entity of this entity, if it has one." },
			{ "HasComponent<T>() where T : Component", "bool", "Checks if the entity has a component of type T." },
			{ "Log()", "void", "Logs the entity's ID to the console for debug purposes" },
			{ "RemoveComponent<T>()", "void", "Removes the component of type T from the entity, if it has one." },
			{ "SetParent(Entity parent)", "void", "Parents the given entity to this entity." },
			{ "OnCollisionEnter2D(Entity other)", "abstract void", "Called when a non-trigger collider attached to this entity collides with another." },
			{ "OnCollisionExit2D(Entity other)", "abstract void", "Called when a non-trigger collider attached to this entity stops colliding with another entity’s non-trigger collider" },
			{ "OnCreate()", "abstract void", "Called before the first frame." },
			{ "OnTriggerEnter2D(Entity other)", "abstract void", "Called when a trigger collider attached to this entity collides with another entity's collider" },
			{ "OnTriggerExit2D(Entity other)", "abstract void", "Called when a trigger collider attached to this entity stops colliding with another entity’s collider" },
			{ "OnUpdate(float ts)", "abstract void", "Called every frame." },
			{ "Rotate(Vec3 rotation)", "void", "Rotates the entity in 3D space" },
			{ "ToString()", "override string", "Returns the entity's ID as a string" },
			{ "Translate(Vec3 translation)", "void", "Translates the entity in 3D space" },
			{ "Destroy(Entity entity)", "static void", "Destroys the given entity" },
			{ "FindEntityByName(string name)", "static Cataclysm.Entity", "Finds an entity in the scene with the given name" },
			{ "Instantiate(string name)", "static Cataclysm.Entity", "Instantiates an empty entity and returns it" },
			{ "Instantiate(Entity parent, string name)", "static Cataclysm.Entity", "Instantiates an empty entity, parents it to the given entity, and returns it. "},
			{ "==(Entity entity1, Entity entity2)", "static operator", "Returns true if the entities have the same ID, otherwise returns false "},
			{ "!=(Entity entity1, Entity entity2)", "static operator", "Returns false if the entities have the same ID, otherwise returns true "},
		}
	},
	{
		"Cataclysm.Input",
		"CCML/Source/Cataclysm/Input.cs",
		"Static class containing Cataclysm input functions.",
		{},
		{
			{ "GetMousePosition()", "static Cataclysm.Vec2", "Returns the mouse world space coordinates" },
			{ "IsKeyDown(KeyCode keycode)", "static bool", "Checks if the given key is down this frame" },
			{ "IsMouseButtonDown(MouseButton mouseButton)", "static bool", "Checks if the given mouse button is down this frame" }
		}
	},
	{
		"Cataclysm.KeyCode",
		"CCML/Source/Cataclysm/KeyCode.cs",
		"Enum containing Cataclysm key codes.",
		{},
		{}
	},
	{
		"Cataclysm.MouseCode",
		"CCML/Source/Cataclysm/MouseCode.cs",
		"Enum containing Cataclysm mouse codes",
		{},
		{}
	},
	{
		"Cataclysm.Rigidbody2DComponent",
		"CCML/Source/Cataclysm/ECS/Rigidbody2DComponent.cs",
		"Cataclysm Rigidbody2D component",
		{
			{ "GravityScale", "float", "" },
			{ "Mass", "float", "" },
			{ "Type", "Cataclysm.Rigidbody2D.BodyType", "Rigidbody type" },
			{ "Velocity", "Cataclysm.Vec2", "LinearVelocity" }
		},
		{
			{ "ApplyAngularImpulse(float impulse, bool wake)", "void", "Applies an angular impulse to the rigidbody" },
			{ "ApplyForce(Vec2 force, Vec2 point, bool wake)", "void", "Applies a force to the rigidbody at the given point" },
			{ "ApplyForce(Vec2 force, bool wake)", "void", "Applies a force to the rigidbody at the center" },
			{ "ApplyLinearImpulse(Vec2 impulse, Vec2 worldPosition, bool wake)", "void", "Applies a linear impulse to the rigidbody at the given position" },
			{ "ApplyLinearImpulse(Vec2 impulse, bool wake)", "void", "Applies a linear impulse to the rigidbody at the center" },
			{ "Reset()", "void", "Resets all properties to default values" }
		}
	},
	{
		"Cataclysm.SpriteRendererComponent",
		"CCML/Source/Cataclysm/ECS/SpriteRendererComponent.cs",
		"Cataclysm SpriteRenderer component",
		{
			{ "Color", "Cataclysm.Vec4", "Vertex color" },
			{ "TilingFactor", "float", "Sprite tiling factor" }
		},
		{
			{ "Reset()", "void", "Resets all properties to default values" }
		}
	},
	{
		"Cataclysm.TextComponent",
		"CCML/Source/Cataclysm/ECS/TextComponent.cs",
		"Cataclysm Text component",
		{
			{ "Color", "Cataclysm.Vec4", "Text vertex color" },
			{ "Kerning", "float", "" },
			{ "LineSpacing", "float", "" },
			{ "text", "string", "" }
		},
		{
			{ "Reset()", "void", "Resets all properties to default values" }
		}
	},
	{
		"Cataclysm.TransformComponent",
		"CCML/Source/Cataclysm/ECS/TransformComponent.cs",
		"Cataclysm Transform component",
		{
			{ "GlobalRotation", "Cataclysm.Vec3", "Global rotation in Euler angles" },
			{ "GlobalScale", "Cataclysm.Vec3", "Global scale" },
			{ "GlobalTranslation", "Cataclysm.Vec3", "Global translation in 3D coordinates" },
			{ "Rotation", "Cataclysm.Vec3", "Local rotation in Euler angles" },
			{ "Scale", "Cataclysm.Vec3", "Local scale" },
			{ "Translation", "Cataclysm.Vec3", "Local translation in 3D coordinates" }
		},
		{
			{ "Reset()", "void", "Resets all properties to default values" },
			{ "Rotate(Vec3 rotation)", "void", "" },
			{ "Translate(Vec3 translation)", "void", "" }
		}
	},
	{
		"Cataclysm.Vec2",
		"CCML/Source/Cataclysm/Vec2.cs",
		"Represents a 2-dimensional floating point vector",
		{
			{ "X", "float", "" },
			{ "Y", "float", "" },
			{ "Zero", "static Cataclysm.Vec2", "Returns a new Vec2 with coordinates at the origin" }
		},
		{
			{ "Cataclysm.Vec2(float scalar)", "Cataclysm.Vec2", "Constructor" },
			{ "Cataclysm.Vec2(float x, float y)", "Cataclysm.Vec2", "Constructor" },
			{ "Magnitude()", "float", "Returns the absolute length of the vector" },
			{ "MagnitudeSquared()", "float", "" },
			{ "ToString()", "override string", "({X}, {Y})" },
			{ "DotProduct(Vec2 a, Vec2 b)", "static float", "" },
			{ "Lerp(Vec2 a, Vec2 b, float timestep)", "static Cataclysm.Vec2", "Linearly interpolates between the two vectors" },
			{ "Normalized(Vec2 vector)", "static Cataclysm.Vec2", "Returns a vector of the same direction with a magnitude of 1" },
			{ "+(Vec2 a, Vec2 b)", "static operator Cataclysm.Vec2", "" },
			{ "*(Vec2 vector, float scalar)", "static operator Cataclysm.Vec2", "" },
			{ "-(Vec2 vector)", "static operator Cataclysm.Vec2", "" },
			{ "-(Vec2 a, Vec2 b)", "static operator Cataclysm.Vec2", "" }
		}
	},
	{
		"Cataclysm.Vec3",
		"CCML/Source/Cataclysm/Vec3.cs",
		"Represents a 3-dimensional floating point vector",
		{
			{ "X", "float", "" },
			{ "Y", "float", "" },
			{ "Z", "float", "" },
			{ "XY", "Cataclysm.Vec2", "" },
			{ "Zero", "static Cataclysm.Vec3", "Returns a new Vec3 with coordinates at the origin" }
		},
		{
			{ "Cataclysm.Vec3(float scalar)", "Cataclysm.Vec3", "Constructor" },
			{ "Cataclysm.Vec3(float x, float y, float z)", "Cataclysm.Vec3", "Constructor" },
			{ "Cataclysm.Vec3(Vec2 xy, float z)", "Cataclysm.Vec3", "Constructor" },
			{ "Magnitude()", "float", "Returns the absolute length of the vector" },
			{ "MagnitudeSquared()", "float", "" },
			{ "ToString()", "override string", "({X}, {Y}, {Z})" },
			{ "DotProduct(Vec3 a, Vec3 b)", "static float", "" },
			{ "Lerp(Vec3 a, Vec3 b, float timestep)", "static Cataclysm.Vec3", "Linearly interpolates between the two vectors" },
			{ "Normalized(Vec3 vector)", "static Cataclysm.Vec3", "Returns a vector of the same direction with a magnitude of 1" },
			{ "+(Vec3 a, Vec3 b)", "static operator Cataclysm.Vec3", "" },
			{ "*(Vec3 vector, float scalar)", "static operator Cataclysm.Vec3", "" },
			{ "-(Vec3 vector)", "static operator Cataclysm.Vec3", "" },
			{ "-(Vec3 a, Vec3 b)", "static operator Cataclysm.Vec3", "" }
		}
	},
	{
		"Cataclysm.Vec4",
		"CCML/Source/Cataclysm/Vec4.cs",
		"Represents a 4-dimensional floating point vector",
		{
			{ "X", "float", "" },
			{ "Y", "float", "" },
			{ "Z", "float", "" },
			{ "W", "float", "" },
			{ "XY", "Cataclysm.Vec2", "" },
			{ "XYZ", "Cataclysm.Vec3", "" },
			{ "Zero", "static Cataclysm.Vec4", "Returns a new Vec4 with coordinates at the origin" }
		},
		{
			{ "Cataclysm.Vec4(float scalar)", "Cataclysm.Vec4", "Constructor" },
			{ "Cataclysm.Vec4(float x, float y, float z, float w)", "Cataclysm.Vec4", "Constructor" },
			{ "Cataclysm.Vec4(Vec3 xyz, float w)", "Cataclysm.Vec4", "Constructor" },
			{ "Magnitude()", "float", "Returns the absolute length of the vector" },
			{ "MagnitudeSquared()", "float", "" },
			{ "ToString()", "override string", "({X}, {Y}, {Z}, {W})" },
			{ "DotProduct(Vec4 a, Vec4 b)", "static float", "" },
			{ "Lerp(Vec4 a, Vec4 b, float timestep)", "static Cataclysm.Vec4", "Linearly interpolates between the two vectors" },
			{ "Normalized(Vec4 vector)", "static Cataclysm.Vec4", "Returns a vector of the same direction with a magnitude of 1" },
			{ "+(Vec4 a, Vec4 b)", "static operator Cataclysm.Vec4", "" },
			{ "*(Vec4 vector, float scalar)", "static operator Cataclysm.Vec4", "" },
			{ "-(Vec4 vector)", "static operator Cataclysm.Vec4", "" },
			{ "-(Vec4 a, Vec4 b)", "static operator Cataclysm.Vec4", "" }
		}
	}
	};


	void DocumentationPanel::OnImGuiRender()
	{
		static int selected_index = 0;

		ImGui::Begin("CCML Documentation");

		// Left panel: List of entries
		ImGui::BeginChild("LeftPanel", ImVec2(250, 0), true);
		for (int i = 0; i < s_Documentation.size(); ++i) {
			if (ImGui::Selectable(s_Documentation[i].name.c_str(), selected_index == i)) {
				selected_index = i;
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();

		// Right panel: Details of the selected entry
		ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);

		if (!s_Documentation.empty() && selected_index < s_Documentation.size())
		{
			const DocEntry& entry = s_Documentation[selected_index];

			ImGui::TextWrapped(entry.name.c_str());
			ImGui::TextWrapped(entry.path.c_str());

			// ImGui::Separator();
			// ImGui::TextWrapped("%s", entry.summary.c_str());

			ImGui::Separator();
			if (ImGui::BeginTable("PropertiesTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable)) {
				ImGui::TableSetupColumn("Type");
				ImGui::TableSetupColumn("Property Name");
				ImGui::TableSetupColumn("Summary");
				ImGui::TableHeadersRow();

				for (const auto& prop : entry.properties) {
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::TextWrapped(prop.type.c_str());

					ImGui::TableSetColumnIndex(1);
					ImGui::TextWrapped(prop.name.c_str());

					ImGui::TableSetColumnIndex(2);
					ImGui::TextWrapped("%s", prop.summary.c_str());
				}
				ImGui::EndTable();
			}
			ImGui::Separator();
			if (ImGui::BeginTable("MethodsTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
				ImGui::TableSetupColumn("Return Type");
				ImGui::TableSetupColumn("Method Signature");
				ImGui::TableSetupColumn("Summary");
				ImGui::TableHeadersRow();

				for (const auto& method : entry.methods) {
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::TextWrapped(method.returnType.c_str());

					ImGui::TableSetColumnIndex(1);
					ImGui::TextWrapped(method.signature.c_str());

					ImGui::TableSetColumnIndex(2);
					ImGui::TextWrapped("%s", method.summary.c_str());
				}
				ImGui::EndTable();
			}
		}
		else 
		{
			ImGui::Text("No documentation available.");
		}

		ImGui::EndChild();
		ImGui::End();
	}
}
