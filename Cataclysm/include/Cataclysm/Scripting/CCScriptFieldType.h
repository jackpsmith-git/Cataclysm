#pragma once

namespace Cataclysm
{
	enum class CCScriptFieldType
	{
		None = 0,
		Float, Double,
		Bool, Char, Byte, Short, Int, Long,
		UByte, UShort, UInt, ULong,
		Vec2, Vec3, Vec4,
		Entity, String,
		AudioSource, BoxCollider2D, Camera, CircleCollider2D, CircleRenderer, Rigidbody2D, SpriteRenderer, Text, Transform,
		AudioClip, Font, Texture2D
	};
}
