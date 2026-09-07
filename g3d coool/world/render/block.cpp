#include "renderLevel.h"
#include "../applet.h"

#define bump_pad 0.3f
#define bump 0.15f
#define bevel 0.05f

static void add_triangle(block::Block* block, const Vector3& v1, const Vector3& v2, const Vector3& v3)
{
	block->part_indices.append(__render_world->addVertice(v1, cross(v2 - v1, v3 - v1).direction(), Color4::ZERO, Vector2::zero()));
	block->part_indices.append(__render_world->addVertice(v2, cross(v3 - v2, v1 - v2).direction(), Color4::ZERO, Vector2::zero()));
	block->part_indices.append(__render_world->addVertice(v3, cross(v1 - v3, v2 - v3).direction(), Color4::ZERO, Vector2::zero()));
}

static void add_quad(block::Block* block, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4)
{
	add_triangle(block, v1, v2, v3);
	add_triangle(block, v1, v3, v4);
}

static void build_simple_face(block::Block* self, block::Face face, const Vector3& size, const Vector3& offset)
{
	switch (face)
	{
		case block::TOP:
		{
			add_quad(self,
				offset + Vector3(size.x, size.y, -size.z),
				offset + Vector3(-size.x, size.y, -size.z),
				offset + Vector3(-size.x, size.y, size.z),
				offset + Vector3(size.x, size.y, size.z));
			break;
		}
		case block::RIGHT:
		{
			add_quad(self,
				offset + Vector3(-size.x, size.y, size.z),
				offset + Vector3(-size.x, size.y, -size.z),
				offset + Vector3(-size.x, -size.y, -size.z),
				offset + Vector3(-size.x, -size.y, size.z));
			break;
		}
		case block::LEFT:
		{
			add_quad(self,
				offset + Vector3(size.x, size.y, -size.z),
				offset + Vector3(size.x, size.y, size.z),
				offset + Vector3(size.x, -size.y, size.z),
				offset + Vector3(size.x, -size.y, -size.z));
			break;
		}
		case block::FRONT:
		{
			add_quad(self,
				offset + Vector3(size.x, -size.y, -size.z),
				offset + Vector3(-size.x, -size.y, -size.z),
				offset + Vector3(-size.x, size.y, -size.z),
				offset + Vector3(size.x, size.y, -size.z));
			break;
		}
		case block::BACK:
		{
			add_quad(self,
				offset + Vector3(-size.x, -size.y, size.z),
				offset + Vector3(size.x, -size.y, size.z),
				offset + Vector3(size.x, size.y, size.z),
				offset + Vector3(-size.x, size.y, size.z));
			break;
		}
	}
}

static void build_simple_bump(block::Block* self, const Vector3& size, const Vector3& offset)
{
	build_simple_face(self, block::TOP, size, offset);
	build_simple_face(self, block::FRONT, size, offset);
	build_simple_face(self, block::BACK, size, offset);
	build_simple_face(self, block::RIGHT, size, offset);
	build_simple_face(self, block::LEFT, size, offset);
}

static void add_bumps(block::Block* self, const Vector3& size, block::SurfaceType surface, block::Face face)
{
	Vector3 origin;
	Vector3 axis_hors;
	Vector3 axis_vert;
	int x = 0;
	int y = 0;
	Vector3 realSize = size * 4;
	switch (face)
	{
		case block::TOP:
		case block::BOTTOM:
		{
			x = realSize.x;
			y = realSize.z;
			axis_hors = Vector3(1, 0, 0);
			axis_vert = Vector3(0, 0, 1);
			origin = Vector3(-size.x + bump, size.y, -size.z + bump);
			break;
		}
	}

	/* ezpz */
	for (int i = 0; i < x; i++)
	{
		printf("build\n");
		Vector3 hors = (axis_hors * (i * (bump_pad + bump)));
		printf("%d %f, %f, %f\n",i, hors.x, hors.y, hors.z);
		build_simple_bump(self, 
			Vector3(bump, 0.1f, bump), 
			origin + hors);
	}
}

