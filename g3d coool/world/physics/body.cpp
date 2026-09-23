#include "body.h"
#include "primitive.h"
#include "joint/joint.h"

CoordinateFrame block::Physics::Body::getPosition()
{
	if (body)
	{
		const dReal* position = dBodyGetPosition(body);
		const dReal* rotation = dBodyGetRotation(body);
		return CoordinateFrame(Matrix3(rotation[0], rotation[1], rotation[2],
			rotation[4], rotation[5], rotation[6],
			rotation[8], rotation[9], rotation[10]),
			Vector3(position[0], position[1], position[2]));
	}
	return CoordinateFrame();
}

Vector3 block::Physics::Body::getTranslation()
{
	return getPosition().translation;
}

void block::Physics::Body::setTotalMass(float fMass)
{
	if (body)
	{
		dMass mass;
		dBodyGetMass(body, &mass);
		dMassAdjust(&mass, fMass);
		dBodySetMass(body, &mass);
	}
}

void block::Physics::Body::setAnchored(bool anchored)
{
	if (anchored != this->anchored)
	{
		this->anchored = anchored;
		bendJoints(); /* joints need to be rebuilt */
		if (anchored)
		{
			dBodyDisable(body);
			dBodySetGravityMode(body, 0);
		}
		else
		{
			dBodyEnable(body);
			dBodySetGravityMode(body, 1);
		}
	}
}

void block::Physics::Body::setCofm(const Vector3& newCofm)
{
	if (body)
	{
		dMass mass;
		Vector3 translated;
		dBodyGetMass(body, &mass);
		translated.x = (mass.c[0] - newCofm.x);
		translated.y = (mass.c[1] - newCofm.y);
		translated.z = (mass.c[2] - newCofm.z);
		dMassTranslate(&mass, translated.x, translated.y, translated.z);
		dBodySetMass(body, &mass);
	}
}

Vector3 block::Physics::Body::getSize()
{
	return size;
}

void block::Physics::Body::setSize(const Vector3& size)
{
	if (body)
	{
		dMass mass;
		dBodyGetMass(body, &mass);
		this->size = size;
		switch (geometry)
		{
			case GEOMETRY_BLOCK:
			{
				mass.setBox(0.01f, size.x / 2, size.y / 2, size.z / 2);
				break;
			}
			default:
			{
				mass.setSphere(0.01f, size.x / 2);
				break;
			}
		}
		dBodySetMass(body, &mass);
	}
}

void block::Physics::Body::setPosition(const CoordinateFrame& position)
{
	if (body)
	{
		dMatrix3 dRotation = g3dtoode(position.rotation);
		dBodySetPosition(body, position.translation.x, position.translation.y, position.translation.z);
		dBodySetRotation(body, dRotation);
		origin = position;
	}
}

void block::Physics::Body::setVelocity(const Velocity& vel)
{
	if (body)
	{
		if (velocity.lin.length() < vel.lin.length()
			|| velocity.rot.length() < vel.rot.length())
		{
			dBodyEnable(body);
		}
		dBodySetLinearVel(body, vel.lin.x, vel.lin.y, vel.lin.z);
		dBodySetAngularVel(body, vel.rot.x, vel.rot.y, vel.rot.z);
	}
	velocity.lin = vel.lin;
	velocity.rot = vel.rot;
}

block::Physics::Velocity block::Physics::Body::getVelocity()
{
	return velocity;
}

void block::Physics::Body::remove()
{
	if (body != 0)
	{
		Kernel::get()->removeBody(body);
		body = 0;
	}
}

void block::Physics::Body::applyTorque(const Vector3& torque)
{
	if (body)
	{
		if (!dBodyIsEnabled(body))
		{
			dBodyEnable(body);
		}
		dBodyAddTorque(body, torque.x, torque.y, torque.z);
	}
}

void block::Physics::Body::notifyDetach(Primitive* primitive)
{
	if (attached.contains(primitive))
	{
		attached.remove(attached.findIndex(primitive));
	}
}

void block::Physics::Body::notifyAttach(Primitive* primitive)
{
	if (!attached.contains(primitive))
	{
		attached.append(primitive);
	}
}

void block::Physics::Body::notifyAttachJoint(Joint* joint)
{
	if (!joints.contains(joint))
	{
		joints.append(joint);
	}
}

void block::Physics::Body::notifyDetachJoint(Joint* joint)
{
	if (joints.contains(joint))
	{
		joints.remove(joints.findIndex(joint));
	}
}

void block::Physics::Body::joinBody(Body* body)
{
	/* a compound body's sole reason is to house bodies and
	   attach their primitives to itself, then when its no longer needed
	   it dies. */
	if (body->compound && body != parent && body != this)
	{
		/* detach our primitives and attach them to body */
		/* might not work im doing this all in my head (bad practice) */
		/* but in theory: this should assimilate  the old parents children into our new parent, given these should connect */
		if (parent)
		{
			for (int i = 0; i < parent->children.size(); i++)
			{
				Body* child = parent->children[i];
				child->leaveBody();
				child->joinBody(body);
			}
		}
		parent = body;
		parent->children.append(this);
		if (this->body)
		{
			setAnchored(1);
		}
	}
}

void block::Physics::Body::leaveBody()
{
	if (parent)
	{
		for (int i = 0; i < parent->attached.size(); i++)
		{
			Primitive* prim = attached[i];
			prim->detach();
			prim->attach(this);
		}
		if (!body)
		{
			create();
		}
		parent->children.remove(parent->children.findIndex(this));
		parent = 0;
	}
}

void block::Physics::Body::bendJoints()
{
	for (int i = 0; i < joints.size(); i++)
	{
		joints[i]->bendJoint();
	}
}

bool block::Physics::Body::atRest()
{
	if (body)
	{
		return dBodyIsEnabled(body);
	}
	return 0;
}

void block::Physics::Body::wakeUp()
{
	if (body && !anchored)
	{
		return dBodyEnable(body);
	}
}

void block::Physics::Body::applyForce(const Vector3& force)
{
	if (body)
	{
		if (!dBodyIsEnabled(body))
		{
			dBodyEnable(body);
		}
		dBodyAddForce(body, force.x, force.y, force.z);
	}
}

void block::Physics::Body::create()
{
	if (body == 0)
	{
		body = Kernel::get()->createBody(size, origin, geometry);
		setVelocity(velocity);
	}
}

Box block::Physics::Body::getBox()
{
	if (body)
	{
		if(attached.size() == 1)
		{
			Primitive* primitive = attached[0];
			return primitive->getBox();
		}
		else {
			/* multiple: write this later */
		}
	}
}

AABox block::Physics::Body::getAABox()
{
	if (body)
	{
		if (attached.size() == 1)
		{
			Primitive* primitive = attached[0];
			return primitive->getAABox();
		}
		else {
			/* multiple: write this later */
		}
	}
}

bool block::Physics::Body::linkedTo(Body* body)
{
	return (parent && body->parent == parent || body->compound && body == parent || compound && body->compound && body == this);
}

block::Physics::Body* block::Physics::Body::makeEmptyCompound()
{
	Body* newCompound = new Body(Vector3::one(), CoordinateFrame(), GEOMETRY_BLOCK);
	newCompound->remove();
	newCompound->compound = true;
	return newCompound;
}

block::Physics::Body::Body(const Vector3& size, const CoordinateFrame& position, Geometry geometry)
{
	this->size = size;
	this->geometry = geometry;
	this->origin = position;
	body = Kernel::get()->createBody(size, position, geometry);
}

block::Physics::Body::~Body()
{
	Kernel::get()->removeBody(body);
}
