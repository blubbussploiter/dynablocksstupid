#pragma once

#include "kernel.h"

namespace block
{
	namespace Physics
	{
		class Body;
		class Kernel;

		class Primitive
		{
		protected:
			friend class Kernel;
			friend class Body;
			Geometry geometry;
			Body* body;
			Vector3 size;
			float elasticity;
			float friction;
		public:
			dGeomID geom[1];
			CoordinateFrame getPosition();
			void setPosition(const CoordinateFrame& position);
			void setSize(const Vector3& size);
			Vector3 getSize();
			void modifyOffsetWorldCoordinateFrame(CoordinateFrame offset);
			bool attachedTo(Body* body) { return this->body == body; }
			void attach(Body* body);
			void detach();
			Box getBox();
			AABox getAABox();
			Primitive(const Vector3& size, const CoordinateFrame& position, Geometry geometry);
			Primitive();
			~Primitive();
		};
	}
}