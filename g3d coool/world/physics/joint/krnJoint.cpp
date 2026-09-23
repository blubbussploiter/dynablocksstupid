#include "joint.h"
#include "../body.h"
#include "../world/object/partInstance.h"

block::Physics::Joint::Joint()
{
	odeJoint = 0;
	part1 = 0;
	part2 = 0;
	Kernel::get()->addJoint(this);
}

bool block::Physics::Joint::connectedToStaticBody()
{
	return (part1->getAnchored() || part2->getAnchored());
}

void block::Physics::Joint::createJoint(PartInstance* body1, PartInstance* body2)
{
	this->part1 = body1;
	this->part2 = body2;
	if (part1->getBody() && part2->getBody()) {
		part1->getBody()->notifyAttachJoint(this);
		part2->getBody()->notifyAttachJoint(this);
	}
}

void block::Physics::Joint::deleteJoint()
{
	part1->getBody()->notifyDetachJoint(this);
	part2->getBody()->notifyDetachJoint(this);
}

block::Physics::Joint::~Joint()
{
	/* maybe delete joint? might get handled by derived classes */
	Kernel::get()->removeJoint(this);
}
