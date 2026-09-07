#pragma once

#include <G3DAll.h>
#include "../instance.h"

namespace block
{
	class IRenderable : public Instance
	{
	public:
		virtual void doRender(RenderDevice* renderDevice)   = 0;
		virtual void doRender2D(RenderDevice* renderDevice) = 0;
	};
}