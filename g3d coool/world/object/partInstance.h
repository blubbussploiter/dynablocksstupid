#pragma once

#include "pvinstance.h"
#include "../render/renderLevel.h"

namespace block
{

	class BlockInstance : public PVInstance
	{
	protected:
		friend class RenderLevel;
		friend class WorkspaceInstance;

		CoordinateFrame cframe;
		BlockType blockType;
		float friction;
		float elasticity;
		Vector3 size;
		Color3 color;
		int _block;

		SurfaceType top;
		SurfaceType bottom;
		SurfaceType right;
		SurfaceType left;
		SurfaceType front;
		SurfaceType back;

	public:

		void setCoordinateFrame(const CoordinateFrame& newCFrame);
		CoordinateFrame getCoordinateFrame() { return cframe; }
		void setPosition(const Vector3& position) { setCoordinateFrame(CoordinateFrame(cframe.rotation, position)); }
		Vector3 getPosition() { return cframe.translation; }
		void setSize(const Vector3& newSize);
		Vector3 getSize() { return size; }
		void setColor(const Color3& newColor);
		Color3 getColor() { return color;  }
		void setTopSurface(SurfaceType surface) {
			top = surface;
			notifyUpdateSurface();
		}
		SurfaceType getTopSurface() { return top; }
		void setBottomSurface(SurfaceType surface) {
			bottom = surface;
			notifyUpdateSurface();
		}
		SurfaceType getBottomSurface() { return bottom; }
		void setFrontSurface(SurfaceType surface) {
			front = surface;
			notifyUpdateSurface();
		}
		SurfaceType getFrontSurface() { return front; }
		void setBackSurface(SurfaceType surface) {
			back = surface;
			notifyUpdateSurface();
		}
		SurfaceType getBackSurface() { return back; }
		void setRightSurface(SurfaceType surface) { 
			right = surface; 
			notifyUpdateSurface();
		}
		SurfaceType getRightSurface() { return right; }
		void setLeftSurface(SurfaceType surface) { 
			left = surface; 
			notifyUpdateSurface();
		}
		SurfaceType getLeftSurface() { return left; }
		void notifyUpdateSurface();

		void doRender(RenderDevice* renderDevice);

		BlockInstance();
		~BlockInstance();
	};
}