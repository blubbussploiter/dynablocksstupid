#include "renderWorld.h"
#include "../applet.h"

#define VAREA_BASESZ ((16777216) + 8) * 4

void block::RenderWorld::beginRender(RenderDevice* renderDevice)
{
	renderDevice->beginIndexedPrimitives();
	sendGeometry(renderDevice);
}

void block::RenderWorld::endRender(RenderDevice* renderDevice)
{
	renderDevice->endIndexedPrimitives();
}

void block::RenderWorld::sendGeometry(RenderDevice* renderDevice)
{
	if (varDirty)
	{
		createVAR();
		varDirty = false;
	}

	renderDevice->setVertexArray(vertexVAR);
	renderDevice->setColorArray(colorVAR);
	renderDevice->setNormalArray(normalVAR);

}

void block::RenderWorld::createVAR()
{
	if (varDirty)
	{
		varArea = VARArea::create((44 * vertices.size() + 1024), VARArea::WRITE_ONCE);

		vertexVAR = VAR(vertices, varArea);
		normalVAR = VAR(normals, varArea);
		colorVAR = VAR(color, varArea);
	}
}

Vector3 block::RenderWorld::getVertice(uint32_t index)
{
	if (vertexRef.size() > index)
	{
		return vertices[index];
	}
	return Vector3::zero();
}

Vector3 block::RenderWorld::getNormal(uint32_t index)
{
	if (vertexRef.size() > index)
	{
		return normals[index];
	}
	return Vector3::zero();
}

uint32_t block::RenderWorld::addVertice(Vector3 vertex, Vector3 normal, Color4 ncolor, Vector2 uv)
{
	if (freeVertex.size() > 0)
	{
		uint32_t last_free = freeVertex.pop();
		vertices[last_free] = vertex;
		normals[last_free] = normal;
		color[last_free] = ncolor;
		texture[last_free] = uv;
		freeVertex[last_free] = 1;
		return last_free;
	}
	else
	{
		int indice = vertexRef.size();
		vertexRef.push_back(indice);
		vertices.push_back(vertex);
		normals.push_back(normal);
		color.push_back(ncolor);
		texture.push_back(uv);
		varDirty = true;
		return indice;
	}
}

void block::RenderWorld::updateVertice(uint32_t index, Vector3 vertex, Vector3 normal, Color4 ncolor, Vector2 uv)
{
	if (vertexRef.size() > index)
	{
		vertices[index] = vertex;
		normals[index] = normal;
		color[index] = ncolor;
		texture[index] = uv;
		varDirty = true;
	}
}

void block::RenderWorld::updateVerticeColor(uint32_t index, Color4 ncolor)
{
	if (vertexRef.size() > index)
	{
		//printf("Makes dirty update COL\n");
		color[index] = ncolor;
		varDirty = true;
	}
}

void block::RenderWorld::updateVerticeVert(uint32_t index, Vector3 vert)
{
	if (vertexRef.size() > index)
	{
		//printf("Makes dirty update VERT\n");
		vertices[index] = vert;
		varDirty = true;
	}
}

void block::RenderWorld::updateVerticeNormal(uint32_t index, Vector3 norm)
{
	if (vertexRef.size() > index)
	{
		//printf("Makes dirty update NRM\n");
		normals[index] = norm;
		varDirty = true;
	}
}

void block::RenderWorld::updateVerticeUV(uint32_t index, Vector2 uv)
{
	if (vertexRef.size() > index)
	{
		//printf("Makes dirty update UV\n");
		texture[index] = uv;
		varDirty = true;
	}
}

void block::RenderWorld::removeVertice(uint32_t index)
{
	uint32_t indice = vertexRef[index];
	if (indice == 1)
	{
		vertexRef[index] = 0;
		freeVertex.append(index);
	}
	else
	{
		vertexRef[index] = indice - 1;
	}
}

block::RenderWorld::RenderWorld()
{
	varArea = VARArea::create(VAREA_BASESZ, VARArea::WRITE_EVERY_FRAME);
	varDirty = false;
	printf("varArea size = %d kb\n", varArea->freeSize() / 1024);
}

block::RenderWorld::~RenderWorld()
{

}

block::RenderWorld* block::RenderWorld::get()
{
	return Datamodel::get()->renderWorld;
}