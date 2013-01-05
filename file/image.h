#ifndef __IMAGE_H_
#define __IMAGE_H_
#include <Magick++.h>
#include "../utils/rgba.h"

class Image
{
	public:
		Image();
		Image(const char *filename);
		RGBA getPixel(float x, float y) const;
	private:
		float m_invMax;
		float m_width;
		float m_height;
		const Magick::PixelPacket *m_pixels;
		Magick::Image m_image;
};

#endif /* end of include guard: __IMAGE_H_ */
