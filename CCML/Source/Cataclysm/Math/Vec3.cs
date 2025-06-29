namespace Cataclysm.Math
{
	/// <summary>
	/// Represents a 3-dimensional floating point vector
	/// </summary>
	public struct Vec3
    {
        public float X, Y, Z;

        public static Vec3 Zero => new Vec3(0.0f);
		public static Vec3 One => new Vec3(1.0f);
		public static Vec3 UnitX => new Vec3(1.0f, 0.0f, 0.0f);
		public static Vec3 UnitY => new Vec3(0.0f, 1.0f, 0.0f);
		public static Vec3 UnitZ => new Vec3(0.0f, 0.0f, 1.0f);

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
		public static Vec3 operator *(float scalar, Vec3 vector) => new Vec3(vector.X * scalar, vector.Y * scalar, vector.Z * scalar);
		public static Vec3 operator *(Vec3 a, Vec3 b) => new Vec3(a.X * b.X, a.Y * b.Y, a.Z * b.Z);
		public static Vec3 operator /(Vec3 vector, float scalar) => new Vec3(vector.X / scalar, vector.Y / scalar, vector.Z / scalar);
		public static Vec3 operator -(Vec3 vector) => new Vec3(-vector.X, -vector.Y, -vector.Z);
		public static Vec3 operator -(Vec3 a, Vec3 b) => new Vec3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
		public static bool operator ==(Vec3 a, Vec3 b) => ((a.X == b.X) && (a.Y == b.Y) && (a.Z == b.Z));
		public static bool operator !=(Vec3 a, Vec3 b) => !(a == b);

		public float MagnitudeSquared() => ((X * X) + (Y * Y) + (Z * Z));
		public float Magnitude() => (float)System.Math.Sqrt(MagnitudeSquared());
		public Vec3 Normalized() => (Magnitude() > 0) ? new Vec3(X / Magnitude(), Y / Magnitude(), Z / Magnitude()) : new Vec3(0.0f, 0.0f, 0.0f);

		public static Vec3 Lerp(Vec3 a, Vec3 b, float timestep) => (a + (b - a) * timestep);
		public static float DotProduct(Vec3 a, Vec3 b) => ((a.X * b.X) + (a.Y * b.Y) + (a.Z * b.Z));

		public static float MagnitudeSquared(Vec3 vector) => vector.MagnitudeSquared();
		public static float Magnitude(Vec3 vector) => vector.Magnitude();
		public static Vec3 Normalized(Vec3 vector) => vector.Normalized();

		public override string ToString() => $"({X}, {Y}, {Z})";

		public override bool Equals(object obj)
		{
			return obj is Vec3 vec &&
				   X == vec.X &&
				   Y == vec.Y &&
				   Z == vec.Z;
		}

		public override int GetHashCode()
		{
			int hashCode = -801722062;
			hashCode = hashCode * -1521134295 + X.GetHashCode();
			hashCode = hashCode * -1521134295 + Y.GetHashCode();
			hashCode = hashCode * -1521134295 + Z.GetHashCode();
			return hashCode;
		}
	}
}
