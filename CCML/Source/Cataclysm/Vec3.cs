using System;

namespace Cataclysm
{
	/// <summary>
	/// Represents a 3-dimensional floating point vector.
	/// </summary>
	public struct Vec3
    {
        public float X, Y, Z;

        public static Vec3 Zero => new Vec3(0.0f);

        public Vec3(float scalar)
        {
            X = scalar;
            Y = scalar;
            Z = scalar;
        }

        public Vec3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

		public Vec3(Vec2 xy, float z)
		{
			X = xy.X;
			Y = xy.Y;
			Z = z;
		}

		public Vec2 XY
		{
			get => new Vec2(X, Y);
			set
			{
				X = value.X;
				Y = value.Y;
			}
		}

		public static Vec3 operator +(Vec3 a, Vec3 b) => new Vec3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
        public static Vec3 operator *(Vec3 vector, float scalar) => new Vec3(vector.X * scalar, vector.Y * scalar, vector.Z * scalar);
		public static Vec3 operator -(Vec3 vector) => new Vec3(-vector.X, -vector.Y, -vector.Z);
		public static Vec3 operator -(Vec3 a, Vec3 b) => new Vec3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);

		public float MagnitudeSquared() => ((X * X) + (Y * Y) + (Z * Z));
		public float Magnitude() => (float)Math.Sqrt(MagnitudeSquared());
		public static Vec3 Lerp(Vec3 a, Vec3 b, float timestep) => (a + (b - a) * timestep);
		public static float DotProduct(Vec3 a, Vec3 b) => ((a.X * b.X) + (a.Y * b.Y) + (a.Z * b.Z));

		public static Vec3 Normalized(Vec3 vector)
		{
			float magnitude = vector.Magnitude();
			if (magnitude > 0)
			{
				return new Vec3(vector.X / magnitude, vector.Y / magnitude, vector.Z / magnitude);
			}
			else
			{
				return new Vec3(0.0f, 0.0f, 0.0f);
			}
		}

		public override string ToString() => $"({X}, {Y}, {Z})";
	}
}
