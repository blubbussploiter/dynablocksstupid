#include "../applet.h"
#include "datamodel.h"

block::Datamodel* block::Datamodel::get()
{
	return globalApplet->Datamodel();
}

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
	runService = new RunService();
}

block::Datamodel::~Datamodel()
{

}
