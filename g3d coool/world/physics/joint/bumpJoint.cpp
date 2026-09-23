#include "bumpJoint.h"
#include "../body.h"
#include "../world/object/partInstance.h"

void block::Physics::BumpJoint::createLink(PartInstance* part1, PartInstance* part2)
{
	createJoint(part1, part2);
}

void block::Physics::BumpJoint::odeAttachPrimitives(Body* body)
{
	for (int i = 0; i < body->attached.size(); i++)
	{
		/* also adjust mass here too */
		Primitive* prim = body->attached[i];
		if (!prim->attachedTo(collectiveBody))
		{
			CoordinateFrame primPosition = prim->getPosition();
			prim->detach();
			prim->attach(collectiveBody);
			prim->modifyOffsetWorldCoordinateFrame(primPosition);
		}
	}
}

void block::Physics::BumpJoint::bendJoint()
{
	printf("Bend for some reason\n");
}

void block::Physics::BumpJoint::adjustCompoundMass(Vector3& size, CoordinateFrame& cofm)
{
	Vector3 minP, maxP;
	Vector3 minS, maxS;
	Matrix3 rot;

	for (int i = 0; i < collectiveBody->children.size(); i++)
	{
		Body* body = collectiveBody->children[i];
		Vector3 pos = body->getTranslation();

		AABox box = body->getAABox();
		Vector3 high = box.high();
		Vector3 low = box.low();

		if (minP == Vector3::zero())
		{
			minP = pos;
		}
		else
		{
			if (pos.x < minP.x) minP.x = pos.x;
			if (pos.y < minP.y) minP.y = pos.y;
			if (pos.z < minP.z) minP.z = pos.z;
		}

		if (maxP == Vector3::zero())
		{
			maxP = pos;
		}
		else
		{
			if (pos.x > maxP.x) maxP.x = pos.x;
			if (pos.y > maxP.y) maxP.y = pos.y;
			if (pos.z > maxP.z) maxP.z = pos.z;
		}

		if (minS == Vector3::zero())
		{
			minS = low;
		}
		else
		{
			if (low.x < minS.x) minS.x = low.x;
			if (low.y < minS.y) minS.y = low.y;
			if (low.z < minS.z) minS.z = low.z;
		}

		if (maxS == Vector3::zero())
		{
			maxS = high;
		}
		else
		{
			if (high.x > maxS.x) maxS.x = high.x;
			if (high.y > maxS.y) maxS.y = high.y;
			if (high.z > maxS.z) maxS.z = high.z;
		}
	}
	cofm.translation = ((maxP - minP) / 2) + minP;
	size = abs(maxS - minS);
	printf("REAL size = %f, %f, %f\n", size.x, size.y, size.z);
}

void block::Physics::BumpJoint::connect()
{
	CoordinateFrame cofm;
	Vector3 size;
	Body* body1 = part1->getBody();
	Body* body2 = part2->getBody();
	Body* compound1 = 0;
	Body* compound2 = 0;
	if (body1->isCompound())
	{
		compound1 = body1;
	}
	if (body2->isCompound())
	{
		compound2 = body2;
	}

	if (compound1 && compound2)
	{
		printf("boththht \n");
		collectiveBody = compound1;
		body2->leaveBody();
		body2->joinBody(collectiveBody);
		adjustCompoundMass(size, cofm);
	}
	else if (compound1)
	{
		collectiveBody = compound1;
		body2->joinBody(collectiveBody);
		adjustCompoundMass(size, cofm);
	}
	else if (compound2)
	{
		printf("compound2 \n");
		collectiveBody = compound2;
		body1->joinBody(collectiveBody);
		adjustCompoundMass(size, cofm);
	}
	else
	{
		collectiveBody = Body::makeEmptyCompound();
		body1->joinBody(collectiveBody);
		body2->joinBody(collectiveBody);
		adjustCompoundMass(size, cofm);
		if (!collectiveBody->body)
		{
			printf("size = %f, %f, %f\n", size.x, size.y, size.z);
			printf("cofm = %f, %f, %f\n", cofm.translation.x, cofm.translation.y, cofm.translation.z);
			collectiveBody->body = Kernel::get()->createBody(size, cofm, Physics::GEOMETRY_BLOCK);
		}
	}
	for (int i = 0; i < collectiveBody->children.size(); i++)
	{
		odeAttachPrimitives(collectiveBody->children[i]);
	}
	collectiveBody->setTotalMass(collectiveBody->children.size() * 0.5f);
	//collectiveBody->setSize(size);
	collectiveBody->setCofm(cofm.translation);
	printf("okay done\n");
}

bool block::Physics::BumpJoint::connected()
{
	Body* body1 = part1->getBody();
	Body* body2 = part2->getBody();
	if (body1 && body2)
	{
		return body1->linkedTo(body2);
	}
	return 0;
}

block::Physics::BumpJoint::BumpJoint()
{
	collectiveBody = 0;
}

block::Physics::BumpJoint::~BumpJoint()
{
}