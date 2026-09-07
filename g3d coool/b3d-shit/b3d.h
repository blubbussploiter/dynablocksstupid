#pragma once

#include <G3DAll.h>
#include "G3D/Table.h"
#include "b3dserialize.h"

struct B3DSeq
{
	std::string name;
	int start;
	int end;
};

struct B3DKey
{
	int flags;
	struct Frame
	{
		int frame;
		Vector3 pos;
		Vector3 scale;
		Vector4 rot;
	};
	Table<int, Frame> frames;
};

struct B3DBone
{
	struct BoneInfo
	{
		Vector3 origin_transform;
		int affected_vertex;
		float weight;
	};
	Array<BoneInfo> boneInfo;
	Array<B3DKey> keys;
};

struct B3DAnimDescriptor
{
	int flags;
	int frames;
	float fps;
};

struct B3DMesh
{
	Array<Vector3> vertices;
	Array<Vector3> normals;
	Array<Color4> color;
	Array<Vector2> texture;
	VAR vertexVAR;
	VAR normalVAR;
	VAR colorVAR;
	VAR textureVAR;

	struct Triangles
	{
		int brush;
		Array<uint32_t> indices;
	};

	Array<Triangles> triangles;
};

class B3DAnimator
{
protected:

	B3DMesh* mesh;
	B3DAnimDescriptor* anim;
	B3DSeq* currentSequence;

	Array<B3DSeq*> sequences;
	Array<B3DBone*> bones;

	int currentKeyframe;
	int currentSequenceIndex;
	int currentSequenceLength;
	bool currentSequenceLooped;

	void LoadB3DBones(NODEChunk* parentBone);

public:

	/* Loads B3D bones, sequences and anim */
	void InitAnimator(B3DMesh* parentMesh, B3DSerializedEntity* serializedEnt);
	void LoadSequence(std::string sequenceName);
	void NextFrame();

	B3DAnimator();
};

class B3DEntity : public B3DAnimator
{
protected:

	CoordinateFrame cframe;

	void CreateVAR();
	void RefreshVerticesVAR();

public:

	void CreateFromB3D(std::string b3dFileName);
	void Render(RenderDevice* renderDevice);

	B3DEntity();
};