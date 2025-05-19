using Cataclysm;
using System;

namespace Pinball
{
	public class Trigger : Entity
	{
		Rigidbody2DComponent rb;

		public void OnCreate()
		{
			rb = GetComponent<Rigidbody2DComponent>();
		}

		public void OnUpdate(float ts)
		{
			rb.ApplyForce(new Vec2(ts, ts), true);
		}
	}
}
