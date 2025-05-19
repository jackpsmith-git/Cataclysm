using System;
using System.Runtime.CompilerServices;

namespace Cataclysm
{
	/// <summary>
	/// Cataclysm Entity
	/// </summary>
	public class Entity : IEquatable<Entity>
	{
		protected Entity() { ID = 0; }

		internal Entity(ulong id)
		{
			ID = id;
		}

		public bool Enabled
		{
			get => InternalCalls.Entity_GetEnabled(ID);
			set => InternalCalls.Entity_SetEnabled(ID, value);
		}

		public TransformComponent Transform
		{
			get
			{
				return GetComponent<TransformComponent>();
			}
			private set { }
		}

		public readonly ulong ID;

		public Vec3 Translation
		{
			get
			{
				InternalCalls.TransformComponent_GetTranslation(ID, out Vec3 result);
				return result;
			}
			set
			{
				InternalCalls.TransformComponent_SetTranslation(ID, ref value);
			}
		}

		public Vec3 Rotation
		{
			get
			{
				InternalCalls.TransformComponent_GetRotation(ID, out Vec3 result);
				return result;
			}
			set
			{
				InternalCalls.TransformComponent_SetRotation(ID, ref value);
			}
		}

		public Vec3 Scale
		{
			get
			{
				InternalCalls.TransformComponent_GetScale(ID, out Vec3 result);
				return result;
			}
			set
			{
				InternalCalls.TransformComponent_SetScale(ID, ref value);
			}
		}

		public void Translate(Vec3 translation)
		{
			Translation += translation;
		}

		public void Rotate(Vec3 rotation)
		{
			Rotation += rotation;
		}

		public void SetParent(Entity parent) => InternalCalls.Entity_SetParent(ID, parent.ID);
		public void RemoveParent() => InternalCalls.Entity_RemoveParent(ID);
		
		public Entity GetParent()
		{
			InternalCalls.Entity_GetParent(ID, out ulong id);
			return new Entity(id);
		}

		public static Entity Instantiate(string name = "NewEntity")
		{
			InternalCalls.Entity_InstantiateEmpty(name, out ulong entityID);
			return new Entity(entityID);
		}

		public static Entity Instantiate(Entity parent, string name = "NewEntity")
		{
			InternalCalls.Entity_InstantiateEmptyAsChild(name, parent.ID, out ulong entityID);
			return new Entity(entityID);
		}

		/// <summary>
		/// Checks if the entity has a component of type T
		/// </summary>
		/// <typeparam name="T">Component Type</typeparam>
		/// <returns>True if entity has component of type "T", otherwise returns false</returns>
        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCalls.Entity_HasComponent(ID, componentType);
        }

		/// <summary>
		/// Checks if the entity has a component of type T and returns it
		/// </summary>
		/// <typeparam name="T">Component Type</typeparam>
		/// <returns>Component of type "T" if the entity has it, otherwise returns null</returns>
        public T GetComponent<T>() where T : Component, new()
        {
			if (!HasComponent<T>())
			{
				Debug.Log("[Entity.GetComponent] Entity does not have component of type '" + typeof(T).ToString() + "'.");
				return null;
			}

            T component = new T() { Entity = this };
            return component;
        }

		public void RemoveCommponent<T>() where T : Component, new()
		{
			if (typeof(T) == typeof(NativeScriptComponent))
			{
				Debug.Error("[Entity.RemoveComponent] Cannot remove NativeScriptComponent at runtime.");
				return;
			}
			else if (typeof(T) == typeof(MonoScriptComponent))
			{
				Debug.Error("[Entity.RemoveComponent] Cannot remove MonoScriptComponent at runtime.");
				return;
			}
			else if (typeof(T) == typeof(TransformComponent))
			{
				Debug.Error("[Entity.RemoveComponent] Cannot manually remove TransformComponent.");
				return;
			}
			else if (typeof(T) == typeof(Rigidbody2DComponent))
			{
				Debug.Error("[Entity.RemoveComponent] Cannot remove Rigidbody2DComponent at runtime.");
				return;
			}
			else if (!HasComponent<T>())
			{
				Debug.Error("[Entity.RemoveComponent] Entity does not have component of type '" + typeof(T).ToString() + "'.");
				return;
			}

			if (typeof(T) == typeof(BoxCollider2DComponent))
			{
				InternalCalls.Entity_RemoveBoxCollider2DComponent(ID);
			}
			else if (typeof(T) == typeof(CameraComponent))
			{
				InternalCalls.Entity_RemoveCameraComponent(ID);
			}
			else if (typeof(T) == typeof(CircleCollider2DComponent))
			{
				InternalCalls.Entity_RemoveCircleCollider2DComponent(ID);
			}
			else if (typeof(T) == typeof(CircleRendererComponent))
			{
				InternalCalls.Entity_RemoveCircleRendererComponent(ID);
			}
			else if (typeof(T) == typeof(SpriteRendererComponent))
			{
				InternalCalls.Entity_RemoveSpriteRendererComponent(ID);
			}
			else if (typeof(T) == typeof(SpriteRendererComponent))
			{
				InternalCalls.Entity_RemoveTextComponent(ID);
			}
			else
			{
				Debug.Error("[Entity.RemoveComponent] '" + typeof(T) + "' is not a valid component type.");
				return;
			}
		}

