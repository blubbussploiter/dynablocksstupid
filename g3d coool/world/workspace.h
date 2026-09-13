#pragma once

#include "model.h"
#include "object/renderstub.h"

namespace block
{
	class PartInstance;

	class WorkspaceInstance : public ModelInstance
	{
	protected:
		Array<IRenderable*> workspaceRenderables;
	public:

		void createBlockGeometry(PartInstance* block);
		void doRender(RenderDevice* renderDevice);
		void onDescendentAdded(Instance* parent, Instance* descendent);
		void onDescendentRemoved(Instance* parent, Instance* descendent);
		WorkspaceInstance();
		~WorkspaceInstance();
	};
}