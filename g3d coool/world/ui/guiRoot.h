#pragma once

#include <G3DAll.h>
#include "../instance.h"

namespace block
{
	class GuiRoot : public Instance
	{
	protected:
		GFontRef trek;
		GFontRef dominant;
	public:

		GFontRef loadFont(const std::string& fontName);
		GFontRef getTrekFont();
		GFontRef getDominantFont();

		GuiRoot();
		~GuiRoot();
	};
}