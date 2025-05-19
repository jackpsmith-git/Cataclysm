using Cataclysm;

namespace Pinball
{
	public class Ball : Entity
	{
		public void OnCreate()
		{
			Rigidbody2DComponent rigidbody = GetComponent<Rigidbody2DComponent>();
			rigidbody.ApplyLinearImpulse(new Vec2(-1, -0.2f), true);
		}
	}
}
