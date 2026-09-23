#include "renderLevel.h"
#include "../applet.h"
#include "renderHelp.hpp"
#include "surface.hpp"

#define bevel 0.1f

void block::Block::buildBlockFace(SurfaceType surface, NormalId face, const Vector3& size)
{
	Vector3 renderSize = size / 2;
	renderSize.x -= bevel;
	renderSize.y -= bevel;
	renderSize.z -= bevel;
	switch (face)
	{
		case TOP:
		{
			/* main quad */
			add_quad(this, Vector3(renderSize.x, renderSize.y + bevel, -renderSize.z),
						   Vector3(-renderSize.x, renderSize.y + bevel, -renderSize.z),
						   Vector3(-renderSize.x, renderSize.y + bevel, renderSize.z),
						   Vector3(renderSize.x, renderSize.y + bevel, renderSize.z));
			/* top bevel right */
			add_quad(this, 
				Vector3(renderSize.x + bevel, renderSize.y, -renderSize.z),
				Vector3(renderSize.x, renderSize.y + bevel, -renderSize.z),
				Vector3(renderSize.x, renderSize.y + bevel, renderSize.z),
				Vector3(renderSize.x + bevel, renderSize.y, renderSize.z));
			/* top bevel left */
			add_quad(this,
				Vector3(-renderSize.x - bevel, renderSize.y, renderSize.z),
				Vector3(-renderSize.x, renderSize.y + bevel, renderSize.z),
				Vector3(-renderSize.x, renderSize.y + bevel, -renderSize.z),
				Vector3(-renderSize.x - bevel, renderSize.y, -renderSize.z));
			/* top bevel front */
			add_quad(this, 
				Vector3(-renderSize.x, renderSize.y + bevel, -renderSize.z),
				Vector3(renderSize.x, renderSize.y + bevel, -renderSize.z),
				Vector3(renderSize.x, renderSize.y, -renderSize.z - bevel),
				Vector3(-renderSize.x, renderSize.y, -renderSize.z - bevel));
			/* top bevel back */
			add_quad(this,
				Vector3(renderSize.x, renderSize.y + bevel, renderSize.z),
				Vector3(-renderSize.x, renderSize.y + bevel, renderSize.z),
				Vector3(-renderSize.x, renderSize.y, renderSize.z + bevel),
				Vector3(renderSize.x, renderSize.y, renderSize.z + bevel));
			break;
		}
		case BOTTOM:
		{
			/* main quad */
			add_quad(this, 
				Vector3(renderSize.x, -renderSize.y - bevel, renderSize.z),
				Vector3(-renderSize.x, -renderSize.y - bevel, renderSize.z),
				Vector3(-renderSize.x, -renderSize.y - bevel, -renderSize.z),
				Vector3(renderSize.x, -renderSize.y - bevel, -renderSize.z));
			/* top bevel right */
			add_quad(this,
				Vector3(renderSize.x + bevel, -renderSize.y, renderSize.z),
				Vector3(renderSize.x, -renderSize.y - bevel, renderSize.z),
				Vector3(renderSize.x, -renderSize.y - bevel, -renderSize.z),
				Vector3(renderSize.x + bevel, -renderSize.y, -renderSize.z));
			/* top bevel left */
			add_quad(this,
				Vector3(-renderSize.x - bevel, -renderSize.y, -renderSize.z),
				Vector3(-renderSize.x, -renderSize.y - bevel, -renderSize.z),
				Vector3(-renderSize.x, -renderSize.y - bevel, renderSize.z),
				Vector3(-renderSize.x - bevel, -renderSize.y, renderSize.z));
			/* top bevel front */
			add_quad(this,
				Vector3(renderSize.x, -renderSize.y - bevel, -renderSize.z),
				Vector3(-renderSize.x, -renderSize.y - bevel, -renderSize.z),
				Vector3(-renderSize.x, -renderSize.y, -renderSize.z - bevel),
				Vector3(renderSize.x, -renderSize.y, -renderSize.z - bevel));
			/* top bevel back */
			add_quad(this,
				Vector3(-renderSize.x, -renderSize.y - bevel, renderSize.z),
				Vector3(renderSize.x, -renderSize.y - bevel, renderSize.z),
				Vector3(renderSize.x, -renderSize.y, renderSize.z + bevel),
				Vector3(-renderSize.x, -renderSize.y, renderSize.z + bevel));
			break;
		}
		case FRONT:
		{
			add_quad(this,
				Vector3(renderSize.x, -renderSize.y, -renderSize.z - bevel),
				Vector3(-renderSize.x, -renderSize.y, -renderSize.z - bevel),
				Vector3(-renderSize.x, renderSize.y, -renderSize.z - bevel),
				Vector3(renderSize.x, renderSize.y, -renderSize.z - bevel));

			/* left bevel */

			add_quad(this,
				Vector3(renderSize.x + bevel, -renderSize.y, -renderSize.z),
				Vector3(renderSize.x, -renderSize.y, -renderSize.z - bevel),
				Vector3(renderSize.x, renderSize.y, -renderSize.z - bevel),
				Vector3(renderSize.x + bevel, renderSize.y, -renderSize.z));

			/* top triangle */
			
			add_block_triangle(this,
				Vector3(renderSize.x + bevel, renderSize.y, -renderSize.z),
				Vector3(renderSize.x, renderSize.y, -renderSize.z - bevel),
				Vector3(renderSize.x, renderSize.y + bevel, -renderSize.z));

			/* bottom triangle */
			add_block_triangle(this,
				Vector3(renderSize.x + bevel, -renderSize.y, -renderSize.z),
				Vector3(renderSize.x, -renderSize.y - bevel, -renderSize.z),
				Vector3(renderSize.x, -renderSize.y, -renderSize.z - bevel));

			/* right bevel */

			add_quad(this,
				Vector3(-renderSize.x - bevel, renderSize.y, -renderSize.z),
				Vector3(-renderSize.x, renderSize.y, -renderSize.z - bevel),
				Vector3(-renderSize.x, -renderSize.y, -renderSize.z - bevel),
				Vector3(-renderSize.x - bevel, -renderSize.y, -renderSize.z));

			/* top triangle */

			add_block_triangle(this,
				Vector3(-renderSize.x - bevel, renderSize.y, -renderSize.z), 
				Vector3(-renderSize.x, renderSize.y + bevel, -renderSize.z),
				Vector3(-renderSize.x, renderSize.y, -renderSize.z - bevel));

			/* bottom triangle */

			add_block_triangle(this,
				Vector3(-renderSize.x - bevel, -renderSize.y, -renderSize.z),
				Vector3(-renderSize.x, -renderSize.y, -renderSize.z - bevel),
				Vector3(-renderSize.x, -renderSize.y - bevel, -renderSize.z));

			break;
		}
		case BACK:
		{
			add_quad(this,
				Vector3(-renderSize.x, -renderSize.y, renderSize.z + bevel),
				Vector3(renderSize.x, -renderSize.y, renderSize.z + bevel),
				Vector3(renderSize.x, renderSize.y, renderSize.z + bevel),
				Vector3(-renderSize.x, renderSize.y, renderSize.z + bevel));

			/* left bevel */

			add_quad(this,
				Vector3(-renderSize.x, renderSize.y, renderSize.z + bevel),
				Vector3(-renderSize.x - bevel, renderSize.y, renderSize.z),
				Vector3(-renderSize.x - bevel, -renderSize.y, renderSize.z),
				Vector3(-renderSize.x, -renderSize.y, renderSize.z + bevel));

			/* right bevel */

			add_quad(this,
				Vector3(renderSize.x, -renderSize.y, renderSize.z + bevel),
				Vector3(renderSize.x + bevel, -renderSize.y, renderSize.z),
				Vector3(renderSize.x + bevel, renderSize.y, renderSize.z),
				Vector3(renderSize.x, renderSize.y, renderSize.z + bevel));

			/* bevel tris */

			/* top left? */

			add_block_triangle(this,
				Vector3(-renderSize.x, renderSize.y, renderSize.z + bevel),
				Vector3(-renderSize.x, renderSize.y + bevel, renderSize.z),
				Vector3(-renderSize.x - bevel, renderSize.y, renderSize.z));

			/* bottom left */

			add_block_triangle(this,
				Vector3(renderSize.x, -renderSize.y, renderSize.z + bevel),
				Vector3(renderSize.x, -renderSize.y - bevel, renderSize.z),
				Vector3(renderSize.x + bevel, -renderSize.y, renderSize.z));

			/* top right */

			add_block_triangle(this,
				Vector3(renderSize.x, renderSize.y, renderSize.z + bevel),
				Vector3(renderSize.x + bevel, renderSize.y, renderSize.z),
				Vector3(renderSize.x, renderSize.y + bevel, renderSize.z));
			/* bottom right */
			add_block_triangle(this,
				Vector3(-renderSize.x, -renderSize.y, renderSize.z + bevel),
				Vector3(-renderSize.x - bevel, -renderSize.y, renderSize.z),
				Vector3(-renderSize.x, -renderSize.y - bevel, renderSize.z));

			break;
		}
		case LEFT:
		{
			add_quad(this,
				Vector3(renderSize.x + bevel, renderSize.y, -renderSize.z),
				Vector3(renderSize.x + bevel, renderSize.y, renderSize.z),
				Vector3(renderSize.x + bevel, -renderSize.y, renderSize.z),
				Vector3(renderSize.x + bevel, -renderSize.y, -renderSize.z));
			break;
		}
		case RIGHT:
		{
			add_quad(this,
				Vector3(-renderSize.x - bevel, renderSize.y, renderSize.z),
				Vector3(-renderSize.x - bevel, renderSize.y, -renderSize.z),
				Vector3(-renderSize.x - bevel, -renderSize.y, -renderSize.z),
				Vector3(-renderSize.x - bevel, -renderSize.y, renderSize.z));
			break;
		}
	}

}

