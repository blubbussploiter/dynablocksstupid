#include "renderLevel.h"

#include "../object/partInstance.h"
#include "../applet.h"

void block::RenderLevel::notifyBlockChangeColor(int blockIndex, const Color3& newColor)
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

void block::RenderLevel::removeBlock(int blockIndex)
{
	if (renderBlocks.size() > blockIndex)
	{
		Block* block = renderBlocks[blockIndex];
		for (int i = 0; i < block->part_indices.size(); i++)
		{
			__render_world->removeVertice(block->part_indices[i]);
		}
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

int block::RenderLevel::createBlockFromBlockInstance(BlockInstance* blockInstance)
{
	if (blockInstance->_block == -1)
	{
		Block* newBlock = new Block();
		int blockIndex = renderBlocks.size();

		renderBlocks.append(newBlock);

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
				newBlock->translateBlock(blockInstance->cframe);
				notifyBlockChangeColor(blockIndex, blockInstance->color);
				break;
			}
		}

		return blockIndex;
	}
	return -1;
}

block::RenderLevel::RenderLevel()
{

}

block::RenderLevel::~RenderLevel()
{
	for (int i = 0; i < renderBlocks.size(); i++)
	{
		delete renderBlocks[i];
	}
	renderBlocks.clear();
}