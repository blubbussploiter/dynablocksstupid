#include "renderLevel.h"

#include "../object/partInstance.h"
#include "../applet.h"

#include "icosphere.hpp"
#include "cylinder.hpp"

void block::RenderLevel::Level::renderGeometry(RenderDevice* renderDevice)
{
	if (indexArray.size() > 0)
	{
		renderDevice->sendIndices(RenderDevice::TRIANGLES, indexArray);
	}
}

void block::RenderLevel::notifyBlockChangeColor(int blockIndex, const Color4& newColor)
{
	if (renderBlocks.size() > blockIndex)
	{
		Block* block = renderBlocks[blockIndex];
		if (block)
		{
			for (int i = 0; i < block->part_indices.size(); i++)
			{
				__render_world->updateVerticeColor(block->part_indices[i], newColor);
			}
		}
	}
}

void block::RenderLevel::changeLevel(int blockIndex, RenderLevel::Level* to)
{
	if (renderBlocks.size() > blockIndex)
	{
		Block* block = renderBlocks[blockIndex];
		if (block->level != to)
		{
			for (int i = 0; i < block->part_indices.size(); i++)
			{
				block->level->removeIndex(block->part_indices[i]);
				to->addIndex(block->part_indices[i]);
			}
			block->level = to;
		}

	}
}

void block::RenderLevel::removeBlock(int blockIndex)
{
	if (renderBlocks.size() > blockIndex) 
	{
		Block* block = renderBlocks[blockIndex];
		for (int i = 0; i < block->part_indices.size(); i++)
		{
			block->level->removeIndex(block->part_indices[i]);
			__render_world->removeVertice(block->part_indices[i]);
		}
		for (int i = 0; i < block->cylinder_indices.size(); i++)
		{
			block->level->removeIndex(block->cylinder_indices[i]);
			__render_world->removeVertice(block->cylinder_indices[i]);
		}
		for (int i = 0; i < block->surface_indices.size(); i++)
		{
			surfaceLevel->removeIndex(block->surface_indices[i]);
			__render_world->removeVertice(block->surface_indices[i]);
		}
		block->cylinder_indices.clear();
		block->part_indices.clear();
	}
}

void block::RenderLevel::notifyBlockTranslate(int blockIndex, const CoordinateFrame& newCframe)
{
	if (renderBlocks.size() > blockIndex)
	{
		Block* block = renderBlocks[blockIndex];
		block->translateBlock(newCframe);
	}
}

int block::RenderLevel::createBlockFromBlockInstance(PartInstance* blockInstance)
{
	if (blockInstance->_block == -1)
	{
		Block* newBlock = new Block();
		int blockIndex = renderBlocks.size();

		/* project might be fucked 
			blocks not rendering sometimes... 9/11 4:08 am
			very odd
			put a print statement and then it works??
			what???
			the msvc compiler is crazy
		*/

		renderBlocks.append(newBlock);

		/* change with transparency */
		if (blockInstance->transparency > 0)
		{
			newBlock->level = transparentLevel;
		}
		else
		{
			newBlock->level = opaqueLevel;
		}

		switch (blockInstance->blockType)
		{
			case BlockType::NORMAL_BLOCK:
			{
				newBlock->buildBlockFace(blockInstance->top, TOP, blockInstance->size, blockInstance->cframe);
				newBlock->buildBlockFace(blockInstance->bottom, BOTTOM, blockInstance->size, blockInstance->cframe);
				newBlock->buildBlockFace(blockInstance->front, FRONT, blockInstance->size, blockInstance->cframe);
				newBlock->buildBlockFace(blockInstance->back, BACK, blockInstance->size, blockInstance->cframe);
				newBlock->buildBlockFace(blockInstance->right, RIGHT, blockInstance->size, blockInstance->cframe);
				newBlock->buildBlockFace(blockInstance->left, LEFT, blockInstance->size, blockInstance->cframe);
				break;
			}
			case BlockType::BALL_BLOCK:
			{
				build_ball(newBlock, blockInstance->size);
				break;
			}
			case BlockType::CYLINDER_BLOCK:
			{
				CoordinateFrame origin;
				float height = blockInstance->size.y / 4;
				float radius = blockInstance->size.x / 4;
				build_cylinder(newBlock, blockInstance->cframe, blockInstance->color, height, radius, 10);
				build_pluses(newBlock, blockInstance->cframe, height);
				break;
			}
		}
		/* fix bumps for cylinders soon plz */
		newBlock->build3DSurface(blockInstance->top, TOP, blockInstance->controllerType, blockInstance->size, blockInstance->cframe);
		newBlock->build3DSurface(blockInstance->bottom, BOTTOM, blockInstance->controllerType, blockInstance->size, blockInstance->cframe);
		newBlock->build3DSurface(blockInstance->front, FRONT, blockInstance->controllerType, blockInstance->size, blockInstance->cframe);
		newBlock->build3DSurface(blockInstance->back, BACK, blockInstance->controllerType, blockInstance->size, blockInstance->cframe);
		newBlock->build3DSurface(blockInstance->right, RIGHT, blockInstance->controllerType, blockInstance->size, blockInstance->cframe);
		newBlock->build3DSurface(blockInstance->left, LEFT, blockInstance->controllerType, blockInstance->size, blockInstance->cframe);
		if (blockInstance->blockType != block::CYLINDER_BLOCK)
		{
			newBlock->translateBlock(blockInstance->cframe);
		}
		else { /* why translate here? because we translate in the build_cylinder call, this should probably change in the future to clean this up a bit, but it works for giving that
			`roblox` look to it */
			newBlock->translateSurfaces(blockInstance->cframe);
			newBlock->center = blockInstance->cframe;
		}
		notifyBlockChangeColor(blockIndex, blockInstance->color);

		return blockIndex;
	}
	return -1;
}

block::RenderLevel::RenderLevel()
{
	lighting = Lighting::create();
	lightingParameters = LightingParameters();
	lighting->ambientTop = Color3(0.81960785f, 0.81568629f, 0.8509804f);
	lighting->ambientBottom = Color3(0.47843137f, 0.52549022f, 0.47058824f);
	lightingParameters.lightColor = Color3(0.59607846f, 0.53725493f, 0.40000001f);
	lightingParameters.setTime(toSeconds(12, 0, PM));
	opaqueLevel = new Level(RenderDevice::TRIANGLES);
	transparentLevel = new Level(RenderDevice::TRIANGLES);
	reflectLevel = new Level(RenderDevice::TRIANGLES);
	surfaceLevel = new Level(RenderDevice::TRIANGLES);
}

block::RenderLevel::~RenderLevel()
{
	for (int i = 0; i < renderBlocks.size(); i++)
	{
		delete renderBlocks[i];
	}
	delete opaqueLevel;
	delete surfaceLevel;
	renderBlocks.clear();
}

void block::RenderLevel::Level::addIndex(uint32 index)
{
	int i = indexArray.findIndex(index);
	if (i == -1)
	{
		indexArray.append(index);
	}
}

void block::RenderLevel::Level::removeIndex(uint32 index)
{
	int i = indexArray.findIndex(index);
	if (i != -1)
	{
		indexArray.remove(i);
	}
}
