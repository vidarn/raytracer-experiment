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
}
