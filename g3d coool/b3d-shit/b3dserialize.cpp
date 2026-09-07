#include "b3dserialize.h"

bool B3DSerializedEntity::IsTag(const char* tag)
{
	std::string __tag = binaryStream->readString(4);
	binaryStream->setPosition(binaryStream->getPosition() - 4);
	return (__tag == tag);
}

void B3DSerializedEntity::SerializeB3DHeader(B3DChunkHeader* header)
{
	binaryStream->readBytes(&header->tag, 4);
	header->tag[4] = '\0'; /* terminate it, B3D tags are NOT null terminated */
	header->length = binaryStream->readInt32();
}

void B3DSerializedEntity::SerializeBB3DChunk(BB3DChunk* chunk)
{
	SerializeB3DHeader(&chunk->header);
	if (strcmp(chunk->header.tag, "BB3D"))
	{
		printf("Invalid BB3D chunk `%s`\n", chunk->header.tag);
		return;
	}
	if (chunk->header.length == 0)
	{
		return;
	}
	chunk->version = binaryStream->readInt32();

	chunk->textures = new TEXSChunk();
	chunk->brushes = new BRUSChunk();
	chunk->rootNode = new NODEChunk();

	SerializeTEXSChunk(chunk->textures);
	SerializeBRUSChunk(chunk->brushes);
	SerializeNODEChunk(chunk->rootNode);
}

void B3DSerializedEntity::SerializeTEXSChunk(TEXSChunk* chunk)
{
	SerializeB3DHeader(&chunk->header);
	if (strcmp(chunk->header.tag, "TEXS"))
	{
		printf("SERIALIZER: Invalid or no TEXS chunk (no textures?)\n");
		return;
	}
	if (chunk->header.length == 0)
	{
		return;
	}
	int end = binaryStream->getPosition() + chunk->header.length;
	while (binaryStream->getPosition() < end)
	{
		TEXSChunk::TEXS texs;
		texs.file = binaryStream->readString();
		texs.flags = binaryStream->readInt32();
		texs.blend = binaryStream->readInt32();
		texs.x_pos = binaryStream->readFloat32();
		texs.y_pos = binaryStream->readFloat32();
		texs.x_scale = binaryStream->readFloat32();
		texs.y_scale = binaryStream->readFloat32();
		texs.rotation = binaryStream->readFloat32();
		chunk->textures.append(texs);
		printf("SERIALIZER: texs %s\n", texs.file.c_str());
	}
}

void B3DSerializedEntity::SerializeBRUSChunk(BRUSChunk* chunk)
{
	SerializeB3DHeader(&chunk->header);
	if (strcmp(chunk->header.tag, "BRUS"))
	{
		printf("SERIALIZER: Invalid or no BRUS chunk (no textures?)\n");
		return;
	}
	if (chunk->header.length == 0)
	{
		return;
	}
	chunk->n_texs = binaryStream->readInt32();
	int end = binaryStream->getPosition() + (chunk->header.length - 4); /* account for n_texs */
	while (binaryStream->getPosition() < end)
	{
		BRUSChunk::BRUS brus = BRUSChunk::BRUS();
		brus.name = binaryStream->readString();
		brus.red = binaryStream->readFloat32();
		brus.green = binaryStream->readFloat32();
		brus.blue = binaryStream->readFloat32();
		brus.alpha = binaryStream->readFloat32();
		brus.shininess = binaryStream->readFloat32();
		brus.blend = binaryStream->readInt32();
		brus.fx = binaryStream->readInt32();
		brus.texture_id = new int[chunk->n_texs];
		for (int i = 0; i < chunk->n_texs; i++)
		{
			brus.texture_id[i] = binaryStream->readInt32();
		}
		chunk->brushes.append(brus);
		printf("SERIALIZER: brus %s %d texs\n", brus.name.c_str(),
			chunk->n_texs);
	}
}

void B3DSerializedEntity::SerializeVRTSChunk(VRTSChunk* chunk)
{
	SerializeB3DHeader(&chunk->header);
	if (strcmp(chunk->header.tag, "VRTS"))
	{
		printf("SERIALIZER: Invalid or no VRT chunk\n");
		return;
	}

	if (chunk->header.length == 0)
	{
		return;
	}

	int end = binaryStream->getPosition() + chunk->header.length; /* account for previous members */

	chunk->flags = binaryStream->readInt32();
	chunk->tex_coord_sets = binaryStream->readInt32();
	chunk->tex_coord_set_size = binaryStream->readInt32();

	while (binaryStream->getPosition() < end)
	{
		VRTSChunk::VRTS vrts;
		vrts.x = binaryStream->readFloat32();
		vrts.y = binaryStream->readFloat32();
		vrts.z = binaryStream->readFloat32();
		if (chunk->flags & 1)
		{
			vrts.nx = binaryStream->readFloat32();
			vrts.ny = binaryStream->readFloat32();
			vrts.nz = binaryStream->readFloat32();
		}
		if (chunk->flags & 2)
		{
			vrts.red = binaryStream->readFloat32();
			vrts.green = binaryStream->readFloat32();
			vrts.blue = binaryStream->readFloat32();
			vrts.alpha = binaryStream->readFloat32();
		}			
		vrts.tex_coords = (float*)(malloc(sizeof(float) * (chunk->tex_coord_sets * chunk->tex_coord_set_size)));
		/* come back to this */
		for (int i = 0; i < chunk->tex_coord_sets; i++)
		{
			for (int j = 0; j < chunk->tex_coord_set_size; j++)
			{
				vrts.tex_coords[i * chunk->tex_coord_set_size + j] = binaryStream->readFloat32();
			}
		}
		chunk->vertices.append(vrts);
	}
}

