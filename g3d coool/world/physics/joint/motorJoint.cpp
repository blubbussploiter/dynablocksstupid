#include "motorJoint.h"
#include "../body.h"

void block::Physics::MotorJoint::createMotor(Body* body1, Body* body2)
{
	odeJoint = dJointCreateHinge2(Kernel::get()->world, Kernel::get()->joints);
	//connectingJoint = dJointCreateBall(Kernel::get()->world, Kernel::get()->joints);
	this->body1 = body1;
	this->body2 = body2;
}

void block::Physics::MotorJoint::connect()
{
	if (odeJoint && body1 && body2)
	{
		printf("Attach motor\n");

		Vector3 body1Pos = body1->getPosition().translation;
		Vector3 body2Pos = body2->getPosition().translation;
		Vector3 direction = (body2Pos - body1Pos).direction();
		float magnitude = (body2Pos - body1Pos).magnitude();

		Vector3 anchor = body2Pos + (direction * magnitude * 0.01f);

		dJointAttach(odeJoint, body1->body, body2->body);
		dJointSetHinge2Anchor(odeJoint, anchor.x, anchor.y, anchor.z);
		dJointSetHinge2Param(odeJoint, dParamFMax, 0.5f);
		dJointSetHinge2Param(odeJoint, dParamHiStop, 0);
		dJointSetHinge2Param(odeJoint, dParamLoStop, 0);
		dJointSetHinge2Param(odeJoint, dParamSuspensionERP, 0.8f);
		dJointSetHinge2Param(odeJoint, dParamSuspensionCFM, 0.2f);
		dJointSetHinge2Param(odeJoint, dParamStopERP, 0.8f);
		dJointSetHinge2Param(odeJoint, dParamStopCFM, 0.2f);
		dJointSetHinge2Param(odeJoint, dParamBounce, 0.4f);
		dJointSetHinge2Axis1(odeJoint, 0, 1, 0);
		dJointSetHinge2Axis2(odeJoint, 0, 0, 1);

		//dJointSetAMotorMode(odeJoint, dAMotorEuler);
		//dJointAttach(connectingJoint, body1->body, body2->body);
		//dJointSetBallAnchor(connectingJoint, ballAnchor.x, ballAnchor.y, ballAnchor.z);
	}
}

bool block::Physics::MotorJoint::connected()
{
	if (odeJoint && body1 && body2)
	{
		return dAreConnectedExcluding(body1->body, body2->body, dJointTypeContact);
	}
	return false;
}

block::Physics::MotorJoint::~MotorJoint()
{

}
