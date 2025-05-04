using System;
using System.Runtime.CompilerServices;

namespace Cataclysm
{
	/// <summary>
	/// Cataclysm Entity
	/// </summary>
	public class Entity
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

		public readonly ulong ID;

		public static Entity Instantiate(string name)
		{
			InternalCalls.Entity_InstantiateEmpty(name, out ulong entityID);
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
                return null;

            T component = new T() { Entity = this };
            return component;
        }

		/// <summary>
		/// Adds a component of type T if it does not already exist and returns it
		/// </summary>
		/// <typeparam name="T">Component Type</typeparam>
		/// <returns></returns>
		public T AddComponent<T>() where T : Component, new()
		{
			if (typeof(T) == typeof(NativeScriptComponent) ||
				typeof(T) == typeof(MonoScriptComponent) ||
				typeof(T) == typeof(TransformComponent))
			{
				return null;
			}

			if (HasComponent<T>())
				return null;

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
			else if (typeof(T) == typeof(Rigidbody2DComponent))
			{
				InternalCalls.Entity_AddRigidbody2DComponent(ID);
			}
			else if (typeof(T) == typeof(SpriteRendererComponent))
			{
				InternalCalls.Entity_AddSpriteRendererComponent(ID);
			}
			else if (typeof(T) == typeof(SpriteRendererComponent))
			{
				InternalCalls.Entity_AddTextComponent(ID);
			}

			return GetComponent<T>();
		}

		/// <summary>
		/// Finds an entity with the given name
		/// </summary>
		/// <param name="name">Entity name</param>
		/// <returns>Entity if found, otherwise returns null</returns>
		public Entity FindEntityByName(string name)
		{
			ulong entityID = InternalCalls.Entity_FindEntityByName(name);
			if (entityID == 0)
				return null;

			return new Entity(entityID);
		}

		public T As<T>() where T : Entity, new()
		{
			object instance = InternalCalls.GetScriptInstance(ID);
			return instance as T;
		}

		public void Log() => Debug.Log($"{ID}");

		public override string ToString() => ID.ToString();
	}
}
