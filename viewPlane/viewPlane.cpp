#include "viewPlane.h"
#include <cstring>

ViewPlane::ViewPlane(int resX, int resY, double sizeX, double sizeY)
{
    m_resolution[0] = resX;
    m_resolution[1] = resY;
    if(resY == 0){
        m_resolution[1] = m_resolution[0];
    }
    m_size[0] = sizeX;
    m_size[1] = sizeY;
    m_pixels.reserve(m_resolution[0]*m_resolution[1]);
    m_pixels.resize(m_resolution[0]*m_resolution[1]);
}

Ray ViewPlane::getPixelRay(int index, Point sample)
{
    double x = index % m_resolution[0];
    double y = index / m_resolution[0];
    x += sample[0];
    y += sample[1];
    double posX = -1.0 + 2.0*((double) x)/((double) m_resolution[0]);
    double posY = -1.0 + 2.0*((double) y)/((double) m_resolution[1]);
    posX *= m_size[0];
    posY *= m_size[1];
    Point origin(posX,posY,0.0);
    Vec3 direction(0.0,0.0,1.0);
    Ray ray(origin,direction);
    return ray;
}

void ViewPlane::setPixelValue(int index, RGBA color)
{
    m_pixels[index] = color;
}

void ViewPlane::saveToTiff(const char *filename)
{
    TIFF* out = TIFFOpen(filename, "w");
    int sampleperpixel = 4;
    char *image=new char [m_resolution[0]*m_resolution[1]*sampleperpixel];
    for(int i = 0;i<m_resolution[0]*m_resolution[1];i++){
        RGBA pixel = m_pixels[i];
        image[i*sampleperpixel] = pixel.r()*255;
        image[i*sampleperpixel+1] = pixel.g()*255;
        image[i*sampleperpixel+2] = pixel.b()*255;
        image[i*sampleperpixel+3] = pixel.a()*255;
    }
    TIFFSetField(out, TIFFTAG_IMAGEWIDTH, m_resolution[0]);  
    TIFFSetField(out, TIFFTAG_IMAGELENGTH, m_resolution[1]);    
    TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, sampleperpixel);   
    TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 8);    
    TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);    
    TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    tsize_t linebytes = sampleperpixel * m_resolution[0];
    unsigned char *buf = NULL;        
    if (TIFFScanlineSize(out) < linebytes)
        buf =(unsigned char *)_TIFFmalloc(linebytes);
    else
        buf = (unsigned char *)_TIFFmalloc(TIFFScanlineSize(out));

    TIFFSetField(out, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(out, m_resolution[0]*sampleperpixel));

    for (uint32 row = 0; row < m_resolution[1]; row++)
    {
        memcpy(buf, &image[(m_resolution[1]-row-1)*linebytes], linebytes);    
        if (TIFFWriteScanline(out, buf, row, 0) < 0)
        break;
    }

    TIFFClose(out);
}

std::ostream& operator<<(std::ostream &out, ViewPlane &vp)
{
    for(int y=0; y< vp.m_resolution[1];y++){
        for(int x=0; x< vp.m_resolution[0];x++){
            RGBA color = vp.m_pixels[x+y*vp.m_resolution[0]];
            if(color.r() < 0.25){
                out << "  ";
            }
            if(color.r() >= 0.25 && color.r() < 0.5){
                out << ".'";
            }
            if(color.r() >= 0.5 && color.r() < 0.75){
                out << "//";
            }
            if(color.r() >= 0.75){
                out << "XX";
            }
        }
        out << std::endl;
    }
	return out;
}
