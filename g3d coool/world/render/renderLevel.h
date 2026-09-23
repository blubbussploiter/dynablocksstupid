#pragma once

#include <G3DAll.h>
#include "../ui/control.h"

namespace block
{

	enum BlockType
	{
		BALL_BLOCK,
		NORMAL_BLOCK,
		CYLINDER_BLOCK
	};

	enum SurfaceType
	{
		NO_SURFACE,
		BUMP,
		MOTOR,
		HINGE
	};

	enum NormalId
	{
		RIGHT,
		TOP,
		BACK,
		LEFT,
		BOTTOM,
		FRONT,
		UNDEFINED
	};

	class PartInstance;
	class Block;

	class RenderLevel
	{
	protected:
		Array<Block*> renderBlocks;

		LightingRef lighting;
		LightingParameters lightingParameters;

	public:

		class Level
		{
		public:
			Array<uint32> indexArray;
			RenderDevice::Primitive primitive;

			void addIndex(uint32 index);
			void removeIndex(uint32 index);

			void renderGeometry(RenderDevice* renderDevice);

			Level()
			{
				primitive = RenderDevice::QUADS;
				indexArray = Array<uint32>();
			}

			// I could do : primitive(primitive), but constructor would look too blank
			Level(RenderDevice::Primitive primitive)
			{
				this->primitive = primitive;
				indexArray = Array<uint32>();
			}
		};

		RenderLevel::Level* opaqueLevel;
		RenderLevel::Level* reflectLevel;
		RenderLevel::Level* transparentLevel;
		RenderLevel::Level* surfaceLevel;

		void notifyBlockChangeColor(int blockIndex, const Color4& newColor);
		void notifyBlockTranslate(int blockIndex, const CoordinateFrame& newCframe);
		int createBlockFromBlockInstance(PartInstance* blockInstance);

		void changeLevel(int blockIndex, RenderLevel::Level* to);
		void removeBlock(int blockIndex);

		static RenderLevel* get();

		LightingRef getLighting() { return lighting; }
		LightingParameters getLightingParameters() { return lightingParameters; }

		size_t numBlocks() { return renderBlocks.size(); }

		RenderLevel();
		~RenderLevel();
	};

	class Block /* block's container for 3d geometry */
	{
	public:
		Array<uint32_t> part_indices;
		Array<uint32_t> cylinder_indices;
		Array<uint32_t> surface_indices;
		RenderLevel::Level* level;
		CoordinateFrame center;

		void buildBlockFace(SurfaceType surface, NormalId face, const Vector3& size);
		void build3DSurface(SurfaceType surface, NormalId face, Controller::ControllerType controller, const Vector3& size, const CoordinateFrame& cframe);
		void translateBlock(const CoordinateFrame& cframe);
		void translateSurfaces(const CoordinateFrame& cframe);
		Block();
	};

}