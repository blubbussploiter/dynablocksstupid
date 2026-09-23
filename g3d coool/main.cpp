#include "applet.h"

int main()
{

	GAppSettings settings;

	settings.window.width = 1024;
	settings.window.height = 820;
	settings.window.resizable = true;
	settings.dataDir = ExePath() + "\\content\\";

	printf("datadir = %s\n", settings.dataDir.c_str());

	App app(settings);
	app.setDebugMode(true);
	app.debugController.setMoveRate(20);
	app.debugController.setActive(true);

	app.run();
	return 0;
}