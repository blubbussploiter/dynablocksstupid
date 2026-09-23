#pragma once

#include <G3DAll.h>
#include "renderLevel.h"

namespace block
{
	class RenderWorld
	{
	private:


		VARAreaRef varArea;

		Array<uint32_t> vertexRef;
		Array<uint32_t> freeVertex;

		Array<Vector3> vertices;
		Array<Vector3> normals;
		Array<Color4> color;
		Array<Vector2> texture;
		VAR vertexVAR;
		VAR normalVAR;
		VAR colorVAR;
		VAR textureVAR;

		bool varDirty;

	public:

		void beginRender(RenderDevice* renderDevice);
		void endRender(RenderDevice* renderDevice);
		void sendGeometry(RenderDevice* renderDevice);

		void createVAR();
		
		Vector3 getVertice(uint32_t index);
		Vector3 getNormal(uint32_t index);
		uint32_t addVertice(Vector3 vertex, Vector3 normal, Color4 color, Vector2 uv);
		void updateVertice(uint32_t index, Vector3 vertex, Vector3 normal, Color4 color, Vector2 uv);
		void updateVerticeColor(uint32_t index, Color4 color);
		void updateVerticeVert(uint32_t index, Vector3 vert);
		void updateVerticeNormal(uint32_t index, Vector3 norm);
		void updateVerticeUV(uint32_t index, Vector2 uv);
		void removeVertice(uint32_t index);

		static RenderWorld* get();

		RenderWorld();
		~RenderWorld();

	};
}