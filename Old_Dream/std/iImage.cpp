#include "iImage.h"

#include "iStd.h"

iImage::iImage()
{
	imgs = new iArray(cb);
	tex = NULL;
	position = iPointZero;
	frame = 0;
	aniDt = 0.0f;
	_aniDt = default_aniDt;
	animation = false;
	repeatNum = _repeatNum = 0;// inf
	method = NULL;
	parm = NULL;

	alpha = 1.0f;
	degree = 0.0f;
	scale = 1.0f;
	reverse = REVERSE_NONE;
}

iImage::~iImage()
{
	delete imgs;
}

void iImage::cb(void* data)
{
	freeImage((Texture*)data);
}

void iImage::paint(float dt, iPoint p)
{
	if (animation)
	{
		aniDt += dt;
		if (aniDt >= _aniDt)
		{
			aniDt -= _aniDt;

			frame++;
			if (frame > imgs->count - 1)
			{
				if (_repeatNum == 0)
					frame = 0;
				else// if (_repeatNum)
				{
					printf("repeatNum = %d\n", repeatNum);
					repeatNum++;
					if (repeatNum < _repeatNum)
						frame = 0;
					else
					{
						frame = imgs->count - 1;// frame--;
						animation = false;
						if (method)
							method(parm);
					}
				}
			}
		}
	}

	float r, g, b, a;
	getRGBA(r, g, b, a);
	setRGBA(r, g, b, a * alpha);

	tex = (Texture*)imgs->objectAtIndex(frame);
	//drawImage(tex, position + p, TOP | LEFT);
	p += position;

	p.x += (1.0f - scale) / 2 * tex->width;
	p.y += (1.0f - scale) / 2 * tex->height;

	drawImage(tex, p.x, p.y, scale, scale, TOP|LEFT,
		0, 0, tex->width, tex->height,
		2, degree, reverse);
	setRGBA(r, g, b, a);
}

void iImage::addObject(Texture* tex)
{
	if (this->tex == NULL)
		this->tex = tex;
	imgs->addObject(tex);
	tex->retainCount++;
}

void iImage::startAnimation(cbAnimation m, void* p)
{
	animation = true;
	frame = 0;
	aniDt = 0.0f;
	method = m;
	parm = p;

	if (_repeatNum)
		repeatNum = 0;
}
void iImage::stopAnimation()
{
	animation = false;
}

iRect iImage::rect(iPoint p, iSize s)
{
	return iRectMake(
		position.x + p.x - s.width / 2,
		position.y + p.y - s.height / 2,
		tex->width + s.width,
		tex->height + s.height);
}

iPoint iImage::center(iPoint p)
{
	iRect rt = rect(p);
	return iPointMake(	rt.origin.x + rt.size.width / 2,
						rt.origin.y + rt.size.height / 2);
}

iImage* iImage::clone()
{
	iImage* img = new iImage();

	memcpy(img, this, sizeof(iImage));

	img->imgs = new iArray(cb);
	for (int i = 0; i < imgs->count; i++)
		img->addObject( (Texture*)imgs->objectAtIndex(i) );

	return img;
}
