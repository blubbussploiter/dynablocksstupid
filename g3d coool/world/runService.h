#pragma once

#include "instance.h"
#include "object/stepperstub.h"
#include "physics/kernel.h"

namespace block
{
	class RunService : public Instance
	{
		/* probably move kernel into here */
	protected:
		friend class Physics::Kernel;
		Physics::Kernel* physics;
		Array<ISteppable*> steppers;
		bool running;
	public:

		void run();
		void stop();

		void addStepper(ISteppable* stepper);
		void removeStepper(ISteppable* stepper);
		void step();

		RunService();
		~RunService();
	};
}