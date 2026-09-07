#include "workspace.h"
#include "object/partInstance.h"
#include "../applet.h"

block::WorkspaceInstance::WorkspaceInstance()
{
	name = "Workspace";
	className = "Workspace";
}

block::WorkspaceInstance::~WorkspaceInstance()
{

}

void block::WorkspaceInstance::doRender(RenderDevice* renderDevice)
{
	for (int i = 0; i < workspaceRenderables.size(); i++)
	{
		workspaceRenderables[i]->doRender(renderDevice);
	}

	renderDevice->push2D();

	for (int i = 0; i < workspaceRenderables.size(); i++)
	{
		workspaceRenderables[i]->doRender2D(renderDevice);
	}

	renderDevice->pop2D();
}

void block::WorkspaceInstance::onDescendentAdded(Instance* parent, Instance* descendent)
{
	if (isInstanceClassType<IRenderable>(descendent))
	{
		IRenderable* renderStub = toClassType<IRenderable>(descendent);

		if (workspaceRenderables.contains(renderStub))
		{
			return;
		}
		workspaceRenderables.append(renderStub);

		/* level block stuff */

		if (isA(renderStub, "Block"))
		{
			BlockInstance* blockInstance = toClassType<BlockInstance>(renderStub);
			if (blockInstance->_block == -1)
			{
				blockInstance->_block = __render_level->createBlockFromBlockInstance(blockInstance);
				printf("%s -> block %d\n", blockInstance->name.c_str(), blockInstance->_block);
			}
		}
	}

}

void block::WorkspaceInstance::onDescendentRemoved(Instance* parent, Instance* descendent)
{
	if (isInstanceClassType<IRenderable>(descendent))
	{
		IRenderable* renderStub = toClassType<IRenderable>(descendent);
		if (workspaceRenderables.contains(renderStub))
		{
			workspaceRenderables.remove(workspaceRenderables.findIndex(renderStub));
		}

		if (isA(renderStub, "Block"))
		{
			BlockInstance* blockInstance = toClassType<BlockInstance>(renderStub);
			if (blockInstance->_block != -1)
			{
				__render_level->removeBlock(blockInstance->_block);
			}
		}
	}
}