#pragma once

#include <vector>
#include <string>

#include <G3DAll.h>

namespace block
{
	class Instance;

	typedef ReferenceCountedPointer<Instance> InstanceRef;
	typedef std::vector<InstanceRef> Instances;

	template <typename Class>
	Class* toClassType(Instance* instance)
	{
		return dynamic_cast<Class*>(instance);
	}

	template <typename Class>
	bool isInstanceClassType(Instance* instance)
	{
		return toClassType<Class>(instance) != nullptr;
	}

	class Instance : public ReferenceCountedObject
	{
	protected:
		friend class Datamodel;

		std::string name;
		std::string className;
		Instances children;
		Instance* parent;
		
		bool archivable;
		bool isParentLocked;

	public:

		template <typename T>
		T* findFirstAncestorWhichIsA(std::string className)
		{
			const Instance* v2; // eax

			v2 = parent;
			if (!parent)
				return 0;
			while (1)
			{
				v2 = v2->parent;
				if (v2 && v2->className == className)
					break;
				if (!v2)
					return 0;
			}
			return (T*)v2;
		}

		std::string getFullName()
		{
			Instance* parent = this->parent;
			std::string fullName;

			if (parent)
			{
				if (parent->parent)
				{
					fullName = parent->getFullName();
					fullName += ".";
					fullName += name;
				}
				else
				{
					fullName = name;
				}
				return fullName;
			}
			else
			{
				return name;
			}
		}

		template <typename T>
		inline T* findFirstChild(std::string name)
		{
			return (T*)findFirstChild(name);
		}

		template<typename T>
		inline T* findFirstChildOfClass(std::string name)
		{
			Instance* child;
			for (size_t i = 0; i < getChildren().size(); i++)
			{
				child = getChildren()->at(i);
				if (child && child->getClassName() == name)
					return toClassType<T*>(child);
			}
			return 0;
		}


		void clearAllChildren();

		void setName(std::string newName) { name = newName; }
		std::string getName() { return name; }
		void setClassName(std::string newClassName) { className = newClassName; }
		std::string getClassName() { return className; }
		bool getArchivable() { return archivable; }
		void setArchivable(bool narchivable) { archivable = narchivable; }
		void remove();
		bool contains(const Instance* child);


		virtual void onDescendentAdded(Instance* parent, Instance* descendent) {}
		virtual void onDescendentRemoved(Instance* parent, Instance* descendent) {}

		Instances getChildren() { return children; }

		void setParent(Instance* instance);
		Instance* getParent();
		Instance* findFirstChild(std::string name);

		static void signalOnDescendentAdded(Instance* instance, Instance* newParent, Instance* oldParent);
		static void signalOnDescendentRemoved(Instance* instance, Instance* beginParent, Instance* oldParent);
		static bool isAncestorOf(Instance* anc, Instance* des);
		
		Instance();
		~Instance();
	};


	bool isA(Instance* instance, const char* className)
	{
		return (className == instance->getClassName());
	}

}