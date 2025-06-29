namespace Cataclysm.Math
{
	/// <summary>
	/// Represents a 2-dimensional floating point vector
	/// </summary>
	public struct Vec2
    {
        public float X, Y;

        public static Vec2 Zero => new Vec2(0.0f);
		public static Vec2 One => new Vec2(1.0f);
		public static Vec2 UnitX => new Vec2(1.0f, 0.0f);
		public static Vec2 UnitY => new Vec2(0.0f, 1.0f);

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
        public static Vec2 operator *(float scalar, Vec2 vector) => new Vec2(vector.X * scalar, vector.Y * scalar);
        public static Vec2 operator *(Vec2 a, Vec2 b) => new Vec2(a.X * b.X, a.Y * b.Y);
		public static Vec2 operator /(Vec2 vector, float scalar) => new Vec2(vector.X / scalar, vector.Y / scalar);
		public static Vec2 operator -(Vec2 vector) => new Vec2(-vector.X, -vector.Y);
		public static Vec2 operator -(Vec2 a, Vec2 b) => new Vec2(a.X - b.X, a.Y - b.Y);
		public static bool operator ==(Vec2 a, Vec2 b) => ((a.X == b.X) && (a.Y == b.Y));
		public static bool operator !=(Vec2 a, Vec2 b) => !(a == b);

		public float MagnitudeSquared() => ((X * X) + (Y * Y));
		public float Magnitude() => (float)System.Math.Sqrt(MagnitudeSquared());
		public Vec2 Normalized() => (Magnitude() > 0) ? new Vec2(X / Magnitude(), Y / Magnitude()) : new Vec2(0.0f, 0.0f);

		public static Vec2 Lerp(Vec2 a, Vec2 b, float timestep) => (a + (b - a) * timestep);
		public static float DotProduct(Vec2 a, Vec2 b) => ((a.X * b.X) + (a.Y * b.Y));

		public static float MagnitudeSquared(Vec2 vector) => vector.MagnitudeSquared();
		public static float Magnitude(Vec2 vector) => vector.Magnitude();
		public static Vec2 Normalized(Vec2 vector) => vector.Normalized();

		public override string ToString() => $"({X}, {Y})";

		public override bool Equals(object obj)
		{
			return obj is Vec2 vec &&
				   X == vec.X &&
				   Y == vec.Y;
		}

		public override int GetHashCode()
		{
			int hashCode = 1861411795;
			hashCode = hashCode * -1521134295 + X.GetHashCode();
			hashCode = hashCode * -1521134295 + Y.GetHashCode();
			return hashCode;
		}
	}
}
