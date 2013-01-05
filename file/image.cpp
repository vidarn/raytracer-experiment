#include "image.h"
#include <iostream>

Image::Image()
{
	m_width = 0;
	m_height = 0;
	m_pixels = 0;
}

Image::Image(const char *filename)
{
	m_image.read(filename);
	m_width  = m_image.columns();
	m_height = m_image.rows();
	m_pixels = m_image.getConstPixels(0,0,m_width, m_height);
	using namespace Magick;
	m_invMax = 1.0f/float(MaxRGB);
}

RGBA Image::getPixel(float x, float y) const
{
	RGBA ret;
	if(m_width > 0 && m_height > 0){
		int ix = x * m_width;
		int iy = y * m_height;
		ix = ix % int(m_width);
		if(ix < 0)
			ix += m_width;
		iy = m_height - iy;
		iy = iy % int(m_height);
		if(iy < 0)
			iy += m_height;
		iy *= m_width;
		const Magick::PixelPacket *p = m_pixels + ix + iy;
		ret = RGBA(p->red*m_invMax, p->green*m_invMax, p->blue*m_invMax, p->opacity*m_invMax);
	}
	return ret;
}
