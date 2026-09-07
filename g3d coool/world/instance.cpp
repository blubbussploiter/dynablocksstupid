#include "instance.h"

block::Instance* block::Instance::getParent()
{
	return parent;
}

void block::Instance::setParent(Instance* instance)
{
	Instance* oldParent;
	oldParent = parent;

	if (isParentLocked)
		return;

	if (instance == this)
	{
		MessageBox(0, "Cannot set object as its own parent", 0, 0);
		return;
	}

	if (oldParent != instance) {

		if (oldParent)
		{
			if (oldParent->contains(this))
			{
				Instances children = oldParent->getChildren();
				for (size_t i = 0; i < children.size(); i++)
				{
					if (children[i].getPointer() == this)
					{
						oldParent->children.erase(oldParent->children.begin() + i);
					}
				}
				signalOnDescendentRemoved(this, oldParent, instance);
			}
		}

		parent = instance;

		if (parent)
		{
			parent->children.push_back(this);
			signalOnDescendentAdded(this, instance, oldParent);
		}

	}

}

block::Instance* block::Instance::findFirstChild(std::string name)
{
	InstanceRef child;
	for (size_t i = 0; i < children.size(); i++)
	{
		child = children.at(i);
		if (child.notNull())
		{
			if (child->getName() == name)
			{
				return child.getPointer();
			}
		}
	}
	return 0;
}

bool block::Instance::isAncestorOf(Instance* ancestor, Instance* descendent)
{
	const Instance* v2; // eax

	v2 = descendent;
	if (!descendent)
		return 0;
	while (1)
	{
		v2 = v2->parent;
		if (v2 == ancestor)
			break;
		if (!v2)
			return 0;
	}
	return 1;
}

void block::Instance::clearAllChildren()
{
	for (size_t i = 0; i < children.size(); i++)
	{
		InstanceRef child = children.at(i);
		if (child.notNull())
		{
			child->remove();
		}
	}
}

void block::Instance::remove()
{
	if (!isParentLocked)
	{
		//InstanceGarbageCollector::get()->addInstance(this);

		for (InstanceRef i : children)
		{
			if (i.notNull())
			{
				i->remove();
			}
		}

		setParent(0);
		//delete this;
	}
}

bool block::Instance::contains(const Instance* child)
{
	const Instance* v2; // eax

	v2 = child;
	if (!child)
		return 0;
	while (v2 != this)
	{
		v2 = v2->parent;
		if (!v2)
			return 0;
	}
	return 1;
}

void block::Instance::signalOnDescendentAdded(Instance* instance, Instance* beginParent, Instance* oldParent)
{
	for (Instance* i = beginParent; i; i = i->parent)
	{
		if (i == oldParent || oldParent && (oldParent->parent == i || isAncestorOf(i, oldParent->parent)))
			break;
		i->onDescendentAdded(i, instance);
	}

	Instances children = instance->getChildren();
	for (size_t i = 0; i < children.size(); i++)
	{
		InstanceRef in = children.at(i);
		in->signalOnDescendentAdded(in.getPointer(), beginParent, oldParent);
	}
}

void block::Instance::signalOnDescendentRemoved(Instance* instance, Instance* beginParent, Instance* newParent)
{
	for (Instance* i = beginParent; i; i = i->parent)
	{
		if (i == newParent || newParent && (newParent->parent == i || isAncestorOf(i, newParent->parent)))
			break;
		i->onDescendentRemoved(i, instance);
	}

	Instances children = instance->getChildren();
	for (size_t i = 0; i < children.size(); i++)
	{
		InstanceRef in = children.at(i);
		in->signalOnDescendentRemoved(in.getPointer(), beginParent, newParent);
	}
}

block::Instance::Instance()
{
	className = "Instance";
	name = "Instance";
	archivable = true;
	isParentLocked = false;
	parent = 0;
}

block::Instance::~Instance()
{
	/* not much to really delete */
}