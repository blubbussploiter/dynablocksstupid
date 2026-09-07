#include "renderWorld.h"

void block::RenderWorld::doRender(RenderDevice* renderDevice)
{
	if (!vertexVAR.valid())
	{
		createVAR();
	}

	if (varDirty)
	{
		updateVAR();
		varDirty = true;
	}

	renderDevice->beginIndexedPrimitives();

	renderDevice->setVertexArray(vertexVAR);
	renderDevice->setColorArray(colorVAR);
	renderDevice->setNormalArray(normalVAR);

	renderDevice->sendIndices(RenderDevice::TRIANGLES, indices);

	renderDevice->endIndexedPrimitives();
}

void block::RenderWorld::createVAR()
{
	if (vertexVAR.valid())
	{
		return;
	}
	vertexVAR = VAR(vertices, varArea);
	normalVAR = VAR(normals, varArea);
	colorVAR = VAR(color, varArea);
}

void block::RenderWorld::updateVAR()
{
	if (vertexVAR.valid())
	{
		vertexVAR.update(vertices);
		normalVAR.update(normals);
		colorVAR.update(color);
	}
}

Vector3 block::RenderWorld::getVertice(uint32_t index)
{
	if (indices.size() > index)
	{
		return vertices[index];
	}
	return Vector3::zero();
}

uint32_t block::RenderWorld::addVertice(Vector3 vertex, Vector3 normal, Color4 ncolor, Vector2 uv)
{
	if (free_indices.size() > 0)
	{
		uint32_t last_free = free_indices.pop();
		vertices[last_free] = vertex;
		normals[last_free] = normal;
		color[last_free] = ncolor;
		texture[last_free] = uv;
		free_indices[last_free] = 1;
		return last_free;
	}
	else
	{
		int indice = indices.size();
		indices.push_back(indice);
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
	if (indices.size() > index)
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
	if (indices.size() > index)
	{
		color[index] = ncolor;
		varDirty = true;
	}
}

void block::RenderWorld::updateVerticeVert(uint32_t index, Vector3 vert)
{
	if (indices.size() > index)
	{
		vertices[index] = vert;
		varDirty = true;
	}
}

void block::RenderWorld::updateVerticeNormal(uint32_t index, Vector3 norm)
{
	if (indices.size() > index)
	{
		normals[index] = norm;
		varDirty = true;
	}
}

void block::RenderWorld::updateVerticeUV(uint32_t index, Vector2 uv)
{
	if (indices.size() > index)
	{
		texture[index] = uv;
		varDirty = true;
	}
}

void block::RenderWorld::removeVertice(uint32_t index)
{
	if (indices.size() > index)
	{
		uint32_t indice = indices[index];
		if (indice == 1)
		{
			indices[index] = 0;
			free_indices.append(index);
		}
		else
		{
			indices[index] = indice - 1;
		}
	}
}

block::RenderWorld::RenderWorld()
{
	varArea = VARArea::create(((16777216) + 8) * 4, VARArea::WRITE_ONCE);
	varDirty = false;
	printf("varArea size = %d kb\n", varArea->freeSize() / 1024);
}
