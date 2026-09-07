
#include "b3d.h"
#include "applet.h"

void B3DEntity::CreateFromB3D(std::string b3dFileName)
{
	B3DSerializedEntity* serializedEnt = new B3DSerializedEntity();
	BB3DChunk* b3dmain = 0;

	serializedEnt->Serialize(b3dFileName);
	b3dmain = serializedEnt->b3dmain;

	if (!b3dmain)
	{
		printf("ENTITY: Failed to create `%s`\n", b3dFileName.c_str());
		return;
	}

	printf("ENTITY: Done serializing\n");
	printf("ENTITY: Converting...\n");

	MESHChunk* loadedMesh = b3dmain->rootNode->mesh;
	if (loadedMesh)
	{
		cframe = CoordinateFrame(Matrix3::fromEulerAnglesXYZ(-90,0,0),
			Vector3(b3dmain->rootNode->position));

		for (int i = 0; i < loadedMesh->vertices->vertices.size(); i++)
		{
			VRTSChunk::VRTS vrts = loadedMesh->vertices->vertices[i];
			mesh->vertices.append(Vector3(vrts.x, vrts.y, vrts.z) * b3dmain->rootNode->scale);
			mesh->normals.append(Vector3(vrts.nx, vrts.ny, vrts.nz));
			mesh->color.append(Color4(vrts.red, vrts.green, vrts.blue, vrts.alpha));
		}

		for (int i = 0; i < loadedMesh->tris.size(); i++)
		{
			B3DMesh::Triangles g3d_tris;
			TRISChunk* tris = loadedMesh->tris[i];

			g3d_tris.brush = tris->brush_id;

			for (int o = 0; o < tris->triangles.size(); o++)
			{
				g3d_tris.indices.append(tris->triangles[o].vertex_id[0]);
				g3d_tris.indices.append(tris->triangles[o].vertex_id[1]);
				g3d_tris.indices.append(tris->triangles[o].vertex_id[2]);
			}

			mesh->triangles.append(g3d_tris);
		}

	}

	InitAnimator(mesh, serializedEnt);

	delete serializedEnt;
}

void B3DEntity::Render(RenderDevice* renderDevice)
{
	if (!mesh->vertexVAR.valid())
	{
		CreateVAR();
	}

	renderDevice->beginIndexedPrimitives();

	renderDevice->setVertexArray(mesh->vertexVAR);
	renderDevice->setColorArray(mesh->colorVAR);
	renderDevice->setNormalArray(mesh->normalVAR);

	renderDevice->setObjectToWorldMatrix(cframe);

	NextFrame();
	RefreshVerticesVAR();

	for (int i = 0; i < mesh->triangles.size(); i++)
	{
		B3DMesh::Triangles tris = mesh->triangles[i];
		renderDevice->sendIndices(RenderDevice::TRIANGLES, tris.indices);
	}

	renderDevice->endIndexedPrimitives();
}

void B3DEntity::CreateVAR()
{
	if (mesh->vertexVAR.valid())
	{
		return;
	}
	mesh->vertexVAR = VAR(mesh->vertices, globalApplet->varArea);
	mesh->normalVAR = VAR(mesh->normals, globalApplet->varArea);
	mesh->colorVAR = VAR(mesh->color, globalApplet->varArea);
}

void B3DEntity::RefreshVerticesVAR()
{
	if (mesh->vertexVAR.valid())
	{
		mesh->vertexVAR.update(mesh->vertices);
		mesh->normalVAR.update(mesh->normals);
		mesh->colorVAR.update(mesh->color);
	}
}

B3DEntity::B3DEntity()
{
	mesh = new B3DMesh();
}
