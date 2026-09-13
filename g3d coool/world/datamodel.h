#pragma once

#include "instance.h"
#include "workspace.h"
#include "runservice.h"
#include "ui/guiRoot.h"
#include "physics/kernel.h"
#include "render/renderWorld.h"
#include "render/renderLevel.h"

#define __render_world block::Datamodel::get()->renderWorld
#define __render_level block::Datamodel::get()->renderLevel

class Applet;

namespace block
{
	class Datamodel : public Instance
	{
	protected:
		friend class WorkspaceInstance;
		friend class PartInstance;
		friend class RenderLevel;
		friend class Applet;

		WorkspaceInstance* workspace;
		GuiRoot* gui;

	public:

		RenderWorld* renderWorld;
		RenderLevel* renderLevel;
		RunService* runService;

		static Datamodel* get();

		Datamodel();
		~Datamodel();
	};
}