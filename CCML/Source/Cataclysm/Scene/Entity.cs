using System;

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
    }
}
