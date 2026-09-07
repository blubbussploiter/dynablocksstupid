#include "b3d.h"

void B3DAnimator::LoadB3DBones(NODEChunk* parentBone)
{
	for (int i = 0; i < parentBone->children.size(); i++)
	{
		NODEChunk* childBone = parentBone->children[i];
		if (childBone->bone)
		{
			BONEChunk* theBone = childBone->bone;
			B3DBone* b3dBone = new B3DBone();

			for (int b = 0; b < theBone->bones.size(); b++)
			{
				BONEChunk::BONE bone = theBone->bones[b];
				B3DBone::BoneInfo boneInfo;
				/* boom */
				boneInfo.affected_vertex = bone.vertex_id;
				boneInfo.weight = bone.weight;
				boneInfo.origin_transform = mesh->vertices[boneInfo.affected_vertex];
				b3dBone->boneInfo.append(boneInfo);
			}

			for (int k = 0; k < childBone->keys.size(); k++)
			{
				KEYSChunk* keys = childBone->keys[k];
				B3DKey key;
				key.flags = keys->flags;
				for (int f = 0; f < keys->keys.size(); f++)
				{
					KEYSChunk::KEYS frames = keys->keys[f];
					B3DKey::Frame frame;
					frame.frame = frames.frame;
					if (key.flags & 1)
					{
						frame.pos = frames.position;
					}					
					if (key.flags & 2)
					{
						frame.scale = frames.scale;
					}
					if (key.flags & 4)
					{
						frame.rot = frames.rotation;
					}
					key.frames.set(frame.frame, frame);
				}
				b3dBone->keys.append(key);
			}

			bones.append(b3dBone);
		}
		LoadB3DBones(childBone);
	}
}

void B3DAnimator::InitAnimator(B3DMesh* parentMesh, B3DSerializedEntity* serializedEnt)
{
	NODEChunk* rootSerChunk = serializedEnt->b3dmain->rootNode;

	mesh = parentMesh;

	/* load anim desc */
	anim->flags = rootSerChunk->anim->flags;
	anim->frames = rootSerChunk->anim->frames;
	anim->fps = rootSerChunk->anim->fps;

	/* load seqs */
	for (int i = 0; i < rootSerChunk->seqs.size(); i++)
	{
		B3DSeq* b3dSeq = new B3DSeq();
		SEQSChunk* seqs = rootSerChunk->seqs[i];
		b3dSeq->start = seqs->start;
		b3dSeq->end = seqs->end;
		b3dSeq->name = seqs->name;
		sequences.append(b3dSeq);
	}

	/* load bones + keys */
	LoadB3DBones(rootSerChunk);

	printf("ANIMATOR: Animator initialized, %d bones, %d seqs\n", bones.size(), sequences.size());
	printf("ANIMATOR: Description\n%d frames\n%f fps\n", anim->frames, anim->fps);
}

void B3DAnimator::LoadSequence(std::string sequenceName)
{
	for (int i = 0; i < sequences.size(); i++)
	{
		B3DSeq* seq = sequences[i];
		if (seq->name == sequenceName)
		{
			currentSequence = seq;
			currentSequenceIndex = i;
			currentSequenceLength = (seq->end - seq->start);
		}
	}
	if (!currentSequence) 
	{
		printf("ANIMATOR: Sequence `%s` not found\n", sequenceName.c_str());
		return;
	}
	printf("ANIMATOR: Loaded sequence `%s`, %d frames long\n", currentSequence->name.c_str(), currentSequenceLength);
}

void B3DAnimator::NextFrame()
{
	if (currentSequence)
	{
		if (currentKeyframe < currentSequenceLength)
		{
			currentKeyframe++;
		}
		else
		{
			currentKeyframe = 0;
		}

		for (int i = 0; i < bones.size(); i++)
		{
			int currentFrame = currentSequence->start + currentKeyframe;

			B3DBone* bone = bones[i];

			if (currentSequenceIndex < bone->keys.size())
			{

				B3DKey seqKey = bone->keys[currentSequenceIndex];
				B3DKey::Frame frame = seqKey.frames[currentFrame];

				for (int v = 0; v < bone->boneInfo.size(); v++)
				{
					B3DBone::BoneInfo info = bone->boneInfo[v];
					if (seqKey.flags & 1)
					{
						//mesh->vertices[info.affected_vertex] = frame.pos;
					}
				}

			}
		}
	}

}

B3DAnimator::B3DAnimator()
{
	mesh = 0;
	anim = new B3DAnimDescriptor();
	currentKeyframe = 0;
	currentSequence = 0;
	currentSequenceLength = 0;
	currentSequenceIndex = -1;
	currentSequenceLooped = 1;
}
