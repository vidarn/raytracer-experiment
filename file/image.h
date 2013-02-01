#ifndef __IMAGE_H_
#define __IMAGE_H_
#include "../utils/rgba.h"
#include <OpenImageIO/imagecache.h>

OIIO_NAMESPACE_USING;

typedef std::map<int, ustring> ImageMap;

class ImageHandler
{
	public:
		ImageHandler();
		~ImageHandler();
        int loadImage(const char *filename);
		RGBA getPixel(int image, float x, float y);
	private:
		ImageCache *m_cache;
        ImageMap m_images;
        int m_numImages;
};

#endif /* end of include guard: __IMAGE_H_ */
