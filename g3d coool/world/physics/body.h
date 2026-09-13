#pragma once

#include "kernel.h"

namespace block
{
	namespace Physics
	{
		class Primitive;

		class Body
		{
		protected:
			friend Primitive;
			friend Joint;
			friend MotorJoint;
			dBodyID body;
			Velocity velocity;
			Vector3 size;
			Geometry geometry;
			CoordinateFrame origin;
			Array<Primitive*> attached;
			bool anchored;
		public:
			Velocity getVelocity();
			void setVelocity(const Velocity& vel);
			void setSize(const Vector3& size);
			void setPosition(const CoordinateFrame& position);
			CoordinateFrame getPosition();
			void setAnchored(bool anchored);
			void applyForce(const Vector3& force);
			void applyTorque(const Vector3& torque);
			void notifyDetach(Primitive* primitive);
			void notifyAttach(Primitive* primitive);
			void create();
			void remove();
			Body(const Vector3& size, const CoordinateFrame& position, Geometry geometry);
			~Body();
		};
	}
}