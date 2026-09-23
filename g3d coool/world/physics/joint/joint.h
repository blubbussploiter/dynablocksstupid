#pragma once

#include "../../instance.h"
#include "../kernel.h"

namespace block
{
	class PartInstance;
	namespace Physics
	{
		class Body;
		class Primitive;

		class Joint : public Instance
		{
		protected:
			dJointID odeJoint;
			/* use parts instead of bodys -> more accurate */
			/* part1 -> part with joints */
			/* part2 -> connected part*/
			PartInstance* part1;
			PartInstance* part2;
		public:
			virtual void breakJoint();
			/* delete the joint temporarily (for anchoring / unanchoring), 
				recreated at the next step */
			virtual void bendJoint() = 0;
			virtual bool connected() = 0;
			virtual void connect() = 0;
			void createJoint(PartInstance* body1, PartInstance* body2);
			void deleteJoint();
			bool connectedToStaticBody();
			Joint();
			~Joint();
		};
	}
}