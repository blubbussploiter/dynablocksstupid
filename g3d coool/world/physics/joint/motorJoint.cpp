#include "motorJoint.h"
#include "../body.h"
#include "../world/object/partInstance.h"

/* might be able to make this just a hinge -- instead of hinge2 */

void block::Physics::MotorJoint::createMotor(PartInstance* connected, PartInstance* motor)
{
	if (odeJoint)
	{
		return;
	}
	createJoint(motor, connected);
	if (connectedToStaticBody())
	{
		odeJoint = dJointCreateHinge(Kernel::get()->world, 0);
	}
	else
	{
		odeJoint = dJointCreateHinge2(Kernel::get()->world, 0);
	}
}

void block::Physics::Joint::breakJoint()
{
	/* also: with bumps, might have to notify parents ; 
		or just pass notification to parent, 
		when bump created: remove child joint reference
		put them in the parent */
	if (part1 && part2)
	{
		bendJoint();
		part1->getBody()->notifyDetachJoint(this);
		part2->getBody()->notifyDetachJoint(this);
		Kernel::get()->removeJoint(this);
		remove();
	}
}

void block::Physics::MotorJoint::bendJoint()
{
	if (odeJoint)
	{
		dJointDestroy(odeJoint);
		deleteJoint();
		odeJoint = 0;
	}
}

void block::Physics::MotorJoint::connectSingleHinge()
{
	if (odeJoint && part1 && part2)
	{
		Body* body1 = part1->getBody();
		Body* body2 = part2->getBody();
		Vector3 body1Pos = part1->getPosition();
		Vector3 body2Pos = part2->getPosition();
		Vector3 direction = (body2Pos - body1Pos).direction();
		float magnitude = (body2Pos - body1Pos).magnitude();
		Vector3 anchor = body2Pos + (direction * magnitude * 0.01f);
		Vector3 axis = getRotAxis2();

		dBodyID unanchoredBody;
		if (!body1->anchored)
		{
			unanchoredBody = body1->body;
		}
		else
		{
			unanchoredBody = body2->body;
		}
		dJointAttach(odeJoint, 0, unanchoredBody);
		dJointSetHingeAnchor(odeJoint, anchor.x, anchor.y, anchor.z);
		dJointSetHingeAxis(odeJoint, axis.x, axis.y, axis.z);
	}
}

void block::Physics::MotorJoint::connectHinge2()
{
	if (odeJoint && part1 && part2)
	{
		Body* body1 = part1->getBody();
		Body* body2 = part2->getBody();

		Vector3 axis2 = getRotAxis2();
		Vector3 anchor = getAnchorPt();

		printf("anchor = %f, %f, %f\n", anchor.x, anchor.y, anchor.z);
		printf("axis = %f, %f, %f\n", axis2.x, axis2.y, axis2.z);

		dJointAttach(odeJoint, body1->body, body2->body);
		dJointSetHinge2Anchor(odeJoint, anchor.x, anchor.y, anchor.z);
		dJointSetHinge2Param(odeJoint, dParamFMax, 500.0f);
		dJointSetHinge2Param(odeJoint, dParamHiStop, 0);
		dJointSetHinge2Param(odeJoint, dParamLoStop, 0);
		dJointSetHinge2Param(odeJoint, dParamSuspensionERP, 0.2f);
		dJointSetHinge2Param(odeJoint, dParamSuspensionCFM, 0.2f);
		dJointSetHinge2Param(odeJoint, dParamStopERP, 0.4f);
		dJointSetHinge2Param(odeJoint, dParamStopCFM, 0.2f);
		dJointSetHinge2Param(odeJoint, dParamBounce, 0.2f);
		dJointSetHinge2Param(odeJoint, dParamFudgeFactor, 0.4f);
		dJointSetHinge2Axis1(odeJoint, 0, 1, 0); /* we need to find this naturally */
		dJointSetHinge2Axis2(odeJoint, axis2.x, axis2.y, axis2.z);
	}
}

/* put this somewhere else hinge will need it too */
Vector3 block::Physics::MotorJoint::getRotAxis2()
{
	Primitive* prim1 = part1->getPrimitive();
	Primitive* prim2 = part2->getPrimitive();
	HitTestInfo info = g3dHitTest(prim1, prim2);
	if (info.contact_nm.size() > 0)
	{
		return info.contact_nm[0];
	}
	printf("WHAT THE FUCK\n");
	return Vector3::ZERO;
}

Vector3 block::Physics::MotorJoint::getAnchorPt()
{
	Vector3 body1Pos = part1->getPosition();
	Vector3 body2Pos = part2->getPosition();
	Vector3 direction = (body2Pos - body1Pos).direction();
	float magnitude = (body2Pos - body1Pos).magnitude();
	return body2Pos + (direction * magnitude * 0.01f);
}

void block::Physics::MotorJoint::connect()
{
	if (part1 && part2)
	{
		if (odeJoint)
		{
			if (connectedToStaticBody())
			{
				connectSingleHinge();
			}
			else
			{
				connectHinge2();
			}
		}
		else
		{
			createMotor(part1, part2);
		}
	}
}

bool block::Physics::MotorJoint::connected()
{
	if (odeJoint && part1 && part2)
	{
		if (connectedToStaticBody())
		{
			return dJointGetBody(odeJoint, 1) != 0;
		}
		else
		{
			return dAreConnectedExcluding(part1->getBody()->body, 
				part2->getBody()->body, dJointTypeContact);
		}
	}
	return false;
}

block::Physics::MotorJoint::~MotorJoint()
{

}
