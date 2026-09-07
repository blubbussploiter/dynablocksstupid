#include "applet.h"

#include "world/object/partinstance.h"

Applet* globalApplet = 0;
ReferenceCountedPointer<block::BlockInstance> baseplate = 0;

block::Datamodel* Applet::Datamodel()
{
	return globalApplet->datamodel;
}

void App::main()
{
	globalApplet = new Applet(this);
	globalApplet->onInit();
	globalApplet->run();
}

void Applet::onInit()
{
	if (baseplate.isNull())
	{
		baseplate = new block::BlockInstance();
		baseplate->setName("Baseplate");
		baseplate->setColor(Color3(0, 0.5f, 0));
		baseplate->setSize(Vector3(20, 1, 20));
		//baseplate->setParent(datamodel->workspace);
		baseplate->setTopSurface(block::SurfaceType::BUMP);

		block::BlockInstance* otherBlock = new block::BlockInstance();
		otherBlock->setParent(datamodel->workspace);
		otherBlock->setPosition(Vector3(0, 2, 0));
	}
}
void Applet::doGraphics()
{

	app->renderDevice->clear();
	app->renderDevice->setProjectionAndCameraMatrix(app->debugCamera);

	app->renderDevice->setShadeMode(RenderDevice::ShadeMode::SHADE_SMOOTH);

	LightingParameters params;
	params.setTime(toSeconds(12, 0, PM));

	sky->render(params);

	app->renderDevice->enableLighting();

	app->renderDevice->setLight(0, GLight::directional(params.lightDirection, params.lightColor * 0.9f, 1, 1));
	app->renderDevice->setAmbientLightColor(params.ambient);
	app->renderDevice->setSpecularCoefficient(1.f);

	app->renderDevice->setShininess(100);

		datamodel->renderWorld->doRender(app->renderDevice);
		datamodel->workspace->doRender(app->renderDevice);

	app->renderDevice->disableLighting();

	sky->renderLensFlare(params);

}
void Applet::doLogic()
{
	if (baseplate.notNull())
	{
		if (app->userInput->keyDown(SDLK_SPACE))
		{
			baseplate->remove();
		}
		if (app->userInput->keyDown(SDLK_2))
		{
			baseplate->setCoordinateFrame(CoordinateFrame(Vector3(0, -3, 0)));
		}
	}
}

void Applet::onSimulation(RealTime rdt, SimTime sdt, SimTime idt)
{

}