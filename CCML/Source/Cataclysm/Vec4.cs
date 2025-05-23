﻿using System;

namespace Cataclysm
{
	/// <summary>
	/// Cataclysm Vector4f
	/// </summary>
	public struct Vec4
	{
		public float X, Y, Z, W;

		public static Vec4 Zero => new Vec4(0.0f);
		
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
		public static Vec4 operator -(Vec4 vector) => new Vec4(-vector.X, -vector.Y, -vector.Z, -vector.W);
		public static Vec4 operator -(Vec4 a, Vec4 b) => new Vec4(a.X - b.X, a.Y - b.Y, a.Z - b.Z, a.W - b.W);

		public float MagnitudeSquared() => ((X * X) + (Y * Y) + (Z * Z) + (W * W));
		public float Magnitude() => (float)Math.Sqrt(MagnitudeSquared());
		public static Vec4 Lerp(Vec4 a, Vec4 b, float timestep) => (a + (b - a) * timestep);
		public static float DotProduct(Vec4 a, Vec4 b) => ((a.X * b.X) + (a.Y * b.Y) + (a.Z * b.Z) + (a.W * b.W));

		public static Vec4 Normalized(Vec4 vector)
		{
			float magnitude = vector.Magnitude();
			if (magnitude > 0)
			{
				return new Vec4(vector.X / magnitude, vector.Y / magnitude, vector.Z / magnitude, vector.W / magnitude);
			}
			else
			{
				return new Vec4(0.0f, 0.0f, 0.0f, 0.0f);
			}
		}

		public override string ToString() => $"({X}, {Y}, {Z}, {W})";
	}
}