void block::Block::buildBlockFace(SurfaceType surface, Face face, const Vector3& size, const CoordinateFrame& cframe)
{
	Vector3 realSize = size / 4;
	realSize.x -= bevel;
	realSize.y -= bevel;
	realSize.z -= bevel;

	switch (face)
	{
		case TOP:
		{
			/* main quad */
			add_quad(this, Vector3(realSize.x, realSize.y + bevel, -realSize.z),
						   Vector3(-realSize.x, realSize.y + bevel, -realSize.z),
						   Vector3(-realSize.x, realSize.y + bevel, realSize.z),
						   Vector3(realSize.x, realSize.y + bevel, realSize.z));
			/* top bevel right */
			add_quad(this, 
				Vector3(realSize.x + bevel, realSize.y, -realSize.z),
				Vector3(realSize.x, realSize.y + bevel, -realSize.z),
				Vector3(realSize.x, realSize.y + bevel, realSize.z),
				Vector3(realSize.x + bevel, realSize.y, realSize.z));
			/* top bevel left */
			add_quad(this,
				Vector3(-realSize.x - bevel, realSize.y, realSize.z),
				Vector3(-realSize.x, realSize.y + bevel, realSize.z),
				Vector3(-realSize.x, realSize.y + bevel, -realSize.z),
				Vector3(-realSize.x - bevel, realSize.y, -realSize.z));
			/* top bevel front */
			add_quad(this, 
				Vector3(-realSize.x, realSize.y + bevel, -realSize.z),
				Vector3(realSize.x, realSize.y + bevel, -realSize.z),
				Vector3(realSize.x, realSize.y, -realSize.z - bevel),
				Vector3(-realSize.x, realSize.y, -realSize.z - bevel));
			/* top bevel back */
			add_quad(this,
				Vector3(realSize.x, realSize.y + bevel, realSize.z),
				Vector3(-realSize.x, realSize.y + bevel, realSize.z),
				Vector3(-realSize.x, realSize.y, realSize.z + bevel),
				Vector3(realSize.x, realSize.y, realSize.z + bevel));
			break;
		}
		case BOTTOM:
		{
			/* main quad */
			add_quad(this, 
				Vector3(realSize.x, -realSize.y - bevel, realSize.z),
				Vector3(-realSize.x, -realSize.y - bevel, realSize.z),
				Vector3(-realSize.x, -realSize.y - bevel, -realSize.z),
				Vector3(realSize.x, -realSize.y - bevel, -realSize.z));
			/* top bevel right */
			add_quad(this,
				Vector3(realSize.x + bevel, -realSize.y, realSize.z),
				Vector3(realSize.x, -realSize.y - bevel, realSize.z),
				Vector3(realSize.x, -realSize.y - bevel, -realSize.z),
				Vector3(realSize.x + bevel, -realSize.y, -realSize.z));
			/* top bevel left */
			add_quad(this,
				Vector3(-realSize.x - bevel, -realSize.y, -realSize.z),
				Vector3(-realSize.x, -realSize.y - bevel, -realSize.z),
				Vector3(-realSize.x, -realSize.y - bevel, realSize.z),
				Vector3(-realSize.x - bevel, -realSize.y, realSize.z));
			/* top bevel front */
			add_quad(this,
				Vector3(realSize.x, -realSize.y - bevel, -realSize.z),
				Vector3(-realSize.x, -realSize.y - bevel, -realSize.z),
				Vector3(-realSize.x, -realSize.y, -realSize.z - bevel),
				Vector3(realSize.x, -realSize.y, -realSize.z - bevel));
			/* top bevel back */
			add_quad(this,
				Vector3(-realSize.x, -realSize.y - bevel, realSize.z),
				Vector3(realSize.x, -realSize.y - bevel, realSize.z),
				Vector3(realSize.x, -realSize.y, realSize.z + bevel),
				Vector3(-realSize.x, -realSize.y, realSize.z + bevel));
			break;
		}
		case FRONT:
		{
			add_quad(this,
				Vector3(realSize.x, -realSize.y, -realSize.z - bevel),
				Vector3(-realSize.x, -realSize.y, -realSize.z - bevel),
				Vector3(-realSize.x, realSize.y, -realSize.z - bevel),
				Vector3(realSize.x, realSize.y, -realSize.z - bevel));

			/* left bevel */

			add_quad(this,
				Vector3(realSize.x + bevel, -realSize.y, -realSize.z),
				Vector3(realSize.x, -realSize.y, -realSize.z - bevel),
				Vector3(realSize.x, realSize.y, -realSize.z - bevel),
				Vector3(realSize.x + bevel, realSize.y, -realSize.z));

			/* top triangle */
			
			add_triangle(this,
				Vector3(realSize.x + bevel, realSize.y, -realSize.z),
				Vector3(realSize.x, realSize.y, -realSize.z - bevel),
				Vector3(realSize.x, realSize.y + bevel, -realSize.z));

			/* bottom triangle */
			add_triangle(this,
				Vector3(realSize.x + bevel, -realSize.y, -realSize.z),
				Vector3(realSize.x, -realSize.y - bevel, -realSize.z),
				Vector3(realSize.x, -realSize.y, -realSize.z - bevel));

			/* right bevel */

			add_quad(this,
				Vector3(-realSize.x - bevel, realSize.y, -realSize.z),
				Vector3(-realSize.x, realSize.y, -realSize.z - bevel),
				Vector3(-realSize.x, -realSize.y, -realSize.z - bevel),
				Vector3(-realSize.x - bevel, -realSize.y, -realSize.z));

			/* top triangle */

			add_triangle(this,
				Vector3(-realSize.x - bevel, realSize.y, -realSize.z), 
				Vector3(-realSize.x, realSize.y + bevel, -realSize.z),
				Vector3(-realSize.x, realSize.y, -realSize.z - bevel));

			/* bottom triangle */

			add_triangle(this,
				Vector3(-realSize.x - bevel, -realSize.y, -realSize.z),
				Vector3(-realSize.x, -realSize.y, -realSize.z - bevel),
				Vector3(-realSize.x, -realSize.y - bevel, -realSize.z));

			break;
		}
		case BACK:
		{
			add_quad(this,
				Vector3(-realSize.x, -realSize.y, realSize.z + bevel),
				Vector3(realSize.x, -realSize.y, realSize.z + bevel),
				Vector3(realSize.x, realSize.y, realSize.z + bevel),
				Vector3(-realSize.x, realSize.y, realSize.z + bevel));

			/* left bevel */

			add_quad(this,
				Vector3(-realSize.x, realSize.y, realSize.z + bevel),
				Vector3(-realSize.x - bevel, realSize.y, realSize.z),
				Vector3(-realSize.x - bevel, -realSize.y, realSize.z),
				Vector3(-realSize.x, -realSize.y, realSize.z + bevel));

			/* right bevel */

			add_quad(this,
				Vector3(realSize.x, -realSize.y, realSize.z + bevel),
				Vector3(realSize.x + bevel, -realSize.y, realSize.z),
				Vector3(realSize.x + bevel, realSize.y, realSize.z),
				Vector3(realSize.x, realSize.y, realSize.z + bevel));

			/* bevel tris */

			/* top left? */

			add_triangle(this,
				Vector3(-realSize.x, realSize.y, realSize.z + bevel),
				Vector3(-realSize.x, realSize.y + bevel, realSize.z),
				Vector3(-realSize.x - bevel, realSize.y, realSize.z));

			/* bottom left */

			add_triangle(this,
				Vector3(realSize.x, -realSize.y, realSize.z + bevel),
				Vector3(realSize.x, -realSize.y - bevel, realSize.z),
				Vector3(realSize.x + bevel, -realSize.y, realSize.z));

			/* top right */

			add_triangle(this,
				Vector3(realSize.x, realSize.y, realSize.z + bevel),
				Vector3(realSize.x + bevel, realSize.y, realSize.z),
				Vector3(realSize.x, realSize.y + bevel, realSize.z));
			/* bottom right */
			add_triangle(this,
				Vector3(-realSize.x, -realSize.y, realSize.z + bevel),
				Vector3(-realSize.x - bevel, -realSize.y, realSize.z),
				Vector3(-realSize.x, -realSize.y - bevel, realSize.z));

			break;
		}
		case LEFT:
		{
			add_quad(this,
				Vector3(realSize.x + bevel, realSize.y, -realSize.z),
				Vector3(realSize.x + bevel, realSize.y, realSize.z),
				Vector3(realSize.x + bevel, -realSize.y, realSize.z),
				Vector3(realSize.x + bevel, -realSize.y, -realSize.z));
			break;
		}
		case RIGHT:
		{
			add_quad(this,
				Vector3(-realSize.x - bevel, realSize.y, realSize.z),
				Vector3(-realSize.x - bevel, realSize.y, -realSize.z),
				Vector3(-realSize.x - bevel, -realSize.y, -realSize.z),
				Vector3(-realSize.x - bevel, -realSize.y, realSize.z));
			break;
		}
	}

	switch (surface)
	{
	case BUMP:
	{
		add_bumps(this, size / 4, surface, face);
		break;
	}
	}

}

void block::Block::translateBlock(const CoordinateFrame& cframe)
{
	CoordinateFrame identity = CoordinateFrame();
	for (int i = 0; i < part_indices.size(); i++)
	{/* might break */
		uint32_t indice = part_indices[i];
		Vector3 vertex = __render_world->getVertice(indice);
		Vector3 origin_vertex = identity.pointToObjectSpace(vertex);
		Vector3 translated_vertex = cframe.pointToWorldSpace(origin_vertex);
		__render_world->updateVerticeVert(indice, translated_vertex);
	}
}
