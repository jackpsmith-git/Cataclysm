namespace Cataclysm.Math
{
	/// <summary>
	/// Represents a 4-dimensional floating point vector
	/// </summary>
	public struct Vec4
	{
		public float X, Y, Z, W;

		public static Vec4 Zero => new Vec4(0.0f);
		public static Vec4 One => new Vec4(1.0f);
		public static Vec4 UnitX => new Vec4(1.0f, 0.0f, 0.0f, 0.0f);
		public static Vec4 UnitY => new Vec4(0.0f, 1.0f, 0.0f, 0.0f);
		public static Vec4 UnitZ => new Vec4(0.0f, 0.0f, 1.0f, 0.0f);
		public static Vec4 UnitW => new Vec4(0.0f, 0.0f, 0.0f, 1.0f);

		public Vec4(float scalar)
		{
			X = scalar;
			Y = scalar;
			Z = scalar;
			W = scalar;
		}

		public Vec4(float x, float y, float z, float w)
		{
			X = x;
			Y = y;
			Z = z;
			W = w;
		}

		public Vec4(Vec3 xyz, float w)
		{
			X = xyz.X;
			Y = xyz.Y;
			Z = xyz.Z;
			W = w;
		}

		public Vec4(Vec2 xy, float z, float w)
		{
			X = xy.X;
			Y = xy.Y;
			Z = z;
			W = w;
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

		public Vec3 XYZ
		{
			get => new Vec3(X, Y, Z);
			set
			{
				X = value.X;
				Y = value.Y;
				Z = value.Z;
			}
		}

		public static Vec4 operator +(Vec4 a, Vec4 b) => new Vec4((a.X + b.X), (a.Y + b.Y), (a.Z + b.Z), (a.W + b.W));
		public static Vec4 operator *(Vec4 vector, float scalar) => new Vec4(vector.X * scalar, vector.Y * scalar, vector.Z * scalar, vector.W * scalar);
		public static Vec4 operator *(float scalar, Vec4 vector) => new Vec4(vector.X * scalar, vector.Y * scalar, vector.Z * scalar, vector.W * scalar);
		public static Vec4 operator *(Vec4 a, Vec4 b) => new Vec4(a.X * b.X, a.Y * b.Y, a.Z * b.Z, a.W * b.W);
		public static Vec4 operator /(Vec4 vector, float scalar) => new Vec4(vector.X / scalar, vector.Y / scalar, vector.Z / scalar, vector.W / scalar);
		public static Vec4 operator -(Vec4 vector) => new Vec4(-vector.X, -vector.Y, -vector.Z, -vector.W);
		public static Vec4 operator -(Vec4 a, Vec4 b) => new Vec4(a.X - b.X, a.Y - b.Y, a.Z - b.Z, a.W - b.W);
		public static bool operator ==(Vec4 a, Vec4 b) => ((a.X == b.X) && (a.Y == b.Y) && (a.Z == b.Z) && (a.W == b.W));
		public static bool operator !=(Vec4 a, Vec4 b) => !(a == b);

		public float MagnitudeSquared() => ((X * X) + (Y * Y) + (Z * Z) + (W * W));
		public float Magnitude() => (float)System.Math.Sqrt(MagnitudeSquared());
		public Vec4 Normalized() => (Magnitude() > 0) ? new Vec4(X / Magnitude(), Y / Magnitude(), Z / Magnitude(), W / Magnitude()) : new Vec4(0.0f, 0.0f, 0.0f, 0.0f);

		public static Vec4 Lerp(Vec4 a, Vec4 b, float timestep) => (a + (b - a) * timestep);
		public static float DotProduct(Vec4 a, Vec4 b) => ((a.X * b.X) + (a.Y * b.Y) + (a.Z * b.Z) + (a.W * b.W));

		public static float MagnitudeSquared(Vec4 vector) => vector.MagnitudeSquared();
		public static float Magnitude(Vec4 vector) => vector.Magnitude();
		public static Vec4 Normalized(Vec4 vector) => vector.Normalized();

		public override string ToString() => $"({X}, {Y}, {Z}, {W})";

		public override bool Equals(object obj)
		{
			return obj is Vec4 vec &&
				   X == vec.X &&
				   Y == vec.Y &&
				   Z == vec.Z &&
				   W == vec.W;
		}

		public override int GetHashCode()
		{
			int hashCode = 707706286;
			hashCode = hashCode * -1521134295 + X.GetHashCode();
			hashCode = hashCode * -1521134295 + Y.GetHashCode();
			hashCode = hashCode * -1521134295 + Z.GetHashCode();
			hashCode = hashCode * -1521134295 + W.GetHashCode();
			return hashCode;
		}
	}
}
