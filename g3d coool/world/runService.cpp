#include "runService.h"

void block::RunService::run()
{
	running = true;
}

void block::RunService::stop()
{
	running = false;
}

void block::RunService::addStepper(ISteppable* stepper)
{
	if (steppers.contains(stepper)) {
		return;
	}
	steppers.append(stepper);
}

void block::RunService::removeStepper(ISteppable* stepper)
{
	if (steppers.contains(stepper)) { /* might have to make this safe ptr */
		steppers.remove(steppers.findIndex(stepper));
	}
}

void block::RunService::step()
{
	if (running)
	{
		for (int i = 0; i < 8; i++)
		{
			physics->step(0.01f);
		}
		for (int i = 0; i < steppers.size(); i++)
		{
			steppers[i]->onStep();
		}
	}
}

block::RunService::RunService()
{
	physics = new Physics::Kernel();
	running = false;
}

block::RunService::~RunService()
{
}
