#include "image.h"
#include <iostream>

using namespace OIIO;

ImageHandler::ImageHandler()
{
	m_cache = ImageCache::create();
	m_cache->attribute("max_memory_MB", 500.0);
	m_cache->attribute("autotile", 64);
	m_cache->attribute("forcefloat", 1);
    m_numImages = 0;
}

ImageHandler::~ImageHandler()
{
	ImageCache::destroy(m_cache);
}

int ImageHandler::loadImage(const char *filename)
{
    int id = m_numImages++;
    std::cout << "loading image " << filename << std::endl;
    m_images.insert(std::make_pair(id,ustring(filename)));
    return id;
}

RGBA ImageHandler::getPixel(int image, float x, float y)
{
    ustring &filename = m_images[image];
	RGBA ret;
	int channels = 4;
	float pixels[channels];
	ImageSpec spec;
	bool ok = m_cache->get_imagespec(filename, spec);
	if(ok){
        if(x<0.0f)
            x = 1.0f - fmodf(-x,1.0f);
        else
            x = fmodf(x,1.0f);
        if(y<0.0f)
            y = 1.0f - fmodf(-y,1.0f);
        else
            y = fmodf(y,1.0f);
		int ix = x * spec.width;
		int iy = (1.0f-y) * spec.height;
		ok = m_cache->get_pixels(filename, 0, 0, ix, ix+1, iy, iy+1, 0, 1, TypeDesc::FLOAT, pixels);
        if(ok)
            ret = RGBA(pixels[0], pixels[1], pixels[2], pixels[3]);
        else
            std::cout << geterror() << " " << filename <<std::endl;
	}
    else{
        std::cout << geterror() << " " << filename <<std::endl;
    }
	return ret;
}

