#pragma once

/* thanks: https://www.songho.ca/opengl/gl_cylinder.html */

#include <G3DAll.h>
#include "renderLevel.h"
#include "../applet.h"

static void build_surface_circle(block::Block* self, const CoordinateFrame& where, int triangles, float radius, float y, bool invertFace, const Color4& color)
{
	Array<Vector3> circleVertices;

	/* allocate circle */

	for (int i = 0; i < triangles; i++) {
		float angle = (360.0f / triangles) * i;
		float x = radius * cos(toRadians(angle));
		float z = radius * sin(toRadians(angle));
		circleVertices.push_back(Vector3(x, y, z));
	}

	/* add circle */

	for (int i = 0; i < triangles - 2; i++) {

		Vector3 v0 = where.pointToWorldSpace(circleVertices[0]);
		Vector3 v1 = where.pointToWorldSpace(circleVertices[i + 1 + invertFace]);
		Vector3 v2 = where.pointToWorldSpace(circleVertices[i + 2 - invertFace]);

		add_surface_triangle(self, v0, v1, v2, color);
	}
}

static void build_surface_cylinder(block::Block* self, const CoordinateFrame& where, const Color4& color, float height, float radius, int slices)
{
	float y = where.translation.y;

	for (int i = 0; i < slices; i++)
	{
		float theta = float(i) * 2 * pi() / slices;
		float nextTheta = float(i + 1) * 2 * pi() / slices;

		float x1 = cos(theta);
		float z1 = sin(theta);
		float x2 = cos(nextTheta);
		float z2 = sin(nextTheta);

		Vector3 v0 = where.pointToWorldSpace(Vector3(radius * x1, height, radius * z1));
		Vector3 v1 = where.pointToWorldSpace(Vector3(radius * x2, height, radius * z2));
		Vector3 v2 = where.pointToWorldSpace(Vector3(radius * x2, -height, radius * z2));
		Vector3 v3 = where.pointToWorldSpace(Vector3(radius * x1, -height, radius * z1));

		add_surface_triangle(self, v0, v1, v2, color);
		add_surface_triangle(self, v2, v3, v0, color);

	}
	build_surface_circle(self, where, slices, radius, height, 1, color);
	build_surface_circle(self, where, slices, radius, -height, 0, color);
}

static void add_plusss_quad_relative(block::Block* block, const CoordinateFrame& relativeTo, Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4, const Color4& color = Color4::ZERO)
{
	v1 = relativeTo.pointToWorldSpace(v1);
	v2 = relativeTo.pointToWorldSpace(v2);
	v3 = relativeTo.pointToWorldSpace(v3);
	v4 = relativeTo.pointToWorldSpace(v4);
	add_any_triangle(block->level, block->cylinder_indices, v1, v2, v3, color);
	add_any_triangle(block->level, block->cylinder_indices, v1, v3, v4, color);
}

static void build_plus(block::Block* self, const CoordinateFrame& where, float height, int face=-1)
{
	float y = (face * (height + 0.0001f));

	/* vertical */
	add_plusss_quad_relative(self, where,
		Vector3(0.05f, y, face * (-height * 0.75f)),
		Vector3(-0.05f,y, face * (-height * 0.75f)),
		Vector3(-0.05f,y, face * (height * 0.75f)),
		Vector3(0.05f, y, face * (height * 0.75f)), Color3::white());

	/* horizontal */
	add_plusss_quad_relative(self, where,
		Vector3(height * 0.75f, y, face * (-0.05f)),
		Vector3(height * 0.05f, y, face * (-0.05f)),
		Vector3(height * 0.05f, y, face * (0.05f)),
		Vector3(height * 0.75f, y, face * (0.05f)), Color3::white());

	add_plusss_quad_relative(self, where,
		Vector3(-height * 0.75f, y, face * (0.05f)),
		Vector3(height * 0.05f, y, face * (0.05f)),
		Vector3(height * 0.05f, y, face * (-0.05f)),
		Vector3(-height * 0.75f, y, face * (-0.05f)), Color3::white());

}

static void build_pluses(block::Block* self, const CoordinateFrame& where, float height)
{
	build_plus(self, where, height);
	build_plus(self, where, height, 1);
}

static void build_circle(block::Block* self, const CoordinateFrame& where, int triangles, float radius, float y, bool invertFace, const Color4& color)
{
	Array<Vector3> circleVertices;

	/* allocate circle */

	for (int i = 0; i < triangles; i++) {
		float angle = (360.0f / triangles) * i;
		float x = radius * cos(toRadians(angle));
		float z = radius * sin(toRadians(angle));
		circleVertices.push_back(Vector3(x, y, z));
	}

	/* add circle */

	for (int i = 0; i < triangles - 2; i++) {

		Vector3 v0 = where.pointToWorldSpace(circleVertices[0]);
		Vector3 v1 = where.pointToWorldSpace(circleVertices[i + 1 + invertFace]);
		Vector3 v2 = where.pointToWorldSpace(circleVertices[i + 2 - invertFace]);

		add_block_triangle(self, v0, v1, v2, color);
	}
}

static void build_cylinder(block::Block* self, const CoordinateFrame& where, const Color4& color, float height, float radius, int slices)
{

	/* roblox has the cylinder faces split into 3 quads */
	/* so just emulate that */

	float h = height / 2;

	for (int o = 0; o < 4; o++)
	{
		float p = -height + (o * h);
		for (int i = 0; i < slices; i++)
		{
			float theta = float(i) * 2 * pi() / slices;
			float nextTheta = float(i + 1) * 2 * pi() / slices;

			float x1 = cos(theta);
			float z1 = sin(theta);
			float x2 = cos(nextTheta);
			float z2 = sin(nextTheta);

			Vector3 v0 = where.pointToWorldSpace(Vector3(radius * x1, p + h, radius * z1));
			Vector3 v1 = where.pointToWorldSpace(Vector3(radius * x2, p + h, radius * z2));
			Vector3 v2 = where.pointToWorldSpace(Vector3(radius * x2, p, radius * z2));
			Vector3 v3 = where.pointToWorldSpace(Vector3(radius * x1, p, radius * z1));

			add_block_vertex(self, v0, v0);
			add_block_vertex(self, v1, v1);
			add_block_vertex(self, v2, v2);

			add_block_vertex(self, v2, v2);
			add_block_vertex(self, v3, v3);
			add_block_vertex(self, v0, v0);
		}
	}

	build_circle(self, where, slices, radius, height, 1, color);
	build_circle(self, where, slices, radius, -height, 0, color);
}