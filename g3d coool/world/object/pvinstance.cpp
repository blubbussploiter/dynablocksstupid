#include "pvinstance.h"
#include "../applet.h"

block::PVInstance::PVInstance(const char* name)
{
	this->name = name;
	className = "PVInstance";
	nameShown = true;
	controllerFlagShown = true;
}