#pragma once

#include "instance.h"
#include "workspace.h"
#include "ui/guiRoot.h"
#include "render/renderWorld.h"
#include "render/renderLevel.h"

#define __render_world globalApplet->Datamodel()->renderWorld
#define __render_level globalApplet->Datamodel()->renderLevel

class Applet;

namespace block
{
	class Datamodel : public Instance
	{
	protected:
		friend class WorkspaceInstance;
		friend class BlockInstance;
		friend class RenderLevel;
		friend class Applet;

		WorkspaceInstance* workspace;
		GuiRoot* gui;

	public:

		RenderWorld* renderWorld;
		RenderLevel* renderLevel;

		Datamodel();
		~Datamodel();
	};
}