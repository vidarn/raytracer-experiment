#include "viewPlane.h"

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

Ray ViewPlane::getPixelRay(int index)
{
    int x = index % m_resolution[0];
    int y = index / m_resolution[0];
    double posX = -1.0 + 2.0*((double) x)/((double) m_resolution[0]);
    double posY = -1.0 + 2.0*((double) y)/((double) m_resolution[1]);
    Point origin(posX,posY,0.0);
    Vec3 direction(0.0,0.0,1.0);
    Ray ray(origin,direction);
    return ray;
}

void ViewPlane::setPixelValue(int index, RGBA color)
{
    m_pixels[index] = color;
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
