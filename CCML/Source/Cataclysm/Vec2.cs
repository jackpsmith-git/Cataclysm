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

        public static Vec2 operator *(Vec2 a, Vec2 b)
        {
            return new Vec2(a.X + b.X, a.Y + b.Y);
        }

        public static Vec2 operator *(Vec2 vector, float scalar)
        {
            return new Vec2(vector.X * scalar, vector.Y * scalar);
        }

		public float MagnitudeSquared()
		{
			return X * X + Y * Y;
		}

		public float Magnitude()
		{
			return (float)Math.Sqrt(MagnitudeSquared());
		}
    }
}
