#include "rgba.h"

std::ostream& operator<<(std::ostream &out, RGBA &rgba)
{
	out << "RGBA [" << rgba.r() << " , " << rgba.g() << " , " << rgba.b() << " , " << rgba.a() << "]";
	return out;
}
