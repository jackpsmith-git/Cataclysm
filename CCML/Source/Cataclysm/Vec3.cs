namespace Cataclysm
{
	/// <summary>
	/// Cataclysm Vector3f
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

        public static Vec3 operator +(Vec3 a, Vec3 b)
        {
            return new Vec3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
        }

        public static Vec3 operator *(Vec3 vector, float scalar)
        {
            return new Vec3(vector.X * scalar, vector.Y * scalar, vector.Z * scalar);
        }
    }
}
