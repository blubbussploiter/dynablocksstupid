#pragma once

#include "pvinstance.h"
#include "../physics/body.h"
#include "../physics/primitive.h"
#include "../render/renderLevel.h"

/* huge thanks to Taragaeot (@renthes) and hargonservices (@confirmables) from CS for telling of cards */

namespace block
{

	class PartInstance : public PVInstance
	{
	protected:
		friend class RenderLevel;
		friend class WorkspaceInstance;
		Physics::Body* body;
		Physics::Primitive* primitive;
		Controller::ControllerType controllerType;
		CoordinateFrame cframe;
		BlockType blockType;
		float friction;
		float elasticity;
		float transparency;
		float reflectancy;
		bool anchored;
		bool canCollide;
		Vector3 rotVelocity;
		Vector3 linVelocity;
		Vector3 size;
		Color4 color;
		int _block;
		SurfaceType top;
		SurfaceType bottom;
		SurfaceType right;
		SurfaceType left;
		SurfaceType front;
		SurfaceType back;
		/* impl later */
		bool hasCard;
		bool hasOnTouch; 
	public:

		void setCoordinateFrame(const CoordinateFrame& newCFrame);
		CoordinateFrame getCoordinateFrame() { return cframe; }
		void setPosition(const Vector3& position) { setCoordinateFrame(CoordinateFrame(cframe.rotation, position)); }
		Vector3 getPosition() { return cframe.translation; }
		void setSize(const Vector3& newSize);
		Vector3 getSize() { return size; }
		void setColor(const Color3& newColor);
		void setColor4(const Color4& newColor);
		Color3 getColor() { return Color3(color);  }
		void setTopSurface(SurfaceType surface) {
			if (top != surface)
			{
				top = surface;
				notifyGeometryUpdate();
			}
		}
		SurfaceType getTopSurface() { return top; }
		void setBottomSurface(SurfaceType surface) {
			if (bottom != surface)
			{
				bottom = surface;
				notifyGeometryUpdate();
			}
		}
		SurfaceType getBottomSurface() { return bottom; }
		void setFrontSurface(SurfaceType surface) {
			if (front != surface)
			{
				front = surface;
				notifyGeometryUpdate();
			}
		}
		SurfaceType getFrontSurface() { return front; }
		void setBackSurface(SurfaceType surface) {
			if (back != surface)
			{
				back = surface;
				notifyGeometryUpdate();
			}
		}
		SurfaceType getBackSurface() { return back; }
		void setRightSurface(SurfaceType surface) { 
			if (right != surface)
			{
				right = surface;
				notifyGeometryUpdate();
			}
		}
		SurfaceType getRightSurface() { return right; }
		void setLeftSurface(SurfaceType surface) { 
			if (left != surface)
			{
				left = surface;
				notifyGeometryUpdate();
			}
		}
		SurfaceType getLeftSurface() { return left; }
		void notifyGeometryUpdate();
		void notifyLevelUpdate();

		BlockType getShape() { return blockType; }
		void setShape(BlockType shape) { 
			blockType = shape;
			if (shape == BALL_BLOCK)
			{
				size.x = max(size.x, max(size.y, size.z));
				size.y = size.x;
				size.z = size.x;
			}
			notifyGeometryUpdate();
		}
		void setControllerType(Controller::ControllerType type)
		{
			controllerType = type;
			notifyGeometryUpdate();
		}
		Controller::ControllerType getControllerType() { return controllerType; }
		void setTransparency(float newTrans) {
			transparency = newTrans;
			notifyLevelUpdate();
			setColor4(Color4(color.r, color.g, color.b, 1 - transparency));
		}
		bool getAnchored() { return anchored; }
		void setAnchored(bool anchored);
		void setVelocity(Vector3 velocity);
		void setRotVelocity(Vector3 velocity);
		Vector3 getVelocity();
		Vector3 getRotVelocity();
		bool getNameShown() { return nameShown; }
		void setNameShown(bool show) { nameShown = show; }
		Physics::Body* getBody();
		Physics::Primitive* getPrimitive() { return primitive; }
		Box getBox() { return Box(-size, size); }

		static Vector3 getNormalIdDirection(NormalId normal);
		static Vector3 getNormalFromId(const CoordinateFrame& cframe, NormalId normal);
		static NormalId getNormalFromVector(const CoordinateFrame& cframeWorld, const Vector3& direction);

		void doRender(RenderDevice* renderDevice);
		void onStep();

		PartInstance();
		~PartInstance();
	};
}