void block::Block::build3DSurface(SurfaceType surface, NormalId face, block::Controller::ControllerType controller, const Vector3& size, const CoordinateFrame& cframe)
{
	switch (surface)
	{
		case BUMP:
		{
			add_bumps(this, size / 2, face);
			break;
		}
		case MOTOR:
		case HINGE:
		{
			add_torq_surface(this, size / 2, controller, face, (surface == HINGE));
			break;
		}
	}
}

void block::Block::translateBlock(const CoordinateFrame& cframe)
{
	/* fucking disgusting. change this */
	/* sooo many indice arrays */
	for (int i = 0; i < part_indices.size(); i++)
	{/* might break */
		uint32_t indice = part_indices[i];
		Vector3 vertex = RenderWorld::get()->getVertice(indice);
		Vector3 vertex_relative = center.pointToObjectSpace(vertex);
		Vector3 translated_vertex = cframe.pointToWorldSpace(vertex_relative);
		RenderWorld::get()->updateVerticeVert(indice, translated_vertex);
	}
	for (int i = 0; i < cylinder_indices.size(); i++)
	{
		uint32_t indice = cylinder_indices[i];
		Vector3 vertex = RenderWorld::get()->getVertice(indice);
		Vector3 vertex_relative = center.pointToObjectSpace(vertex);
		Vector3 translated_vertex = cframe.pointToWorldSpace(vertex_relative);
		RenderWorld::get()->updateVerticeVert(indice, translated_vertex);
	}
	translateSurfaces(cframe);
	center = cframe;
}

/* why do this??? just move everything into renderer calls */
void block::Block::translateSurfaces(const CoordinateFrame& cframe)
{
	for (int i = 0; i < surface_indices.size(); i++)
	{/* might break */
		uint32_t indice = surface_indices[i];
		Vector3 vertex = RenderWorld::get()->getVertice(indice);
		Vector3 vertex_relative = center.pointToObjectSpace(vertex);
		Vector3 translated_vertex = cframe.pointToWorldSpace(vertex_relative);
		RenderWorld::get()->updateVerticeVert(indice, translated_vertex);
	}
}

block::Block::Block()
{
	level = 0;
	center = CoordinateFrame();
}
