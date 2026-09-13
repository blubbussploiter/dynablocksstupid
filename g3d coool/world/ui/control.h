#pragma once

#include <G3DAll.h>

namespace block
{
	class Controller : public ReferenceCountedObject
	{
	public:
		enum ControllerType
		{
			NO_CONTROLLER,
			PRIMARY_CONTROLLER,
			SECONDARY_CONTROLLER,
			PAD_ONE_CONTROLLER,
			PAD_TWO_CONTROLLER,
			AI_CHASE_CONTROLLER,
			AI_FLEE_CONTROLLER,
			PLAYER_CONTROLLER,
			NUM_CONTROLLER_TYPES
		};
	};

	typedef ReferenceCountedPointer<Controller> ControllerRef;
}