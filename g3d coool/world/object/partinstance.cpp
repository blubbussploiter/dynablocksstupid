#include "partInstance.h"
#include "../applet.h"

void block::BlockInstance::setCoordinateFrame(const CoordinateFrame& newCFrame)
{
	if (cframe != newCFrame)
	{
		if (_block != -1)
		{
			__render_level->notifyBlockTranslate(_block, newCFrame);
		}
		cframe = newCFrame;
	}
}

void block::BlockInstance::setSize(const Vector3& newSize)
{
	Vector3 rndSize; 
	/* round, guess from: https://ia800708.us.archive.org/BookReader/BookReaderImages.php?zip=/26/items/7390-p-001-c-drawings/7390P001C_Drawings_jp2.zip&file=7390P001C_Drawings_jp2/7390P001C_Drawings_0012.jp2&id=7390-p-001-c-drawings&scale=4&rotate=0 */
	rndSize.x = floorf(newSize.x);
	rndSize.y = floorf(newSize.y);
	rndSize.z = floorf(newSize.z);
	if (size != rndSize)
	{
		size = rndSize;
		/* just remake -- might not work smoothly lol */
		if (_block != -1)
		{
			__render_level->removeBlock(_block);
			_block = __render_level->createBlockFromBlockInstance(this);
		}
	}
}

void block::BlockInstance::setColor(const Color3& newColor)
{
	if (color != newColor)
	{
		color = newColor;
		if (_block != -1)
		{
			__render_level->notifyBlockChangeColor(_block, color);
		}
	}
}

void block::BlockInstance::notifyUpdateSurface()
{
	/* just remake -- might not work smoothly lol */
	if (_block != -1)
	{
		//__render_level->removeBlock(_block);
		//_block = __render_level->createBlockFromBlockInstance(this);
	}
}

void block::BlockInstance::doRender(RenderDevice* renderDevice)
{
	if (nameShown)
	{
		GFontRef font = globalApplet->Datamodel()->gui->getDominantFont();
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

block::BlockInstance::BlockInstance() : PVInstance("Block")
{
	className = "Block";
	friction = 0.3f;
	elasticity = 0.5f;
	color = Color3::gray();
	size = Vector3(2, 1, 4);
	blockType = BlockType::NORMAL_BLOCK;
	_block =  -1;
	front = SurfaceType::NO_SURFACE;
	back = SurfaceType::NO_SURFACE;
	bottom = SurfaceType::NO_SURFACE;
	top = SurfaceType::BUMP;
	left = SurfaceType::NO_SURFACE;
	right = SurfaceType::NO_SURFACE;
}

block::BlockInstance::~BlockInstance()
{
}