void B3DSerializedEntity::SerializeTRISChunk(TRISChunk* chunk)
{
	SerializeB3DHeader(&chunk->header);

	if (strcmp(chunk->header.tag, "TRIS"))
	{
		printf("SERIALIZER: Invalid or no TRIS chunk\n");
		return;
	}

	if (chunk->header.length == 0)
	{
		return;
	}

	int end = binaryStream->getPosition() + chunk->header.length;

	chunk->brush_id = binaryStream->readInt32();

	while (binaryStream->getPosition() < end)
	{
		TRISChunk::TRIS tris;
		tris.vertex_id[0] = binaryStream->readInt32();
		tris.vertex_id[1] = binaryStream->readInt32();
		tris.vertex_id[2] = binaryStream->readInt32();
		chunk->triangles.append(tris);
	}
	printf("SERIALIZER: TRIChunk brush -> %d\n", chunk->brush_id);
}

void B3DSerializedEntity::SerializeMESHChunk(MESHChunk* chunk)
{
	SerializeB3DHeader(&chunk->header);

	if (strcmp(chunk->header.tag, "MESH"))
	{
		printf("SERIALIZER: Invalid or no MESH chunk\n");
		return;
	}

	if (chunk->header.length == 0)
	{
		return;
	}

	int end = binaryStream->getPosition() + chunk->header.length;

	chunk->brush_id = binaryStream->readInt32();
	chunk->vertices = new VRTSChunk();

	SerializeVRTSChunk(chunk->vertices);

	while (binaryStream->getPosition() < end)
	{
		TRISChunk* newTris = new TRISChunk();
		SerializeTRISChunk(newTris);
		chunk->tris.append(newTris);
	}
}

void B3DSerializedEntity::SerializeBONEChunk(BONEChunk* chunk)
{
	SerializeB3DHeader(&chunk->header);

	if (strcmp(chunk->header.tag, "BONE"))
	{
		printf("SERIALIZER: Invalid or no BONE chunk\n");
		return;
	}
	if (chunk->header.length == 0)
	{
		return;
	}

	int end = binaryStream->getPosition() + chunk->header.length;
	while (binaryStream->getPosition() < end)
	{
		BONEChunk::BONE bone;
		bone.vertex_id = binaryStream->readInt32();
		bone.weight = binaryStream->readFloat32();
		chunk->bones.append(bone);
	}
	printf("SERIALIZER: %d bones\n", chunk->bones.size());
}

void B3DSerializedEntity::SerializeKEYSChunk(KEYSChunk* chunk)
{
	SerializeB3DHeader(&chunk->header);
	if (strcmp(chunk->header.tag, "KEYS"))
	{
		printf("SERIALIZER: Invalid or no KEYS chunk\n");
		return;
	}
	if (chunk->header.length == 0)
	{
		return;
	}

	size_t end = binaryStream->getPosition() + chunk->header.length;

	chunk->flags = binaryStream->readInt32();

	while (binaryStream->getPosition() < end)
	{
		KEYSChunk::KEYS key;
		key.frame = binaryStream->readInt32();
		if (chunk->flags & 1)
		{
			key.position[0] = binaryStream->readFloat32();
			key.position[1] = binaryStream->readFloat32();
			key.position[2] = binaryStream->readFloat32();
		}
		if (chunk->flags & 2)
		{
			key.scale[0] = binaryStream->readFloat32();
			key.scale[1] = binaryStream->readFloat32();
			key.scale[2] = binaryStream->readFloat32();
		}
		if (chunk->flags & 4)
		{
			key.rotation[0] = binaryStream->readFloat32();
			key.rotation[1] = binaryStream->readFloat32();
			key.rotation[2] = binaryStream->readFloat32();
			key.rotation[3] = binaryStream->readFloat32();
		}
		chunk->keys.append(key);
	}
}

