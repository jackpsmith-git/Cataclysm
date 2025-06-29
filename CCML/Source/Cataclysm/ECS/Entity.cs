using System;

using Cataclysm.Math;

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

		static internal Entity CreateInternal(ulong id) => new Entity(id);

		public bool Enabled
		{
			get => InternalCalls.Entity_GetEnabled(ID);
			set => InternalCalls.Entity_SetEnabled(ID, value);
		}

		public Transform Transform
		{
			get
			{
				return GetComponent<Transform>();
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

		/// <summary>
		/// Translates the entity by the given translation
		/// </summary>
		/// <param name="translation"></param>
		/// <returns><see langword="void"/></returns>
		public void Translate(Vec3 translation)
		{
			Translation += translation;
		}

		/// <summary>
		/// Rotates the entity by the given rotation
		/// </summary>
		/// <param name="rotation"></param>
		/// <returns><see langword="void"/></returns>
		public void Rotate(Vec3 rotation)
		{
			Rotation += rotation;
		}

		/// <summary>
		/// Sets the entity's parent
		/// </summary>
		/// <param name="parent"></param>
		/// <returns><see langword="void"/></returns>
		public void SetParent(Entity parent) => InternalCalls.Entity_SetParent(ID, parent.ID);

		/// <summary>
		/// Removes the entity's parent
		/// </summary>
		/// <returns><see langword="void"/></returns>
		public void RemoveParent() => InternalCalls.Entity_RemoveParent(ID);
		
		/// <summary>
		/// Gets the entity's parent
		/// </summary>
		/// <returns>Parent <see cref="Entity"/></returns>
		public Entity GetParent()
		{
			InternalCalls.Entity_GetParent(ID, out ulong id);
			return new Entity(id);
		}

		/// <summary>
		/// Instantiates an empty <see cref="Entity"/> in the scene and returns it
		/// </summary>
		/// <returns><see langword="new"/> <see cref="Entity"/></returns>
		public static Entity Instantiate()
		{
			InternalCalls.Entity_InstantiateEmptyWithoutName(out ulong entityID);
			return new Entity(entityID);
		}

		/// <summary>
		/// Instantiates an empty <see cref="Entity"/> in the scene with the given name and returns it
		/// </summary>
		/// <param name="name"></param>
		/// <returns><see langword="new"/> <see cref="Entity"/></returns>
		public static Entity Instantiate(string name = "NewEntity")
		{
			InternalCalls.Entity_InstantiateEmpty(name, out ulong entityID);
			return new Entity(entityID);
		}

		/// <summary>
		/// Instnatiates an empty <see cref="Entity"/> in the scene with the given name as a child of the given parent and returns it
		/// </summary>
		/// <param name="parent"></param>
		/// <param name="name"></param>
		/// <returns><see langword="new"/> <see cref="Entity"/></returns>
		public static Entity Instantiate(Entity parent, string name = "NewEntity")
		{
			InternalCalls.Entity_InstantiateEmptyAsChild(name, parent.ID, out ulong entityID);
			return new Entity(entityID);
		}


		/// <typeparam name="T"></typeparam>
		/// <returns><see langword="true"/> if he entity has a component of type T, otherwise <see langword="false"/></returns>
        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCalls.Entity_HasComponent(ID, componentType);
        }

		/// <summary>
		/// Gets a <see cref="Component"/> of the given type
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns></returns>
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

		/// <summary>
		/// Removes a <see cref="Component"/> of the given type, if the entity has it
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns><see langword="void"/></returns>
		public void RemoveCommponent<T>() where T : Component, new()
		{
			if (typeof(T) == typeof(NativeScript))
			{
				Debug.Error("[Entity.RemoveComponent] Cannot remove NativeScriptComponent at runtime.");
				return;
			}
			else if (typeof(T) == typeof(MonoScript))
			{
				Debug.Error("[Entity.RemoveComponent] Cannot remove MonoScriptComponent at runtime.");
				return;
			}
			else if (typeof(T) == typeof(Transform))
			{
				Debug.Error("[Entity.RemoveComponent] Cannot manually remove TransformComponent.");
				return;
			}
			else if (typeof(T) == typeof(Rigidbody2D))
			{
				Debug.Error("[Entity.RemoveComponent] Cannot remove Rigidbody2DComponent at runtime.");
				return;
			}
			else if (!HasComponent<T>())
			{
				Debug.Error("[Entity.RemoveComponent] Entity does not have component of type '" + typeof(T).ToString() + "'.");
				return;
			}

			if (typeof(T) == typeof(BoxCollider2D))
			{
				InternalCalls.Entity_RemoveBoxCollider2DComponent(ID);
			}
			else if (typeof(T) == typeof(Camera))
			{
				InternalCalls.Entity_RemoveCameraComponent(ID);
			}
			else if (typeof(T) == typeof(CircleCollider2D))
			{
				InternalCalls.Entity_RemoveCircleCollider2DComponent(ID);
			}
			else if (typeof(T) == typeof(CircleRenderer))
			{
				InternalCalls.Entity_RemoveCircleRendererComponent(ID);
			}
			else if (typeof(T) == typeof(SpriteRenderer))
			{
				InternalCalls.Entity_RemoveSpriteRendererComponent(ID);
			}
			else if (typeof(T) == typeof(SpriteRenderer))
			{
				InternalCalls.Entity_RemoveTextComponent(ID);
			}
			else if (typeof(T) == typeof(AudioSource))
			{
				InternalCalls.Entity_RemoveAudioSourceComponent(ID);
			}
			else
			{
				Debug.Error("[Entity.RemoveComponent] '" + typeof(T) + "' is not a valid component type.");
				return;
			}
		}

		/// <summary>
		/// Adds a component of the given type, if the entity does not already have it
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns><see langword="new"/> <see cref="Component"/></returns>
		public T AddComponent<T>() where T : Component, new()
		{
			if (typeof(T) == typeof(NativeScript))
			{
				Debug.Error("[Entity.AddComponent] Cannot add NativeScriptComponent at runtime.");
				return null;
			}
			else if (typeof(T) == typeof(MonoScript))
			{
				Debug.Error("[Entity.AddComponent] Cannot add MonoScriptComponent at runtime.");
				return null;
			}
			else if (typeof(T) == typeof(Transform))
			{
				Debug.Error("[Entity.AddComponent] Cannot manually add TransformComponent.");
				return null;
			}
			else if (typeof(T) == typeof(Rigidbody2D))
			{
				Debug.Error("[Entity.AddComponent] Cannot add Rigidbody2DComponent at runtime.");
				return null;
			}
			else if (HasComponent<T>())
			{
				Debug.Error("[Entity.AddComponent] Entity already has component of type '" + typeof(T).ToString() + "'.");
				return null;
			}

			if (typeof(T) == typeof(BoxCollider2D))
			{
				InternalCalls.Entity_AddBoxCollider2DComponent(ID);
			}
			else if (typeof(T) == typeof(Camera))
			{
				InternalCalls.Entity_AddCameraComponent(ID);
			}
			else if (typeof(T) == typeof(CircleCollider2D))
			{
				InternalCalls.Entity_AddCircleCollider2DComponent(ID);
			}
			else if (typeof(T) == typeof(CircleRenderer))
			{
				InternalCalls.Entity_AddCircleRendererComponent(ID);
			}
			else if (typeof(T) == typeof(SpriteRenderer))
			{
				InternalCalls.Entity_AddSpriteRendererComponent(ID);
			}
			else if (typeof(T) == typeof(SpriteRenderer))
			{
				InternalCalls.Entity_AddTextComponent(ID);
			}
			else if (typeof(T) == typeof(AudioSource))
			{
				InternalCalls.Entity_AddAudioSourceComponent(ID);
			}
			else
			{
				Debug.Error("[Entity.AddComponent] '" + typeof(T).ToString() + "' is not a valid component type.");
				return null;
			}

			return GetComponent<T>();
		}

		/// <summary>
		/// Destroys the given <see cref="Entity"/> and removes it from the scene
		/// </summary>
		/// <param name="entity"></param>
		/// <returns><see langword="void"/></returns>
		public static void Destroy(Entity entity) => InternalCalls.Entity_Destroy(entity.ID);

		/// <summary>
		/// Finds the first <see cref="Entity"/> in the scene with the given name, if it exists, and returns it
		/// </summary>
		/// <param name="name"></param>
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

		public static implicit operator ulong(Entity e) => e?.ID ?? 0;
		public static implicit operator Entity(ulong id) => new Entity(id);

		public override bool Equals(object obj)
		{
			if (obj == null) return false;
			if (!(obj is Entity)) return false;

			Entity entity = (Entity)obj;
			return ID == entity.ID;
		}
	}
}
