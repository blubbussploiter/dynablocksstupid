#pragma once

#include "joint.h"

namespace block
{
	namespace Physics
	{
		/* this is an angular hinge */
		class MotorJoint : public Joint
		{
		protected:
			dJointID connectingJoint;
		public:
			void createMotor(Body* body1, Body* body2);
			void connect();
			bool connected();
			MotorJoint() {}
			~MotorJoint();
		};
	}
}