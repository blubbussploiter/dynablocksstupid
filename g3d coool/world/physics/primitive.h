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
			float elasticity;
			float friction;
		public:
			dGeomID geom[1];
			CoordinateFrame getPosition();
			void setPosition(const CoordinateFrame& position);
			void setSize(const Vector3& size);
			void modifyOffsetWorldCoordinateFrame(CoordinateFrame offset);
			bool attachedTo(Body* body) { return this->body == body; }
			void attach(Body* body);
			void detach();
			Primitive(const Vector3& size, const CoordinateFrame& position, Geometry geometry);
			Primitive();
			~Primitive();
		};
	}
}