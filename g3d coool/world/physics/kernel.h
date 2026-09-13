#pragma once

#include <G3DAll.h>
#include "ode/ode.h"

#define g3dtoode(rotation) { rotation[0][0], rotation[0][1], rotation[0][2], 0,  rotation[1][0], rotation[1][1], rotation[1][2], 0, rotation[2][0], rotation[2][1], rotation[2][2], 0 }

namespace block
{
	namespace Physics
	{
		enum Geometry
		{
			GEOMETRY_SPHERE,
			GEOMETRY_BLOCK
		};

		struct Velocity
		{
			Vector3 rot, lin;
		};

		class Kernel
		{
		protected:
			friend class Primitive;
			friend class Joint;
			friend class MotorJoint;
			dWorldID world;
			dSpaceID space;
			dJointGroupID contacts;
			dJointGroupID joints;
			Array<Joint*> worldJoints;
		public:
			void addJoint(Joint* joint);
			void removeJoint(Joint* joint);
			void buildJoints();
			void step(float step);
			dBodyID createBody(const Vector3& size, const CoordinateFrame& position, Geometry geometry);
			void removeBody(dBodyID body);
			static void collisionCallback(void* data, dGeomID geom1, dGeomID geom2);
			static Kernel* get();
			Kernel();
			~Kernel();
		};
	}
}