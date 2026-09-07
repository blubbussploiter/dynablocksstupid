#include "guiRoot.h"
#include "../../ezfs.h"

GFontRef block::GuiRoot::loadFont(const std::string& fontName)
{
	return GFont::fromFile(GetContentFile("font/" + fontName));
}

GFontRef block::GuiRoot::getTrekFont()
{
	if (trek.isNull())
	{
		trek = loadFont("lighttrek-small.fnt");
	}
	return trek;
}

GFontRef block::GuiRoot::getDominantFont()
{
	if (dominant.isNull())
	{
		dominant = loadFont("dominant-small.fnt");
	}
	return dominant;
}

block::GuiRoot::GuiRoot()
{
	name = "GuiRoot";
	className = "GuiRoot";
}

block::GuiRoot::~GuiRoot()
{
	delete trek.getPointer();
	trek = 0;
}
