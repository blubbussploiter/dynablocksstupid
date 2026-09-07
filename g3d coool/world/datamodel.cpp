#include "datamodel.h"

block::Datamodel::Datamodel()
{
	name = "Datamodel";
	className = "Datamodel";

	workspace = new WorkspaceInstance();
	gui = new GuiRoot();

	workspace->setParent(this);
	gui->setParent(this);
	workspace->isParentLocked = true;
	gui->isParentLocked = true;


	renderWorld = new RenderWorld();
	renderLevel = new RenderLevel();
}

block::Datamodel::~Datamodel()
{

}
