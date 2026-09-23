#pragma once

#include "kernel.h"

namespace block
{
	namespace Physics
	{
		class Primitive;
		class MotorJoint;
		class BumpJoint;
		class Joint;

		class Body
		{
		protected:
			friend Primitive;
			friend Joint;
			friend MotorJoint;
			friend BumpJoint;
			bool compound;
			dBodyID body;
			Velocity velocity;
			Vector3 size;
			Geometry geometry;
			CoordinateFrame origin;
			Array<Primitive*> attached;
			Array<Joint*> joints;
			Array<Body*> children;
			Body* parent;
			bool anchored;
		public:
			Velocity getVelocity();
			void setVelocity(const Velocity& vel);
			void setSize(const Vector3& size);
			Vector3 getSize();
			void setPosition(const CoordinateFrame& position);
			CoordinateFrame getPosition();
			Vector3 getTranslation();
			void setTotalMass(float mass);
			void setAnchored(bool anchored);
			void setCofm(const Vector3& newCofm);
			void applyForce(const Vector3& force);
			void applyTorque(const Vector3& torque);
			void notifyDetach(Primitive* primitive);
			void notifyAttach(Primitive* primitive);
			void notifyAttachJoint(Joint* joint);
			void notifyDetachJoint(Joint* joint);
			void joinBody(Body* body);
			void leaveBody();
			void bendJoints();
			bool atRest();
			void wakeUp();
			void create();
			void remove();
			Box getBox();
			AABox getAABox();
			Body(const Vector3& size, const CoordinateFrame& position, Geometry geometry);
			bool isCompound() { return compound; }
			bool linkedTo(Body* body);
			Body* getParent() { return parent; }
			static Body* makeEmptyCompound();
			~Body();
		};
	}
}