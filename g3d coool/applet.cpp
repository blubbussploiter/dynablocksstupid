
#include <ode/ode.h>

#include "applet.h"
#include "world/object/partinstance.h"
#include "world/physics/joint/motorJoint.h"

Applet* globalApplet = 0;
ReferenceCountedPointer<block::PartInstance> baseplate = 0;
block::PartInstance* ball = 0;
block::PartInstance* ball2 = 0;
block::PartInstance* ball3 = 0;
block::PartInstance* ball4 = 0;
block::PartInstance* otherBlock = 0;

block::Datamodel* Applet::Datamodel()
{
	return globalApplet->datamodel;
}

void App::main()
{
	globalApplet = new Applet(this);
	globalApplet->setDesiredFrameRate(45);
	globalApplet->onInit();
	globalApplet->run();
}

void Applet::onInit()
{
	if (baseplate.isNull())
	{
		baseplate = new block::PartInstance();
		baseplate->setName("Baseplate");
		baseplate->setColor(Color3(0.156863f, 0.498039f, 0.27451f));
		baseplate->setSize(Vector3(100, 1, 100));
		baseplate->setTopSurface(block::NO_SURFACE);
		baseplate->setAnchored(true);
		baseplate->setParent(datamodel->workspace);

		otherBlock = new block::PartInstance();
		otherBlock->setSize(Vector3(8, 1, 2));
		otherBlock->setPosition(Vector3(0, 2, 4));
		otherBlock->setAnchored(0);
		otherBlock->setColor(Color3::red() * 0.75f);
		otherBlock->setParent(datamodel->workspace);

		ball = new block::PartInstance();
		ball->setShape(block::CYLINDER_BLOCK);
		ball->setName("Ball");
		ball->setNameShown(1);
		ball->setPosition(Vector3(1, 2, 5));
		ball->setCoordinateFrame(ball->getCoordinateFrame() * Matrix3::fromEulerAnglesXYZ(toRadians(90), 0, 0));
		ball->setParent(datamodel->workspace);
		ball->setBottomSurface(block::SurfaceType::MOTOR);
		ball->setTopSurface(block::SurfaceType::NO_SURFACE);
		ball->setSize(Vector3(2, 1, 2));
		ball->setColor(Color3::gray() * 0.75f);

		ball2 = new block::PartInstance();
		ball2->setShape(block::CYLINDER_BLOCK);
		ball2->setName("Ball2");
		ball2->setNameShown(1);
		ball2->setPosition(Vector3(1, 2, 3));
		ball2->setCoordinateFrame(ball2->getCoordinateFrame() * Matrix3::fromEulerAnglesXYZ(toRadians(90), 0, 0));
		ball2->setParent(datamodel->workspace);
		ball2->setTopSurface(block::SurfaceType::MOTOR);
		ball2->setBottomSurface(block::SurfaceType::NO_SURFACE);
		ball2->setSize(Vector3(2, 1, 2));
		ball2->setColor(Color3::gray() * 0.75f);

		ball3 = new block::PartInstance();
		ball3->setShape(block::CYLINDER_BLOCK);
		ball3->setName("Ball");
		ball3->setNameShown(1);
		ball3->setPosition(Vector3(-1, 2, 5));
		ball3->setCoordinateFrame(ball3->getCoordinateFrame() * Matrix3::fromEulerAnglesXYZ(toRadians(90), 0, 0));
		ball3->setParent(datamodel->workspace);
		ball3->setBottomSurface(block::SurfaceType::MOTOR);
		ball3->setTopSurface(block::SurfaceType::NO_SURFACE);
		ball3->setSize(Vector3(2, 1, 2));
		ball3->setColor(Color3::gray() * 0.75f);

		ball4 = new block::PartInstance();
		ball4->setShape(block::CYLINDER_BLOCK);
		ball4->setName("Ball2");
		ball4->setNameShown(1);
		ball4->setPosition(Vector3(-1, 2, 3));
		ball4->setCoordinateFrame(ball4->getCoordinateFrame() * Matrix3::fromEulerAnglesXYZ(toRadians(90), 0, 0));
		ball4->setParent(datamodel->workspace);
		ball4->setTopSurface(block::SurfaceType::MOTOR);
		ball4->setBottomSurface(block::SurfaceType::NO_SURFACE);
		ball4->setSize(Vector3(2, 1, 2));
		ball4->setColor(Color3::gray() * 0.75f);

		block::Physics::MotorJoint* hinge = new block::Physics::MotorJoint();
		hinge->createMotor(otherBlock->getBody(), ball->getBody());

		block::Physics::MotorJoint* hinge2 = new block::Physics::MotorJoint();
		hinge2->createMotor(otherBlock->getBody(), ball2->getBody());

		block::Physics::MotorJoint* hinge3 = new block::Physics::MotorJoint();
		hinge3->createMotor(otherBlock->getBody(), ball3->getBody());

		block::Physics::MotorJoint* hinge4 = new block::Physics::MotorJoint();
		hinge4->createMotor(otherBlock->getBody(), ball4->getBody());
	}
}
void Applet::doGraphics()
{

	app->renderDevice->clear();
	app->renderDevice->setProjectionAndCameraMatrix(app->debugCamera);

	app->renderDevice->setShadeMode(RenderDevice::ShadeMode::SHADE_SMOOTH);

	LightingParameters params = datamodel->renderLevel->getLightingParameters();
	LightingRef lighting = datamodel->renderLevel->getLighting();
	Color3 ambient = (lighting->ambientBottom + lighting->ambientTop) * 0.5f;

	sky->render(params);

	app->renderDevice->enableLighting();

	/* vertex colors kinda dampen / bring out light, too high = ugly, too low = horrid, middle? just right */
	app->renderDevice->setLight(0, GLight::directional(params.lightDirection, params.lightColor * 0.5f, 1, 1));
	app->renderDevice->setAmbientLightColor(ambient);
	app->renderDevice->setSpecularCoefficient(0.5f);

	app->renderDevice->setShininess(50);

		datamodel->renderWorld->beginRender(app->renderDevice);

			datamodel->renderLevel->opaqueLevel->renderGeometry(app->renderDevice);

		datamodel->renderWorld->endRender(app->renderDevice);

	app->renderDevice->pushState();

		app->renderDevice->disableLighting();

		datamodel->renderWorld->beginRender(app->renderDevice);

				datamodel->renderLevel->surfaceLevel->renderGeometry(app->renderDevice);

		datamodel->renderWorld->endRender(app->renderDevice);

	app->renderDevice->popState();

	app->renderDevice->pushState();

	app->renderDevice->setBlendFunc(RenderDevice::BLEND_SRC_ALPHA, RenderDevice::BLEND_ONE_MINUS_SRC_ALPHA);

		datamodel->renderWorld->beginRender(app->renderDevice);

			datamodel->renderLevel->transparentLevel->renderGeometry(app->renderDevice);

		datamodel->renderWorld->endRender(app->renderDevice);

		app->renderDevice->setBlendFunc(RenderDevice::BLEND_ONE, RenderDevice::BLEND_ONE);

	app->renderDevice->popState();

	app->renderDevice->disableLighting();

	datamodel->workspace->doRender(app->renderDevice);

	sky->renderLensFlare(params);

}

