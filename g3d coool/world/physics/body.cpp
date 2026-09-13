#include "body.h"

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
}

void block::Physics::Body::setAnchored(bool anchored)
{
	this->anchored = anchored;
	if (anchored)
	{
		printf("anchored\n");
		//remove();
		//body = 0;
		dBodyDisable(body);
		dBodySetGravityMode(body, 0);
	}
	else
	{
		printf("unachored\n");
		dBodyEnable(body);
		dBodySetGravityMode(body, 1);
		//create();
	}
}

void block::Physics::Body::setSize(const Vector3& size)
{
	if (body)
	{
		dMass mass;
		switch (geometry)
		{
			case GEOMETRY_BLOCK:
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
