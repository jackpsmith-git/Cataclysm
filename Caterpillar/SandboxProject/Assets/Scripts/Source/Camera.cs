﻿using Cataclysm;

namespace Sandbox
{
    public class Camera : Entity
    {
		public Entity OtherEntity;

		public float DistanceFromPlayer = 0.0f;

		private Entity m_Player;

		void OnCreate()
		{
			m_Player = FindEntityByName("Player");
		}

        void OnUpdate(float ts)
        {
			if (m_Player != null)
				Translation = new Vec3(m_Player.Translation.XY, DistanceFromPlayer);

            float speed = 1.0f;
            Vec3 velocity = Vec3.Zero;

            if (Input.IsKeyDown(KeyCode.Up))
                velocity.Y = 1.0f;
            else if (Input.IsKeyDown(KeyCode.Down))
                velocity.Y = -1.0f;

            if (Input.IsKeyDown(KeyCode.Left))
                velocity.X = -1.0f;
            else if (Input.IsKeyDown(KeyCode.Right))
                velocity.X = 1.0f;

            velocity *= speed;

            Vec3 translation = Translation;
            translation += velocity * ts;
            Translation = translation;
        }
    }
}
