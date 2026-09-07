#pragma once

#include <G3DAll.h>
#include "world/datamodel.h"
#include "ezfs.h"

class Applet;
extern Applet* globalApplet;

class App : public GApp
{
	friend class Applet;
private:
public:

	App(const GAppSettings& settings) : GApp(settings) {}
	void main();
};

class Applet : public GApplet
{
private:
	App* app;
	SkyRef sky;
	block::Datamodel* datamodel;
public:

	void doGraphics();
	void doLogic();
	void onSimulation(RealTime rdt, SimTime sdt, SimTime idt);
	void onInit();

	static block::Datamodel* Datamodel();

	Applet(App* app) : GApplet(app), app(app) 
	{ 
		sky = Sky::create(app->renderDevice, app->dataDir + "sky/");
		datamodel = new block::Datamodel();
	}
};