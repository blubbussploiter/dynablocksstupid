#include "partInstance.h"
#include "../applet.h"
#include "../ui/fonts.h"

/* thank you RNR */
static block::NormalId normals_aligned[6] = {
	block::NormalId::FRONT,
	block::NormalId::BACK,
	block::NormalId::RIGHT,
	block::NormalId::LEFT,
	block::NormalId::TOP,
	block::NormalId::BOTTOM
};

void block::PartInstance::setCoordinateFrame(const CoordinateFrame& newCFrame)
{
	if (cframe != newCFrame)
	{
		if (_block != -1)
		{
			RenderLevel::get()->notifyBlockTranslate(_block, newCFrame);
		}
		cframe = newCFrame;
		if (body)
		{
			body->setPosition(cframe);
		}
		else if (primitive)
		{
			primitive->setPosition(cframe);
		}
	}
}

void block::PartInstance::setSize(const Vector3& newSize)
{
	Vector3 rndSize; 
	/* round, guess from: https://ia800708.us.archive.org/BookReader/BookReaderImages.php?zip=/26/items/7390-p-001-c-drawings/7390P001C_Drawings_jp2.zip&file=7390P001C_Drawings_jp2/7390P001C_Drawings_0012.jp2&id=7390-p-001-c-drawings&scale=4&rotate=0 */
	rndSize.x = floorf(newSize.x);
	rndSize.y = floorf(newSize.y);
	rndSize.z = floorf(newSize.z);
	if (size != rndSize)
	{
		size = rndSize;
		if (blockType != NORMAL_BLOCK)
		{
			size.x = max(size.x, max(size.y, size.z));
			size.y = size.x;
			size.z = size.x;
		}
		notifyGeometryUpdate();
		if (body)
		{
			body->setSize(size);
		}
		else if (primitive)
		{
			primitive->setSize(size);
		}
	}
}

void block::PartInstance::setColor(const Color3& newColor)
{
	setColor4(newColor);
}

void block::PartInstance::setColor4(const Color4& newColor)
{
	if (color != newColor)
	{
		color = newColor;
		if (_block != -1)
		{
			RenderLevel::get()->notifyBlockChangeColor(_block, color);
		}
	}
}

void block::PartInstance::setAnchored(bool anchored) 
{
	this->anchored = anchored;
	ignore = anchored;
	if (body)
	{
		body->setAnchored(anchored);
		if (anchored)
		{
			primitive->detach();
		}
		else {
			primitive->attach(body);
		}
	}
}

void block::PartInstance::setVelocity(Vector3 velocity)
{
	if (body)
	{
		Physics::Velocity vel = body->getVelocity();
		vel.lin = velocity;
		body->setVelocity(vel);
	}
}

void block::PartInstance::setRotVelocity(Vector3 velocity)
{
	if (body)
	{
		Physics::Velocity vel = body->getVelocity();
		vel.rot = velocity;
		body->setVelocity(vel);
	}
}

Vector3 block::PartInstance::getVelocity()
{
	if (body)
	{
		Physics::Velocity vel = body->getVelocity();
		return vel.lin;
	}
	return Vector3::ZERO;
}

Vector3 block::PartInstance::getRotVelocity()
{
	if (body)
	{
		Physics::Velocity vel = body->getVelocity();
		return vel.rot;
	}
	return Vector3::ZERO;
}

block::Physics::Body* block::PartInstance::getBody()
{
	if (body && body->getParent())
	{
		return body->getParent();
	}
	return body;
}

void block::PartInstance::notifyGeometryUpdate()
{
	/* just remake -- might not work smoothly lol */
	if (_block != -1)
	{
		RenderLevel::get()->removeBlock(_block);
		_block = -1;
		_block = RenderLevel::get()->createBlockFromBlockInstance(this);
	}
}

