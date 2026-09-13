#include "primitive.h"
#include "body.h"

void block::Physics::Primitive::setPosition(const CoordinateFrame& position)
{
	if (geom[0])
	{
		Vector3 translation = position.translation;
		dMatrix3 dRotation = g3dtoode(position.rotation);
		dGeomSetPosition(geom[0], translation.x, translation.y, translation.z);
		dGeomSetRotation(geom[0], dRotation);
	}
}

CoordinateFrame block::Physics::Primitive::getPosition()
{
	if (geom[0])
	{
		const dReal* position = dGeomGetPosition(geom[0]);
		const dReal* rotation = dGeomGetRotation(geom[0]);
		return CoordinateFrame(Matrix3(rotation[0], rotation[1], rotation[2],
			rotation[4], rotation[5], rotation[6],
			rotation[8], rotation[9], rotation[10]),
			Vector3(position[0], position[1], position[2]));
	}
	return CoordinateFrame();
}

void block::Physics::Primitive::setSize(const Vector3& size)
{
	if (geom[0])
	{
		switch (geometry)
		{
			case Geometry::GEOMETRY_BLOCK:
			{
				dGeomBoxSetLengths(geom[0], size.x / 2, size.y / 2, size.z / 2);
				break;
			}
			default:
			{
				dGeomSphereSetRadius(geom[0], size.y / 8);
				break;
			}
		}
	}
}

void block::Physics::Primitive::modifyOffsetWorldCoordinateFrame(CoordinateFrame offset)
{

	if (body)
	{

		if (!geom[0]) return;

		Vector3 position = offset.translation;
		Matrix3 rotation = offset.rotation;

		float dRotation[12] = g3dtoode(rotation);

		if (dGeomGetBody(geom[0]))
		{
			dGeomSetOffsetWorldPosition(geom[0], position.x, position.y, position.z);
			dGeomSetOffsetWorldRotation(geom[0], dRotation);
		}

	}
}

block::Physics::Primitive::Primitive(const Vector3& size, const CoordinateFrame& position, Geometry geometry) : Primitive()
{
	if (!geom[0])
	{
		/* identity */
		Vector3 translation = position.translation;
		Matrix3 rotation = position.rotation;

		switch (geometry)
		{
		case Geometry::GEOMETRY_BLOCK:
		{
			geom[0] = dCreateBox(Kernel::get()->space, size.x / 2, size.y / 2, size.z / 2);
			break;
		}
		default:
		{
			geom[0] = dCreateSphere(Kernel::get()->space, size.x / 8); /* why are spheres so big */
			break;
		}
		}
		this->geometry = geometry;
		float dRotation[12] = g3dtoode(rotation);
		dGeomSetPosition(geom[0], translation.x, translation.y, translation.z);
		dGeomSetRotation(geom[0], dRotation);
		dGeomSetData(geom[0], this);
	}
}

void block::Physics::Primitive::attach(Body* body)
{
	if (geom[0] && body->body)
	{
		dGeomSetBody(geom[0], body->body);
		body->notifyAttach(this);
		this->body = body;
	}
}

void block::Physics::Primitive::detach()
{
	if (geom[0])
	{
		dGeomSetBody(geom[0], 0);
		if (body)
		{
			body->notifyDetach(this);
			body = 0;
		}
	}
}

block::Physics::Primitive::Primitive()
{
	elasticity = 0.5f;
	friction = 0.4f;
	body = 0;
	geom[0] = 0;
	geometry = GEOMETRY_BLOCK;
}

block::Physics::Primitive::~Primitive()
{

}
