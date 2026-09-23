#include "control.h"

Color3 block::Controller::controllerTypeToColor(ControllerType type)
{
	Color3 baseColor = Color3::gray();
	switch (type)
	{
	case block::Controller::PRIMARY_CONTROLLER:
	{
		baseColor = Color3::red();
		break;
	}
	case block::Controller::SECONDARY_CONTROLLER:
	{
		baseColor = Color3::blue();
		break;
	}
	case block::Controller::AI_CHASE_CONTROLLER:
	{
		baseColor = Color3::black();
		break;
	}
	case block::Controller::AI_FLEE_CONTROLLER:
	{
		baseColor = Color3::yellow();
		break;
	}
	}
	return baseColor;
}
