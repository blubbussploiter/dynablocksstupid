#pragma once

#include <G3DAll.h>
#include "renderLevel.h"
#include "../applet.h"

static void add_block_vertex(block::Block* block, const Vector3& v, const Vector3& n)
{
	uint32_t i = __render_world->addVertice(v, n, Color4::ZERO, Vector2::zero());
	if (block->level)
	{
		block->level->addIndex(i);
	}
	block->part_indices.append(i);
}

static void add_any_triangle(block::RenderLevel::Level* level, Array<uint32_t>& indices, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Color4& color = Color4::ZERO)
{
	uint32_t i1 = __render_world->addVertice(v1,
		cross(v2 - v1, v3 - v1).direction(),
		color, Vector2::zero());
	uint32_t i2 = __render_world->addVertice(v2,
		cross(v3 - v2, v1 - v2).direction(),
		color, Vector2::zero());
	uint32_t i3 = __render_world->addVertice(v3,
		cross(v1 - v3, v2 - v3).direction(),
		color, Vector2::zero());
	if (level)
	{
		level->addIndex(i1);
		level->addIndex(i2);
		level->addIndex(i3);
	}
	indices.append(i1);
	indices.append(i2);
	indices.append(i3);
}

static void add_block_triangle(block::Block* block, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Color4& color=Color4::ZERO)
{
	add_any_triangle(block->level, block->part_indices, v1, v2, v3, color);
}

static void add_surface_triangle(block::Block* block, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Color4& color)
{
	add_any_triangle(__render_level->surfaceLevel, block->surface_indices, v1, v2, v3, color);
}

static void add_triangle_relative(block::Block* block, const CoordinateFrame& relativeTo, Vector3 v1, Vector3 v2, Vector3 v3)
{
	v1 = relativeTo.pointToWorldSpace(v1);
	v2 = relativeTo.pointToWorldSpace(v1);
	v3 = relativeTo.pointToWorldSpace(v3);
	add_block_triangle(block, v1, v2, v3);
}

static void add_quad(block::Block* block, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, const Color4& color = Color4::ZERO)
{
	add_block_triangle(block, v1, v2, v3, color);
	add_block_triangle(block, v1, v3, v4, color);
}

static void add_quad_relative(block::Block* block, const CoordinateFrame& relativeTo, Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4, const Color4& color=Color4::ZERO)
{
	v1 = relativeTo.pointToWorldSpace(v1);
	v2 = relativeTo.pointToWorldSpace(v2);
	v3 = relativeTo.pointToWorldSpace(v3);
	v4 = relativeTo.pointToWorldSpace(v4);
	add_block_triangle(block, v1, v2, v3, color);
	add_block_triangle(block, v1, v3, v4, color);
}

static void build_simple_face(block::Block* self, block::NormalId face, const Vector3& size, const CoordinateFrame& offset)
{
	switch (face)
	{
	case block::TOP:
	{
		add_quad_relative(self,
			offset,
			Vector3(size.x, size.y, -size.z),
			Vector3(-size.x, size.y, -size.z),
			Vector3(-size.x, size.y, size.z),
			Vector3(size.x, size.y, size.z));
		break;
	}
	case block::RIGHT: 
	{
		add_quad_relative(self,
			offset,
			Vector3(-size.x, size.y, size.z),
			Vector3(-size.x, size.y, -size.z),
			Vector3(-size.x, -size.y, -size.z),
			Vector3(-size.x, -size.y, size.z));
		break;
	}
	case block::LEFT:
	{
		add_quad_relative(self,
			offset,
			Vector3(size.x, size.y, -size.z),
			Vector3(size.x, size.y, size.z),
			Vector3(size.x, -size.y, size.z),
			Vector3(size.x, -size.y, -size.z));
		break;
	}
	case block::FRONT:
	{
		add_quad_relative(self,
			offset,
			Vector3(size.x, -size.y, -size.z),
			Vector3(-size.x, -size.y, -size.z),
			Vector3(-size.x, size.y, -size.z),
			Vector3(size.x, size.y, -size.z));
		break;
	}
	case block::BACK:
	{
		add_quad_relative(self,
			offset,
			Vector3(-size.x, -size.y, size.z),
			Vector3(size.x, -size.y, size.z),
			Vector3(size.x, size.y, size.z),
			Vector3(-size.x, size.y, size.z));
		break;
	}
	}
}