#include "renderLevel.h"
#include "../applet.h"
#include "renderHelp.hpp"
#include "surface.hpp"

#define bevel 0.05f

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
			
			add_block_triangle(this,
				Vector3(realSize.x + bevel, realSize.y, -realSize.z),
				Vector3(realSize.x, realSize.y, -realSize.z - bevel),
				Vector3(realSize.x, realSize.y + bevel, -realSize.z));

			/* bottom triangle */
			add_block_triangle(this,
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

			add_block_triangle(this,
				Vector3(-realSize.x - bevel, realSize.y, -realSize.z), 
				Vector3(-realSize.x, realSize.y + bevel, -realSize.z),
				Vector3(-realSize.x, realSize.y, -realSize.z - bevel));

			/* bottom triangle */

			add_block_triangle(this,
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

			add_block_triangle(this,
				Vector3(-realSize.x, realSize.y, realSize.z + bevel),
				Vector3(-realSize.x, realSize.y + bevel, realSize.z),
				Vector3(-realSize.x - bevel, realSize.y, realSize.z));

			/* bottom left */

			add_block_triangle(this,
				Vector3(realSize.x, -realSize.y, realSize.z + bevel),
				Vector3(realSize.x, -realSize.y - bevel, realSize.z),
				Vector3(realSize.x + bevel, -realSize.y, realSize.z));

			/* top right */

			add_block_triangle(this,
				Vector3(realSize.x, realSize.y, realSize.z + bevel),
				Vector3(realSize.x + bevel, realSize.y, realSize.z),
				Vector3(realSize.x, realSize.y + bevel, realSize.z));
			/* bottom right */
			add_block_triangle(this,
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

}

void block::Block::build3DSurface(SurfaceType surface, Face face, block::Controller::ControllerType controller, const Vector3& size, const CoordinateFrame& cframe)
{
	switch (surface)
	{
		case BUMP:
		{
			add_bumps(this, size / 4, face);
			break;
		}
		case MOTOR:
		case HINGE:
		{
			add_torq_surface(this, size / 4, controller, face, (surface == HINGE));
			break;
		}
	}
}

void block::Block::translateBlock(const CoordinateFrame& cframe)
{
	/* sooo many indice arrays */
	for (int i = 0; i < part_indices.size(); i++)
	{/* might break */
		uint32_t indice = part_indices[i];
		Vector3 vertex = __render_world->getVertice(indice);
		Vector3 vertex_relative = center.pointToObjectSpace(vertex);
		Vector3 translated_vertex = cframe.pointToWorldSpace(vertex_relative);
		__render_world->updateVerticeVert(indice, translated_vertex);
	}
	for (int i = 0; i < cylinder_indices.size(); i++)
	{
		uint32_t indice = cylinder_indices[i];
		Vector3 vertex = __render_world->getVertice(indice);
		Vector3 vertex_relative = center.pointToObjectSpace(vertex);
		Vector3 translated_vertex = cframe.pointToWorldSpace(vertex_relative);
		__render_world->updateVerticeVert(indice, translated_vertex);
	}
	translateSurfaces(cframe);
	center = cframe;
}

void block::Block::translateSurfaces(const CoordinateFrame& cframe)
{
	for (int i = 0; i < surface_indices.size(); i++)
	{/* might break */
		uint32_t indice = surface_indices[i];
		Vector3 vertex = __render_world->getVertice(indice);
		Vector3 vertex_relative = center.pointToObjectSpace(vertex);
		Vector3 translated_vertex = cframe.pointToWorldSpace(vertex_relative);
		__render_world->updateVerticeVert(indice, translated_vertex);
	}
}
