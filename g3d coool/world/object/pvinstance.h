#pragma once

#include <G3DAll.h>
#include "renderstub.h"
#include "../ui/control.h"

namespace block
{
	class PVInstance : public IRenderable
	{
	protected:
		bool nameShown;
		bool controllerFlagShown;
		ControllerRef controller;
		/* physics body and stuff */
	public:

		void doRender(RenderDevice* renderDevice) {}
		void doRender2D(RenderDevice* renderDevice) {}

		PVInstance(const char* name);
	};
}