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
    m_imageOutput = ImageOutput::create(m_filename);
}

ViewPlane::ViewPlane()
{
}

void ViewPlane::setResolution(int resX, int resY)
{
    m_resolution[0] = resX*2;
    m_resolution[1] = resY*2;
    if(resY == 0){
        m_resolution[1] = m_resolution[0];
    }
	m_pixels = new RGBA[m_resolution[0] * m_resolution[1]];
	memset(m_pixels,0,sizeof(RGBA) * m_resolution[0] * m_resolution[1]);
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
	int res[2] = {m_resolution[0]/2, m_resolution[1]/2};
    int channels = 4;
    m_imageSpec = ImageSpec(res[0],res[1], channels, TypeDesc::UINT8);
}

Ray ViewPlane::getPixelRay(int x, int y, Sampler &sampler)
{
    Vec3 sample = sampler.getSquareSample();
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
	getDofRay(ray, sampler);
	ray.computePlucker();
    return ray;
}

void ViewPlane::getDofRay(Ray &ray, Sampler &sampler)
{
	if(m_focusDist > 0.0f){
		Vec3 p = ray.getPointAtPos(m_focusDist);
		ray.m_origin += sampler.getDiskSample()*0.2f;
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

void ViewPlane::saveToImage()
{
    int totalNumSamples = 0;
    int channels = 4;
	int res[2] = {m_resolution[0]/2, m_resolution[1]/2};
    char *pixels= new char [res[0]*channels];
    if(!m_imageOutput)
        std::cerr << "ERROR: Could not open image \"" << m_filename << "\" for writing" << std::endl;
    else{
        struct flock fl;
        fl.l_type = F_WRLCK;
        fl.l_whence = SEEK_SET;
        fl.l_start = 0;
        fl.l_len = 0;
        fl.l_pid = getpid();
        int fd = open(m_filename,O_WRONLY);
        fcntl(fd,F_SETLK, &fl);
        m_imageOutput->open(m_filename, m_imageSpec);
        for(int imageY = 0; imageY<res[1]; imageY++){
            for(int imageX = 0; imageX<res[0]; imageX++){
                RGBA pixel(0);
                float subPixels = 0;
                for(int y = 0; y<2; y++){
                    for(int x = 0; x<2; x++){
                        int a = (m_resolution[1] - 1 -((imageY)*2+y))*m_resolution[0] + imageX*2+x;
                        RGBA tmpPixel = m_pixels[a];
                        if(m_numSamples[a] > 0){
                            tmpPixel *= 1.0f/float(m_numSamples[a]);
                            subPixels += 1.0f;
                        }
                        else{
                            tmpPixel = RGBA(0.0f,0.0f,0.0f,0.0f);
                        }
						clamp(tmpPixel);
                        pixel += tmpPixel;
                        totalNumSamples += m_numSamples[a];
                    }
                }
                if(subPixels > 0.0f)
                    pixel *= 1.0f/subPixels;
                pixel[0] = pow(pixel[0],1.0f/2.2f);
                pixel[1] = pow(pixel[1],1.0f/2.2f);
                pixel[2] = pow(pixel[2],1.0f/2.2f);
				clamp(pixel);
                pixels[imageX*channels]   = pixel.r*255;
                pixels[imageX*channels+1] = pixel.g*255;
                pixels[imageX*channels+2] = pixel.b*255;
                pixels[imageX*channels+3] = pixel.a*255;
            }
            m_imageOutput->write_scanline(imageY, 0, TypeDesc::UINT8, pixels);
        }
        std::cout << "num samples: " << totalNumSamples << std::endl;
        m_imageOutput->close();
        fl.l_type = F_UNLCK;
        fcntl(fd,F_SETLK,&fl);
        close(fd);
    }
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
            if(color.r < 0.25){
                out << "  ";
            }
            if(color.r >= 0.25 && color.r < 0.5){
                out << ".'";
            }
            if(color.r >= 0.5 && color.r < 0.75){
                out << "//";
            }
            if(color.r >= 0.75){
                out << "XX";
            }
        }
        out << std::endl;
    }
	return out;
}