void Applet::doLogic()
{
	if (baseplate.notNull())
	{
		if (app->userInput->keyDown(SDLK_r))
		{
			Datamodel()->get()->runService->run();
		}
		if (app->userInput->keyDown(SDLK_u))
		{
			ball->getBody()->applyTorque(Vector3(0, 0, 4));
			ball2->getBody()->applyTorque(Vector3(0, 0, 4));
			ball3->getBody()->applyTorque(Vector3(0, 0, 4));
			ball4->getBody()->applyTorque(Vector3(0, 0, 4));
		}
		if (app->userInput->keyDown(SDLK_j))
		{
			ball->getBody()->applyTorque(Vector3(0, 0, -4));
			ball2->getBody()->applyTorque(Vector3(0, 0, -4));
			ball3->getBody()->applyTorque(Vector3(0, 0, -4));
			ball4->getBody()->applyTorque(Vector3(0, 0, -4));
		}
		if (app->userInput->keyDown(SDLK_k))
		{
			ball->getBody()->applyTorque(Vector3(4, 0, 0));
			ball2->getBody()->applyTorque(Vector3(4, 0, 0));
			ball3->getBody()->applyTorque(Vector3(4, 0, 0));
			ball4->getBody()->applyTorque(Vector3(4, 0, 0));
		}
		if (app->userInput->keyDown(SDLK_h))
		{
			ball->getBody()->applyTorque(Vector3(-4, 0, 0));
			ball2->getBody()->applyTorque(Vector3(-4, 0, 0));
			ball3->getBody()->applyTorque(Vector3(-4, 0, 0));
			ball4->getBody()->applyTorque(Vector3(-4, 0, 0));
		}
	}
}


void Applet::onSimulation(RealTime rdt, SimTime sdt, SimTime idt)
{
	datamodel->runService->step();
}