void block::PartInstance::notifyLevelUpdate()
{
	if (_block != -1)
	{
		if (transparency > 0)
		{
			RenderLevel::get()->changeLevel(_block, RenderLevel::get()->transparentLevel);
		}
		else
		{
			RenderLevel::get()->changeLevel(_block, RenderLevel::get()->opaqueLevel);
		}
	}
}

void block::PartInstance::onStep()
{
	if (!ignore)
	{
		if (primitive)
		{
			CoordinateFrame position = primitive->getPosition();

			if (cframe != position && _block != -1)
			{
				RenderLevel::get()->notifyBlockTranslate(_block, position);
				cframe = position;
			}
		}
		if (body)
		{
			float velLength = (getVelocity() + getRotVelocity()).length();
			if (body->atRest() && velLength > 0.5f)
			{
				body->wakeUp();
			}
		}
	}
}

void block::PartInstance::doRender(RenderDevice* renderDevice)
{
	if (nameShown)
	{
		GFontRef font = Fonts::getDominantFont();
		Vector3 gamepoint = getPosition();
		Vector3 camerapoint = renderDevice->getCameraToWorldMatrix().translation;
		float distance = powf(powf(gamepoint.x - camerapoint.x, 2) + powf(gamepoint.y - camerapoint.y, 2) + powf(gamepoint.z - camerapoint.z, 2), 0.5f);
		if (distance < 100 && distance > -100)
		{
			if (distance < 0)
				distance = distance * -1;

			renderDevice->pushState();

			renderDevice->setDepthWrite(0);
			renderDevice->setDepthTest((RenderDevice::DepthTest)(RenderDevice::DEPTH_ALWAYS_PASS));

			CoordinateFrame pos(renderDevice->getCameraToWorldMatrix().rotation, gamepoint);

			font->draw3D(renderDevice, name, pos, 0.05 * distance, Color3::yellow(), Color3::black(), G3D::GFont::XALIGN_CENTER, G3D::GFont::YALIGN_CENTER);

			renderDevice->setDepthWrite(1);

			renderDevice->popState();
		}
	}
}

Vector3 block::PartInstance::getNormalIdDirection(NormalId normal)
{
	switch (normal)
	{
	case NormalId::TOP:
	{
		return Vector3::unitY();
	}
	case NormalId::BOTTOM:
	{
		return -Vector3::unitY();
	}
	case NormalId::RIGHT:
	{
		return Vector3::unitX();
	}
	case NormalId::LEFT:
	{
		return -Vector3::unitX();
	}
	case NormalId::FRONT:
	{
		return Vector3::unitZ();
	}
	case NormalId::BACK:
	{
		return -Vector3::unitZ();
	}
	}
	return Vector3::zero();
}

Vector3 block::PartInstance::getNormalFromId(const CoordinateFrame& cframe, NormalId normal)
{
	return cframe.vectorToWorldSpace(getNormalIdDirection(normal));
}

block::NormalId block::PartInstance::getNormalFromVector(const CoordinateFrame& cframeWorld, const Vector3& direction)
{
	float tolerance = 0.001f;

	for (size_t i = 0; i < 6; i++)
	{
		NormalId n = normals_aligned[i];
		if (dot(getNormalFromId(cframeWorld, n), direction) > tolerance)
		{
			return n;
		}
	}
	return UNDEFINED;
}

block::PartInstance::PartInstance() : PVInstance("Block")
{
	className = "Block";
	friction = 0.3f;
	elasticity = 0.5f;
	color = Color3::gray();
	size = Vector3(4, 1, 2);
	blockType = BlockType::NORMAL_BLOCK;
	_block =  -1;
	front = SurfaceType::NO_SURFACE;
	back = SurfaceType::NO_SURFACE;
	bottom = SurfaceType::NO_SURFACE;
	top = SurfaceType::BUMP;
	left = SurfaceType::NO_SURFACE;
	right = SurfaceType::NO_SURFACE;
	canCollide = true;
	anchored = false;
	body = 0;
	primitive = 0;
}

block::PartInstance::~PartInstance()
{
}
