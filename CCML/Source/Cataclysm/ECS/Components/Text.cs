﻿using Cataclysm.Math;

namespace Cataclysm
{
	/// <summary>
	/// Text component
	/// </summary>
	public class Text : Component
	{
		public string text
		{
			get => InternalCalls.TextComponent_GetText(Entity.ID);
			set => InternalCalls.TextComponent_SetText(Entity.ID, value);
		}

		public Vec4 Color
		{
			get
			{
				InternalCalls.TextComponent_GetColor(Entity.ID, out Vec4 color);
				return color;
			}
			set
			{
				InternalCalls.TextComponent_SetColor(Entity.ID, ref value);
			}
		}

		public float Kerning
		{
			get => InternalCalls.TextComponent_GetKerning(Entity.ID);
			set => InternalCalls.TextComponent_SetKerning(Entity.ID, value);
		}

		public float LineSpacing
		{
			get => InternalCalls.TextComponent_GetLineSpacing(Entity.ID);
			set => InternalCalls.TextComponent_SetLineSpacing(Entity.ID, value);
		}

		public Font Font
		{
			set
			{
				InternalCalls.TextComponent_SetFont(Entity.ID, value.ID);
			}
		}

		public override void Reset() => InternalCalls.TextComponent_Reset(Entity.ID);
	}
}
