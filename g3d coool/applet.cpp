
#include <ode/ode.h>

#include "applet.h"
#include "world/object/partinstance.h"
#include "world/physics/joint/motorJoint.h"
#include "world/physics/joint/bumpJoint.h"

/* to do: clean up ode you dunce */

Applet* globalApplet = 0;
ReferenceCountedPointer<block::PartInstance> baseplate = 0;
block::PartInstance* board = 0;
block::PartInstance* wheel1 = 0;
block::PartInstance* wheel2 = 0;
block::PartInstance* wheel3 = 0;
block::PartInstance* wheel4 = 0;
block::PartInstance* arm1 = 0;
block::PartInstance* arm2 = 0;

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

		wheel1 = new block::PartInstance();
		wheel1->setName("Wheel 1");
		//wheel1->setNameShown(1);
		wheel1->setShape(block::CYLINDER_BLOCK);
		wheel1->setColor(Color3(0.74902f, 0.717647f, 0.694118f));
		wheel1->setSize(Vector3(2, 2, 2));
		wheel1->setPosition(Vector3(-3, 3.8, 10));
		wheel1->setCoordinateFrame(wheel1->getCoordinateFrame() * Matrix3::fromEulerAnglesXYZ(toRadians(90), 0, toRadians(90)));
		wheel1->setTopSurface(block::MOTOR);
		wheel1->setParent(datamodel->workspace);

		wheel2 = new block::PartInstance();		
		wheel2->setName("Wheel 2");
		//wheel2->setNameShown(1);
		wheel2->setShape(block::CYLINDER_BLOCK);
		wheel2->setColor(Color3(0.74902f, 0.717647f, 0.694118f));
		wheel2->setSize(Vector3(2, 2, 2));
		wheel2->setPosition(Vector3(-3, 3.8, 5));
		wheel2->setCoordinateFrame(wheel2->getCoordinateFrame() * Matrix3::fromEulerAnglesXYZ(toRadians(90), 0, toRadians(90)));
		wheel2->setTopSurface(block::MOTOR);
		wheel2->setParent(datamodel->workspace);		
		
		wheel3 = new block::PartInstance();
		wheel3->setShape(block::CYLINDER_BLOCK);		
		wheel3->setName("Wheel 3");
		//wheel3->setNameShown(1);
		wheel3->setColor(Color3(0.74902f, 0.717647f, 0.694118f));
		wheel3->setSize(Vector3(2, 2, 2));
		wheel3->setPosition(Vector3(-7, 3.8, 10));
		wheel3->setCoordinateFrame(wheel3->getCoordinateFrame() * Matrix3::fromEulerAnglesXYZ(toRadians(90), 0, toRadians(-90)));
		wheel3->setTopSurface(block::MOTOR);
		wheel3->setParent(datamodel->workspace);

		wheel4 = new block::PartInstance();
		wheel4->setShape(block::CYLINDER_BLOCK);		
		wheel4->setName("Wheel 4");
		//wheel4->setNameShown(1);
		wheel4->setColor(Color3(0.74902f, 0.717647f, 0.694118f));
		wheel4->setSize(Vector3(2, 2, 2));
		wheel4->setPosition(Vector3(-7, 3.8, 5));
		wheel4->setCoordinateFrame(wheel4->getCoordinateFrame() * Matrix3::fromEulerAnglesXYZ(toRadians(90), 0, toRadians(-90)));
		wheel4->setTopSurface(block::MOTOR);
		wheel4->setParent(datamodel->workspace);
		 
		board = new block::PartInstance();
		board->setPosition(Vector3(-5, 4.3f, 7.5f));
		board->setSize(Vector3(2, 1, 7));
		board->setColor(Color3(0.705882, 0.517647, 0.333333));
		board->setParent(datamodel->workspace);

		block::PartInstance* feet = new block::PartInstance();
		feet->setPosition(Vector3(-4.5, 5.8f, 7));
		feet->setSize(Vector3(1, 2, 2));
		feet->setColor(Color3(0.0509804, 0.411765, 0.670588));
		feet->setParent(datamodel->workspace);

		block::PartInstance* body1 = new block::PartInstance();
		body1->setPosition(Vector3(-4.5, 7.3f, 7));
		body1->setSize(Vector3(1, 1, 2));
		body1->setColor(Color3(0.74902f, 0.717647f, 0.694118f));
		body1->setParent(datamodel->workspace);

		block::PartInstance* body2 = new block::PartInstance();
		body2->setPosition(Vector3(-4.5, 8.3f, 7));
		body2->setSize(Vector3(1, 1, 2));
		body2->setColor(Color3(0.0509804, 0.411765, 0.670588));
		body2->setParent(datamodel->workspace);
		body2->setFrontSurface(block::MOTOR);
		body2->setBackSurface(block::MOTOR);

		block::PartInstance* head = new block::PartInstance();
		head->setPosition(Vector3(-4.5, 9.6f, 7));
		head->setShape(block::BALL_BLOCK);
		head->setSize(Vector3(2,2, 2));
		head->setColor(Color3(0.976471, 0.839216, 0.180392));
		head->setParent(datamodel->workspace);
		head->setTopSurface(block::NO_SURFACE);

		arm1 = new block::PartInstance();
		arm1->setPosition(Vector3(-4.5, 7.8, 5.5f));
		arm1->setSize(Vector3(1, 2, 1));
		arm1->setColor(Color3(0.976471, 0.839216, 0.180392));
		arm1->setParent(datamodel->workspace);

		arm2 = new block::PartInstance();
		arm2->setPosition(Vector3(-4.5, 7.8, 8.5f));
		arm2->setSize(Vector3(1, 2, 1));
		arm2->setColor(Color3(0.976471, 0.839216, 0.180392));
		arm2->setParent(datamodel->workspace);

		block::PartInstance* bullshit1 = new block::PartInstance();
		bullshit1->setColor(Color3::black());
		bullshit1->setTransparency(0.5f);
		bullshit1->setSize(Vector3(4, 4, 4));
		bullshit1->setPosition(Vector3(4, 10, 0));
		bullshit1->setParent(datamodel->workspace);

		block::PartInstance* bullshit2 = new block::PartInstance();
		bullshit2->setColor(Color3::gray());
		bullshit2->setSize(Vector3(2, 4, 2));
		bullshit2->setPosition(Vector3(4, 14, 0));
		bullshit2->setParent(datamodel->workspace);

		block::PartInstance* bullshit3 = new block::PartInstance();
		bullshit3->setColor(Color3::gray());
		bullshit3->setSize(Vector3(2, 4, 2));
		bullshit3->setPosition(Vector3(4, 6, 0));
		bullshit3->setParent(datamodel->workspace);

		block::PartInstance* bullshit4 = new block::PartInstance();
		bullshit4->setColor(Color3::gray());
		bullshit4->setSize(Vector3(2, 2, 4));
		bullshit4->setPosition(Vector3(4, 10, 4));
		bullshit4->setParent(datamodel->workspace);
		
		block::PartInstance* bullshit5 = new block::PartInstance();
		bullshit5->setColor(Color3::gray());
		bullshit5->setSize(Vector3(2, 2, 4));
		bullshit5->setPosition(Vector3(4, 10, -4));
		bullshit5->setParent(datamodel->workspace);

		(new block::Physics::BumpJoint())->createLink(bullshit1, bullshit2);
		(new block::Physics::BumpJoint())->createLink(bullshit1, bullshit3);
		(new block::Physics::BumpJoint())->createLink(bullshit1, bullshit4);
		(new block::Physics::BumpJoint())->createLink(bullshit1, bullshit5);
		(new block::Physics::MotorJoint())->createMotor(board, wheel1);
		(new block::Physics::MotorJoint())->createMotor(board, wheel2);
		(new block::Physics::MotorJoint())->createMotor(board, wheel3);
		(new block::Physics::MotorJoint())->createMotor(board, wheel4);
		(new block::Physics::MotorJoint())->createMotor(arm1, body2);
		(new block::Physics::MotorJoint())->createMotor(arm2, body2);
		(new block::Physics::BumpJoint())->createLink(board, feet);
		(new block::Physics::BumpJoint())->createLink(feet, body1);
		(new block::Physics::BumpJoint())->createLink(body1, body2);
		(new block::Physics::BumpJoint())->createLink(body2, head);
		//(new block::Physics::MotorJoint())->createMotor(head, body2);
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


	app->renderDevice->setObjectToWorldMatrix(CoordinateFrame());

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

	}
}


void Applet::onSimulation(RealTime rdt, SimTime sdt, SimTime idt)
{
	datamodel->runService->step();
}