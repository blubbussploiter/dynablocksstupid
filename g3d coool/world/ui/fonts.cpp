#include "fonts.h"

static GFontRef trek;
static GFontRef dominant;

GFontRef block::Fonts::getTrekFont()
{
	if (trek.isNull())
	{
		trek = loadFont("lighttrek-small.fnt");
	}
	return trek;
}

GFontRef block::Fonts::getDominantFont()
{
	if (dominant.isNull())
	{
		dominant = loadFont("dominant-small.fnt");
	}
	return dominant;
}