#include "rgba.h"

void
clamp(RGBA &col)
{
    for(int i=0;i<4;i++){
        col[i] = std::max(0.0f,col[i]);
        col[i] = std::min(1.0f,col[i]);
    }
}
