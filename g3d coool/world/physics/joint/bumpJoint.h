#pragma once

#include "joint.h"


namespace block
{
	namespace Physics
	{
		class BumpJoint : public Joint
		{
		protected:
			Body* collectiveBody;
		public:
			void createLink(PartInstance* part1, PartInstance* part2);
			void odeAttachPrimitives(Body* body);
			void adjustCompoundMass(Vector3& size, CoordinateFrame& cofm);
			void connect();
			bool connected();
			void bendJoint();
			BumpJoint();
			~BumpJoint();
		};
	}
}