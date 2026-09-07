#pragma once

#include <G3DAll.h>

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

	enum Face
	{
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		FRONT,
		BACK
	};

	class BlockInstance;

	class Block /* block's container for 3d geometry */
	{
	public:
		Array<uint32_t> part_indices;
		bool dirty;

		void buildBlockFace(SurfaceType surface, Face face, const Vector3& size, const CoordinateFrame& cframe);
		void translateBlock(const CoordinateFrame& cframe);
	};

	class RenderLevel
	{
	protected:
		Array<Block*> renderBlocks;
	public:

		void notifyBlockChangeColor(int blockIndex, const Color3& newColor);
		void notifyBlockTranslate(int blockIndex, const CoordinateFrame& newCframe);
		int createBlockFromBlockInstance(BlockInstance* blockInstance);
		void removeBlock(int blockIndex);

		RenderLevel();
		~RenderLevel();
	};
}