#pragma once
#include "stdafx.h"

unsigned char* pixels;
int width;
int height;
struct FreeTypeTest
{
	enum FontBuildMode
	{
		FontBuildMode_FreeType,
		FontBuildMode_Stb
	};

	FontBuildMode BuildMode;
	bool          WantRebuild;
	float         FontsMultiply;
	int           FontsPadding;
	unsigned int  FontsFlags;

	FreeTypeTest()
	{
		BuildMode = FontBuildMode_FreeType;
		WantRebuild = true;
		FontsMultiply = 1.0f;
		FontsPadding = 1;
		FontsFlags = 0;
	}

	// Call _BEFORE_ NewFrame()
	bool UpdateRebuild()
	{
		if (!WantRebuild)
			return false;
		EvolvedIO& io = Evolved::GetIO();
		io.Fonts->TexGlyphPadding = FontsPadding;
		for (int n = 0; n < io.Fonts->ConfigData.Size; n++)
		{
			ImFontConfig* font_config = (ImFontConfig*)&io.Fonts->ConfigData[n];
			font_config->RasterizerMultiply = FontsMultiply;
			font_config->RasterizerFlags = (BuildMode == FontBuildMode_FreeType) ? FontsFlags : 0x00;
		}
		if (BuildMode == FontBuildMode_FreeType)
			ImGuiFreeType::BuildFontAtlas(io.Fonts, FontsFlags);
		else if (BuildMode == FontBuildMode_Stb)
			io.Fonts->Build();
		WantRebuild = false;
		return true;
	}
};