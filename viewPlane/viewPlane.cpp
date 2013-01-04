#include "viewPlane.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

ViewPlane::ViewPlane(const char *filename)
{
    m_mutexes = NULL;
	m_filename = filename;
}

ViewPlane::ViewPlane()
{
}

void ViewPlane::setResolution(int resX, int resY)
{
    m_resolution[0] = resX;
    m_resolution[1] = resY;
    if(resY == 0){
        m_resolution[1] = m_resolution[0];
    }
    m_pixels.reserve(m_resolution[0]*m_resolution[1]);
    m_pixels.resize(m_resolution[0]*m_resolution[1]);
    m_numSamples.reserve(m_resolution[0]*m_resolution[1]);
    m_numSamples.resize(m_resolution[0]*m_resolution[1]);
    if(m_mutexes != NULL) 
        delete m_mutexes; 
    m_mutexes = new pthread_mutex_t[m_resolution[0]*m_resolution[1]]; 
    for(int i=0;i<m_resolution[0]*m_resolution[1];i++){
        m_numSamples[i] = 0;
        pthread_mutex_init(&(m_mutexes[i]),NULL); 
    }
	m_size[0] = 1.0f;
    m_size[1] = 1.0f*float(m_resolution[1])/float(m_resolution[0]);
}

Ray ViewPlane::getPixelRay(int x, int y, Sampling &sampling)
{
    Vec3 sample = sampling.getSquareSample(1);
    float posX = float(x) + sample[0];
    float posY = float(y) + sample[1];
    posX = -1.0 + 2.0*((float) posX)/((float) m_resolution[0]);
    posY = -1.0 + 2.0*((float) posY)/((float) m_resolution[1]);
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
		ray.m_origin += sampling.getDiskSample(0)*0.2f;
		ray.m_dir = p - ray.m_origin;
	}
}

void ViewPlane::setPixelValue(int x, int y, RGBA color)
{
    pthread_mutex_lock(&(m_mutexes[x + y*m_resolution[0]]));
    m_pixels[x + y*m_resolution[0]] += color;
    m_numSamples[x + y*m_resolution[0]]++;
    pthread_mutex_unlock(&(m_mutexes[x + y*m_resolution[0]]));
}

void ViewPlane::saveToTiff()
{
    int totalNumSamples = 0;
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
        if(m_numSamples[i] > 0){
            pixel *= 1.0f/float(m_numSamples[i]);
        }
        else{
            pixel = RGBA(0.0f,0.0f,0.0f,0.0f);
        }
        totalNumSamples += m_numSamples[i];
        pixel[0] = pow(pixel[0],1.0f/2.2f);
        pixel[1] = pow(pixel[1],1.0f/2.2f);
        pixel[2] = pow(pixel[2],1.0f/2.2f);
		pixel.clamp();
        image[i*sampleperpixel] = pixel.r()*255;
        image[i*sampleperpixel+1] = pixel.g()*255;
        image[i*sampleperpixel+2] = pixel.b()*255;
        image[i*sampleperpixel+3] = pixel.a()*255;
    }
    std::cout << "num samples: " << totalNumSamples << std::endl;
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
