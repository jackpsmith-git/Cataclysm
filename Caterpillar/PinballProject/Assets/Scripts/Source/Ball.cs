using System;
using Cataclysm;

namespace Pinball
{
	public class Ball : Entity
	{
		private Rigidbody2DComponent m_RigidBody;
		private TransformComponent m_Transform;

		public bool TestBoolean;
		public float TestFloat;
		public int TestInt;
		public double TestDouble;

		public char TestChar;
		public Vec3 TestVec3;
		public string TestString;

		public Entity TestEntity;

		private Entity entity;
		private Rigidbody2DComponent rigidbody;

		public void OnCreate()
		{
			m_RigidBody = GetComponent<Rigidbody2DComponent>();
			m_Transform = GetComponent<TransformComponent>();

			entity = Instantiate("NewEntity");
			BoxCollider2DComponent bc = entity.AddComponent<BoxCollider2DComponent>();
			SpriteRendererComponent src = entity.AddComponent<SpriteRendererComponent>();
			src.Color = new Vec4(1.0f, 1.0f, 0.0f, 1.0f);
			// rigidbody = entity.AddComponent<Rigidbody2DComponent>();
			// rigidbody.Type = Rigidbody2DComponent.BodyType.Dynamic;
		}

		public void OnUpdate(float ts)
		{
			float speed = 1.0f;
			Vec3 velocity = Vec3.Zero;

			if (Input.IsKeyDown(KeyCode.W))
			{
				velocity.Y = 1.0f;
			}
			else if (Input.IsKeyDown(KeyCode.S))
			{
				velocity.Y = -1.0f;
			}

			if (Input.IsKeyDown(KeyCode.A))
			{
				velocity.X = -1.0f;
			}
			else if (Input.IsKeyDown(KeyCode.D))
			{
				velocity.X = 1.0f;
			}

			velocity *= speed;
			// rigidbody.ApplyLinearImpulseToCenter(velocity.XY, true);
			// Console.WriteLine(TestEntity.ID);
			// Console.WriteLine(Input.GetMousePosition());
			// m_RigidBody.ApplyLinearImpulseToCenter(velocity.XY, true);
		}
	}
}
