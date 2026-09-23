
#include <G3DAll.h>

#include "joint/joint.h"
#include "../applet.h"
#include "primitive.h"
#include "body.h"
#include "kernel.h"

void block::Physics::Kernel::addJoint(Joint* joint)
{
	if (!worldJoints.contains(joint))
	{
		worldJoints.append(joint);
	}
}

void block::Physics::Kernel::removeJoint(Joint* joint)
{
	if (worldJoints.contains(joint))
	{
		worldJoints.remove(worldJoints.findIndex(joint));
	}
}

void block::Physics::Kernel::buildJoints()
{
	/* add a checker here to make sure we dont do this every simulation step */
	for (int i = 0; i < worldJoints.size(); i++)
	{
		Joint* joint = worldJoints[i];
		if (!joint->connected())
		{
			printf("Build joint %d\n", i);
			joint->connect();
		}
	}
}

void block::Physics::Kernel::step(float step)
{
	dJointGroupEmpty(contacts);
	dSpaceCollide(space, 0, &Kernel::collisionCallback);
	dWorldQuickStep(world, step);
	buildJoints();
}

dBodyID block::Physics::Kernel::createBody(const Vector3& size, const CoordinateFrame& position, Geometry geometry)
{
	dBodyID body = 0;
	Vector3 translation = position.translation;
	Matrix3 rotation = position.rotation;

	dMass mass;
	switch (geometry)
	{
		case Geometry::GEOMETRY_BLOCK:
		{
			mass.setBox(0.05f, size.x / 2, size.y / 2, size.z / 2);
			break;
		}
		default:
		{
			mass.setSphere(0.05f, size.x / 2);
			break;
		}
	}

	body = dBodyCreate(world);
	dMatrix3 dRotation = g3dtoode(rotation);
	dBodySetRotation(body, dRotation);
	dBodySetPosition(body, translation.x, translation.y, translation.z);
	dBodySetMass(body, &mass);
	dBodySetData(body, 0);

	return body;
}

void block::Physics::Kernel::removeBody(dBodyID body)
{
	if (body)
	{
		for (int i = 0; i < dBodyGetNumJoints(body); i++) {
			dBodyID b1 = dJointGetBody(dBodyGetJoint(body, i), 0);
			dBodyID b2 = dJointGetBody(dBodyGetJoint(body, i), 1);

			dJointDestroy(dBodyGetJoint(body, i));
		}

		//for (int i = 0; i < attachedPrimitives.size(); i++)
		//{
		//	Primitive* primitive = attachedPrimitives[i];
		//	detachPrimitive(primitive);
		//}

		dBodyDestroy(body);
		body = NULL;
	}
}

void block::Physics::Kernel::collisionCallback(void* data, dGeomID geom1, dGeomID geom2)
{
	int i;

	Primitive* prim0 = (Primitive*)dGeomGetData(geom1);
	Primitive* prim1 = (Primitive*)dGeomGetData(geom2);

	if (prim0 && prim1)
	{

		dBodyID b1 = dGeomGetBody(geom1);
		dBodyID b2 = dGeomGetBody(geom2);

		if (b1 && b2 && dAreConnected(b1, b2))
		{
			return;
		}

		dContact contact[4]{};
		int numc = dCollide(geom1, geom2, 4, &contact[0].geom, sizeof(dContact));

		if (numc > 0)
		{
			for (i = 0; i < numc; i++)
			{

				contact[i].surface.mode = dContactBounce | dContactSlip1 | dContactSlip2 | dContactSoftERP | dContactSoftCFM | dContactApprox1;

				// Define contact surface properties

				float fric = max(prim0->friction, prim1->friction);
				float elas = max(prim0->elasticity, prim1->elasticity);

				contact[i].surface.bounce = elas * 2; //Elasticity
				contact[i].surface.mu = fric; //Friction
				contact[i].surface.slip1 = 0.0f;
				contact[i].surface.slip2 = 0.0f;
				contact[i].surface.soft_erp = 0.2f;
				contact[i].surface.soft_cfm = 0.05f;

				dJointID c = dJointCreateContact(
					Kernel::get()->world,
					Kernel::get()->contacts,
					&contact[i]);

				dJointAttach(c, b1, b2);
			}

		}

	}

}

block::Physics::Kernel* block::Physics::Kernel::get()
{
	return globalApplet->Datamodel()->runService->physics;
}

block::Physics::Kernel::Kernel()
{
	world = dWorldCreate();
	space = dSimpleSpaceCreate(0);
	contacts = dJointGroupCreate(0);
	dWorldSetGravity(world, 0, -9.81F, 0);
	dWorldSetAutoDisableFlag(world, 1);
	dWorldSetAutoDisableLinearThreshold(world, 0.1f);
	dWorldSetAutoDisableAngularThreshold(world, 0.1f);
	dWorldSetAutoDisableSteps(world, 20);
	dWorldSetERP(world, 0.5f);
}

block::Physics::Kernel::~Kernel()
{
	dCloseODE();
	dJointGroupDestroy(contacts);
	dSpaceDestroy(space);
	dWorldDestroy(world);
}

block::Physics::HitTestInfo block::Physics::g3dHitTest(Body* body1, Body* body2)
{
	HitTestInfo info;
	CollisionDetection::penetrationDepthForFixedBoxFixedBox(body1->getBox(), body2->getBox(), info.contact_pt, info.contact_nm);
	return info;
}

block::Physics::HitTestInfo block::Physics::g3dHitTest(Primitive* prim1, Primitive* prim2)
{
	HitTestInfo info;
	CollisionDetection::penetrationDepthForFixedBoxFixedBox(prim1->getBox(), prim2->getBox(), info.contact_pt, info.contact_nm);
	return info;
}

bool block::Physics::g3dPointInExtents(Body* body, const Vector3& pt)
{
	return (body->getBox().contains(pt * 0.001f));
}