		/// <summary>
		/// Adds a component of type T if it does not already exist and returns it
		/// </summary>
		/// <typeparam name="T">Component Type</typeparam>
		/// <returns></returns>
		public T AddComponent<T>() where T : Component, new()
		{
			if (typeof(T) == typeof(NativeScriptComponent))
			{
				Debug.Error("[Entity.AddComponent] Cannot add NativeScriptComponent at runtime.");
				return null;
			}
			else if (typeof(T) == typeof(MonoScriptComponent))
			{
				Debug.Error("[Entity.AddComponent] Cannot add MonoScriptComponent at runtime.");
				return null;
			}
			else if (typeof(T) == typeof(TransformComponent))
			{
				Debug.Error("[Entity.AddComponent] Cannot manually add TransformComponent.");
				return null;
			}
			else if (typeof(T) == typeof(Rigidbody2DComponent))
			{
				Debug.Error("[Entity.AddComponent] Cannot add Rigidbody2DComponent at runtime.");
				return null;
			}
			else if (HasComponent<T>())
			{
				Debug.Error("[Entity.AddComponent] Entity already has component of type '" + typeof(T).ToString() + "'.");
				return null;
			}

			if (typeof(T) == typeof(BoxCollider2DComponent))
			{
				InternalCalls.Entity_AddBoxCollider2DComponent(ID);
			}
			else if (typeof(T) == typeof(CameraComponent))
			{
				InternalCalls.Entity_AddCameraComponent(ID);
			}
			else if (typeof(T) == typeof(CircleCollider2DComponent))
			{
				InternalCalls.Entity_AddCircleCollider2DComponent(ID);
			}
			else if (typeof(T) == typeof(CircleRendererComponent))
			{
				InternalCalls.Entity_AddCircleRendererComponent(ID);
			}
			else if (typeof(T) == typeof(SpriteRendererComponent))
			{
				InternalCalls.Entity_AddSpriteRendererComponent(ID);
			}
			else if (typeof(T) == typeof(SpriteRendererComponent))
			{
				InternalCalls.Entity_AddTextComponent(ID);
			}
			else
			{
				Debug.Error("[Entity.AddComponent] '" + typeof(T).ToString() + "' is not a valid component type.");
				return null;
			}

			return GetComponent<T>();
		}

		public static void Destroy(Entity entity) => InternalCalls.Entity_Destroy(entity.ID);

		/// <summary>
		/// Finds an entity with the given name
		/// </summary>
		/// <param name="name">Entity name</param>
		/// <returns>Entity if found, otherwise returns null</returns>
		public static Entity FindEntityByName(string name)
		{
			ulong entityID = InternalCalls.Entity_FindEntityByName(name);
			if (entityID == 0)
			{
				Debug.Error($"Entity, '{name}', does not exist.");
				return null;
			}

			return new Entity(entityID);
		}

		public T As<T>() where T : Entity, new()
		{
			object instance = InternalCalls.GetScriptInstance(ID);
			return instance as T;
		}

		public void Log() => Debug.Log($"{ID}");
		public override string ToString() => ID.ToString();

		public static bool operator ==(Entity entity1, Entity entity2) => entity1.ID == entity2.ID;
		public static bool operator !=(Entity entity1, Entity entity2) => entity1.ID != entity2.ID;
		public override int GetHashCode() => (int)ID;
		public bool Equals(Entity other) => ID == other.ID;

		public override bool Equals(object obj)
		{
			if (obj == null) return false;
			if (!(obj is Entity)) return false;

			Entity entity = (Entity)obj;
			return ID == entity.ID;
		}
	}
}
