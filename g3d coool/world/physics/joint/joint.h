#pragma once

#include "../../instance.h"
#include "../kernel.h"

namespace block
{
	namespace Physics
	{
		class Body;
		class Primitive;

		class Joint : public Instance
		{
		protected:
			dJointID odeJoint;
			Body* body1;
			Body* body2;
		public:
			virtual bool connected() = 0;
			virtual void connect() = 0;
			Joint();
			~Joint();
		};
	}
}