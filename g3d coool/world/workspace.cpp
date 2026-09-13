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

void block::WorkspaceInstance::createBlockGeometry(PartInstance* block)
{
	Physics::Geometry geometry;
	switch (block->blockType)
	{
		case block::NORMAL_BLOCK:
		{
			geometry = Physics::GEOMETRY_BLOCK;
			break;
		}
		default:
		{
			geometry = Physics::GEOMETRY_SPHERE;
			break;
		}
	}
	if (block->body == 0)
	{
		block->body = new Physics::Body(block->getSize(), block->getCoordinateFrame(), geometry);
		block->primitive = new Physics::Primitive(block->getSize(), block->getCoordinateFrame(), geometry);
	}
	block->body->setPosition(block->getCoordinateFrame());
	block->primitive->setPosition(block->getCoordinateFrame());
	block->setAnchored(block->getAnchored());
	printf("Created geom for %s\n", block->getName().c_str());
}

void block::WorkspaceInstance::onDescendentAdded(Instance* parent, Instance* descendent)
{
	/* renderable objects */
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
			PartInstance* blockInstance = toClassType<PartInstance>(renderStub);
			if (blockInstance->_block == -1)
			{
				blockInstance->_block = __render_level->createBlockFromBlockInstance(blockInstance);
			}
		}
	}

	if (isInstanceClassType<ISteppable>(descendent))
	{
		ISteppable* stepperStub = toClassType<ISteppable>(descendent);
		Datamodel::get()->runService->addStepper(stepperStub);

		if (isA(descendent, "Block"))
		{
			createBlockGeometry(toClassType<PartInstance>(descendent));
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
			PartInstance* blockInstance = toClassType<PartInstance>(renderStub);
			if (blockInstance->_block != -1)
			{
				__render_level->removeBlock(blockInstance->_block);
				blockInstance->_block = -1;
			}
		}
	}

	if (isInstanceClassType<ISteppable>(descendent))
	{
		ISteppable* stepperStub = toClassType<ISteppable>(descendent);
		Datamodel::get()->runService->removeStepper(stepperStub);
		if (isA(descendent, "Block"))
		{
			PartInstance* block = toClassType<PartInstance>(descendent);
			if (block->body)
			{
				block->body->remove();
			}
		}
	}
}