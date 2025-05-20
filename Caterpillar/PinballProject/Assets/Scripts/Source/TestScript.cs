using Cataclysm;
using System.Collections.Generic;

namespace Pinball
{
	public class TestScript : Entity
	{
		private Rigidbody2DComponent rigidbody;
		private float speed = 200.0f;

		public bool IsLeft = false;
		public Vec2 TestVec2;
		public Vec3 TestVec3;
		public Vec4 TestVec4;
		public Entity TestEntity;

		public void OnCreate()
		{
			rigidbody = GetComponent<Rigidbody2DComponent>();
		}

		public void OnUpdate(float ts)
		{
			Debug.Log(TestEntity.ID);

			if (IsLeft)
			{
				if (Input.IsKeyDown(KeyCode.W) && Input.IsKeyDown(KeyCode.S))
				{
					rigidbody.Velocity = Vec2.Zero;
				}
				else if (Input.IsKeyDown(KeyCode.W))
				{
					if (Translation.Y < 3.5f)
						rigidbody.Velocity = new Vec2(0, ts * speed);
				}
				else if (Input.IsKeyDown(KeyCode.S))
				{
					if (Translation.Y > -3.5f)
						rigidbody.Velocity = new Vec2(0, -ts * speed);
				}
				else
					rigidbody.Velocity = Vec2.Zero;
			}
			else
			{
				if (Input.IsKeyDown(KeyCode.Up) && Input.IsKeyDown(KeyCode.Down))
				{
					rigidbody.Velocity = Vec2.Zero;
				}
				else if (Input.IsKeyDown(KeyCode.Up))
				{
					if (Translation.Y < 3.5f)
						rigidbody.Velocity = new Vec2(0, ts * speed);
				}
				else if (Input.IsKeyDown(KeyCode.Down))
				{
					if (Translation.Y > -3.5f)
						rigidbody.Velocity = new Vec2(0, -ts * speed);
				}
				else
					rigidbody.Velocity = Vec2.Zero;
			}
		}

		public void OnCollisionEnter2D(Entity other)
		{

		}

		public void OnTriggerEnter2D(Entity other)
		{

		}

		public void OnCollisionExit2D(Entity other)
		{

		}

		public void OnTriggerExit2D(Entity other)
		{

		}
	}
}