void B3DSerializedEntity::SerializeANIMChunk(ANIMChunk* chunk)
{
	SerializeB3DHeader(&chunk->header);
	if (strcmp(chunk->header.tag, "ANIM"))
	{
		printf("SERIALIZER: Invalid or no ANIM chunk\n");
		return;
	}
	if (chunk->header.length == 0)
	{
		return;
	}

	size_t end = binaryStream->getPosition() + chunk->header.length;

	chunk->flags = binaryStream->readInt32();
	chunk->frames = binaryStream->readInt32();
	chunk->fps = binaryStream->readFloat32();
	printf("SERIALIZER: ANIM %d frames, fps = %f\n", chunk->frames, chunk->fps);
}

void B3DSerializedEntity::SerializeNODEChunk(NODEChunk* chunk)
{
	SerializeB3DHeader(&chunk->header);
	if (strcmp(chunk->header.tag, "NODE"))
	{
		printf("SERIALIZER: Invalid or no NODE chunk\n");
		return;
	}
	if (chunk->header.length == 0)
	{
		return;
	}

	chunk->name = binaryStream->readString();
	chunk->position[0] = binaryStream->readFloat32();
	chunk->position[1] = binaryStream->readFloat32();
	chunk->position[2] = binaryStream->readFloat32();
	chunk->scale[0] = binaryStream->readFloat32();
	chunk->scale[1] = binaryStream->readFloat32();
	chunk->scale[2] = binaryStream->readFloat32();	
	chunk->rotation[0] = binaryStream->readFloat32();
	chunk->rotation[1] = binaryStream->readFloat32();
	chunk->rotation[2] = binaryStream->readFloat32();
	chunk->rotation[3] = binaryStream->readFloat32();

	std::string nodeType = binaryStream->readString(4);
	binaryStream->setPosition(binaryStream->getPosition() - 4); /* seek back */

	printf("SERIALIZER: NODE %s -> %s\n", chunk->name.c_str(), nodeType.c_str());

	if (nodeType == "MESH")
	{
		chunk->mesh = new MESHChunk();
		SerializeMESHChunk(chunk->mesh);
	}
	else if (nodeType == "BONE")
	{
		chunk->bone = new BONEChunk();
		SerializeBONEChunk(chunk->bone);
	}
	else
	{
		/* implement pivot... fuck */
		printf("SERIALIZER: NULL or PIVOT node! uh oh\n");
	}

	while (IsTag("KEYS"))
	{
		KEYSChunk* newKeys = new KEYSChunk();
		SerializeKEYSChunk(newKeys);
		chunk->keys.append(newKeys);
	}
	printf("SERIALIZER: %d keys in %s\n", chunk->keys.size(), chunk->name.c_str());

	if (IsTag("ANIM"))
	{
		chunk->anim = new ANIMChunk();
		SerializeANIMChunk(chunk->anim);
	}

	while (IsTag("SEQS"))
	{
		SEQSChunk* seq = new SEQSChunk();
		SerializeSEQSChunk(seq);
		chunk->seqs.append(seq);
	}
	while (IsTag("NODE"))
	{
		NODEChunk* newNode = new NODEChunk();
		SerializeNODEChunk(newNode);
		chunk->children.append(newNode);
	}
}

void B3DSerializedEntity::SerializeSEQSChunk(SEQSChunk* chunk)
{
	SerializeB3DHeader(&chunk->header);
	if (strcmp(chunk->header.tag, "SEQS"))
	{
		printf("SERIALIZER: Invalid or no SEQS chunk\n");
		return;
	}
	if (chunk->header.length == 0)
	{
		return;
	}
	chunk->name = binaryStream->readString();
	chunk->start = binaryStream->readInt32(); /* guessing int32 for all of these */
	chunk->end = binaryStream->readInt32();
	chunk->unk2 = binaryStream->readInt32();
	printf("SERIALIZER: Sequence %s, start %d end %d ? %d\n", chunk->name.c_str(), chunk->start, chunk->end, chunk->unk2);
}

void B3DSerializedEntity::Serialize(const std::string& fileName)
{
	printf("SERIALIZER: Serializing model %s\n", fileName.c_str());

	binaryStream = new BinaryInput(fileName, G3D_LITTLE_ENDIAN);
	b3dmain = new BB3DChunk();

	SerializeBB3DChunk(b3dmain);

}

B3DSerializedEntity::~B3DSerializedEntity()
{
	if (b3dmain)
	{
		if (b3dmain->rootNode)
		{
			if (b3dmain->rootNode->anim)
			{
				delete b3dmain->rootNode->anim;
			}

			if (b3dmain->rootNode->bone)
			{
				delete b3dmain->rootNode->bone;
			}

			if (b3dmain->rootNode->mesh)
			{
				delete b3dmain->rootNode->mesh->vertices;
				delete b3dmain->rootNode->mesh;
			}
		}
	}

	delete binaryStream;
}
