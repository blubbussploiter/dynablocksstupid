#include "joint.h"

block::Physics::Joint::Joint()
{
	odeJoint = 0;
	body1 = 0;
	body2 = 0;
	Kernel::get()->addJoint(this);
}

block::Physics::Joint::~Joint()
{
	/* maybe delete joint? might get handled by derived classes */
	Kernel::get()->removeJoint(this);
}
