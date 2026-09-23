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
			Vector3 getRotAxis2();
			Vector3 getAnchorPt();
			void createMotor(PartInstance* connected, PartInstance* motor);
			void bendJoint();
			void connectSingleHinge();
			void connectHinge2();
			void connect();
			bool connected();
			MotorJoint() {}
			~MotorJoint();
		};
	}
}