#pragma once

#include <G3DAll.h>

struct B3DChunkHeader
{
	char tag[5];
	int length;
};

struct TEXSChunk
{
	B3DChunkHeader header;
	struct TEXS
	{
		std::string file;
		int flags, blend;
		float x_pos, y_pos;
		float x_scale, y_scale;
		float rotation;
	};
	Array<TEXS> textures;
};

struct BRUSChunk
{
	B3DChunkHeader header;
	int n_texs;
	struct BRUS
	{
		std::string name;
		float red, green, blue, alpha;
		float shininess;
		int blend, fx;
		int* texture_id;
	};
	Array<BRUS> brushes;
};

struct VRTSChunk
{
	B3DChunkHeader header;
	int flags;
	int tex_coord_sets;
	int tex_coord_set_size;
	struct VRTS
	{
		float x, y, z;
		float nx, ny, nz;
		float red, green, blue, alpha;
		float* tex_coords;
	};
	Array<VRTS> vertices;
};

struct TRISChunk
{
	B3DChunkHeader header;
	int brush_id;
	struct TRIS
	{
		int vertex_id[3];
	};
	Array<TRIS> triangles;
};

struct MESHChunk
{
	B3DChunkHeader header;
	int brush_id;
	VRTSChunk* vertices;
	Array<TRISChunk*> tris;
};

struct BONEChunk
{
	B3DChunkHeader header;
	struct BONE
	{
		int vertex_id;
		float weight;
	};
	Array<BONE> bones;
};

struct KEYSChunk
{
	B3DChunkHeader header;
	int flags;
	struct KEYS
	{
		int frame;
		float position[3];
		float scale[3];
		float rotation[4];
	};
	Array<KEYS> keys;
};

/* no documentation online on what a SEQS chunk does,, we gotta guess */
struct SEQSChunk
{
	B3DChunkHeader header;
	std::string name;
	int start;
	int end;
	int unk2;
};

struct ANIMChunk
{
	B3DChunkHeader header;
	int flags;
	int frames;
	float fps;
};

struct NODEChunk
{
	B3DChunkHeader header;
	std::string name;
	float position[3];
	float scale[3];
	float rotation[4];
	MESHChunk* mesh;
	BONEChunk* bone;
	ANIMChunk* anim;
	Array<KEYSChunk*> keys;
	Array<NODEChunk*> children;
	Array<SEQSChunk*> seqs; /* what does this do?? */
};

struct BB3DChunk
{
	B3DChunkHeader header;
	int version;
	TEXSChunk* textures;
	BRUSChunk* brushes;
	NODEChunk* rootNode;
};

class B3DSerializedEntity
{
private:
	BinaryInput* binaryStream;
public:
	BB3DChunk* b3dmain;
protected:

	bool IsTag(const char* tag);

	void SerializeB3DHeader(B3DChunkHeader* header);
	void SerializeBB3DChunk(BB3DChunk* chunk);
	void SerializeTEXSChunk(TEXSChunk* chunk);
	void SerializeBRUSChunk(BRUSChunk* chunk);
	void SerializeVRTSChunk(VRTSChunk* chunk);
	void SerializeTRISChunk(TRISChunk* chunk);
	void SerializeMESHChunk(MESHChunk* chunk);
	void SerializeBONEChunk(BONEChunk* chunk);
	void SerializeKEYSChunk(KEYSChunk* chunk);
	void SerializeANIMChunk(ANIMChunk* chunk);
	void SerializeNODEChunk(NODEChunk* chunk);
	void SerializeSEQSChunk(SEQSChunk* chunk);

public:

	void Serialize(const std::string& fileName);

	B3DSerializedEntity()
	{
		b3dmain = 0;
		binaryStream = 0;
	}

	~B3DSerializedEntity();
};