using System;

namespace Cataclysm
{
	/// <summary>
	/// Represents a 2-dimensional floating point vector.
	/// </summary>
	public struct Vec2
    {
        public float X, Y;

        public static Vec2 Zero => new Vec2(0.0f);

        public Vec2(float scalar)
        {
            X = scalar;
            Y = scalar;
        }

        public Vec2(float x, float y)
        {
            X = x;
            Y = y;
        }

        public static Vec2 operator +(Vec2 a, Vec2 b) => new Vec2(a.X + b.X, a.Y + b.Y);
        public static Vec2 operator *(Vec2 vector, float scalar) => new Vec2(vector.X * scalar, vector.Y * scalar);
		public static Vec2 operator -(Vec2 vector) => new Vec2(-vector.X, -vector.Y);
		public static Vec2 operator -(Vec2 a, Vec2 b) => new Vec2(a.X - b.X, a.Y - b.Y);

		public float MagnitudeSquared() => ((X * X) + (Y * Y));
		public float Magnitude() => (float)Math.Sqrt(MagnitudeSquared());
		public static Vec2 Lerp(Vec2 a, Vec2 b, float timestep) => (a + (b - a) * timestep);
		public static float DotProduct(Vec2 a, Vec2 b) => ((a.X * b.X) + (a.Y * b.Y));

		public static Vec2 Normalized(Vec2 vector)
		{
			float magnitude = vector.Magnitude();
			if (magnitude > 0)
			{
				return new Vec2(vector.X / magnitude, vector.Y / magnitude);
			}
			else
			{
				return new Vec2(0.0f, 0.0f);
			}
		}

		public override string ToString() => $"({X}, {Y})";
	}
}
