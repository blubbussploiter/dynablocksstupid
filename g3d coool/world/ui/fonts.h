#pragma once

#include <G3DAll.h>
#include "../../ezfs.h"

namespace block
{
	class Fonts
	{
	public:
		static GFontRef loadFont(const std::string& fontName)
		{
			return GFont::fromFile(GetContentFile("font/" + fontName));
		}

		static GFontRef getTrekFont();

		static GFontRef getDominantFont();
	};
}