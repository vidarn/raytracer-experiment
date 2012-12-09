#include "viewPlane.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

ViewPlane::ViewPlane(int resX, int resY, float sizeX, float sizeY, const char *filename)
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
    RGBA defaultPixel(0.0f,0.0f,0.0f,1.0f);
    for(int i=0;i<m_resolution[0]*m_resolution[1];i++){
        m_pixels[i] = defaultPixel;
    }
	m_filename = filename;
}

Ray ViewPlane::getPixelRay(int index, Sampling &sampling)
{
    float x = index % m_resolution[0];
    float y = index / m_resolution[0];
    Vec3 sample = sampling.getDiskSample(0);
    x += sample[0];
    y += sample[1];
    float posX = -1.0 + 2.0*((float) x)/((float) m_resolution[0]);
    float posY = -1.0 + 2.0*((float) y)/((float) m_resolution[1]);
    posX *= m_size[0];
    posY *= m_size[1];
    Vec3 origin(posX,posY,0.0);
    Vec3 direction;
	direction = origin - m_origin;
	direction.normalize();
    Ray ray(origin,direction,false);
	ray.m_origin[0] = 0.0f;
	ray.m_origin[1] = 0.0f;
	getDofRay(ray, sampling);
	ray.computePlucker();
    return ray;
}

void ViewPlane::getDofRay(Ray &ray, Sampling &sampling)
{
	if(m_focusDist > 0.0f){
		Vec3 p = ray.getPointAtPos(m_focusDist);
		ray.m_origin += sampling.getDiskSample(0)*0.5f;
		ray.m_dir = p - ray.m_origin;
	}
}

void ViewPlane::setPixelValue(int index, RGBA color)
{
    m_pixels[index] = color;
}

void ViewPlane::saveToTiff()
{
    struct flock fl;
    fl.l_type   = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start  = 0;
    fl.l_len    = 0;
    fl.l_pid    = getpid();
    int fd = open(m_filename,O_WRONLY);
    fcntl(fd, F_SETLKW, &fl);
    TIFF* out = TIFFOpen(m_filename, "w");
    int sampleperpixel = 4;
    char *image=new char [m_resolution[0]*m_resolution[1]*sampleperpixel];
    for(int i = 0;i<m_resolution[0]*m_resolution[1];i++){
        RGBA pixel = m_pixels[i];
		pixel.clamp();
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
    fl.l_type   = F_UNLCK;
    fcntl(fd, F_SETLK, &fl);
    close(fd);
}

void ViewPlane::setFov(float fov)
{
	m_origin[0] = 0.0f;
	m_origin[1] = 0.0f;
	m_origin[2] = m_size[0]/tan(fov*0.5f);
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
