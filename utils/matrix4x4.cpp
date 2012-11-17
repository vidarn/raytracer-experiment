#include "matrix4x4.h"

Matrix4x4::Matrix4x4()
{
	setIdentity();
}

Matrix4x4::Matrix4x4(std::ifstream &stream)
{
    for(int i=0; i<16; i++){
        stream.read( reinterpret_cast<char*>( &(m_entries[i]) ), sizeof m_entries[i] );
    }
}

Matrix4x4::Matrix4x4(float entries[4][4])
{
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			m_entries[x+y*4] = entries[x][y];
		}
	}
}

Matrix4x4::Matrix4x4(float entries[16])
{
	for (int i = 0; i < 16; i++) {
		m_entries[i] = entries[i];
	}
}

void Matrix4x4::setTranslation(float x, float y, float z)
{
	setIdentity();
	m_entries[3] = x;
	m_entries[7] = y;
	m_entries[11] = z;
}

void Matrix4x4::setScaling(float x, float y, float z)
{
	setIdentity();
	m_entries[0] = x;
	m_entries[5] = y;
	m_entries[10] = z;
}

void Matrix4x4::setRotation(int axis, float angle)
{
	setIdentity();
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);
    switch(axis){
        case 0:
            m_entries[1+4] = cosAngle;
            m_entries[2+4] = -sinAngle;
            m_entries[1+8] = sinAngle;
            m_entries[2+8] = cosAngle;
            break;
        case 1:
            m_entries[0] = cosAngle;
            m_entries[2] = sinAngle;
            m_entries[8] = -sinAngle;
            m_entries[2+8] = cosAngle;
            break;
        case 2:
            m_entries[0] = cosAngle;
            m_entries[1] = -sinAngle;
            m_entries[4] = sinAngle;
            m_entries[4+1] = cosAngle;
            break;
        default:
            //raise error ;)
            assert(false);
    }
}

void Matrix4x4::setIdentity()
{
	for(int y=0;y<4;y++){
		for(int x=0;x<4;x++){
			m_entries[x+y*4] = (x==y);
		}
	}
}

Matrix4x4 Matrix4x4::invert()
{
	Matrix4x4 tmpMat;
    for (int y = 0; y < 4; y++){
        for (int x = 0; x < 4; x++){
            tmpMat.m_entries[x+y*4] = m_entries[x+y*4];
        }
    }
	Matrix4x4 invMat;
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < y; x++) {
			float mult = tmpMat.m_entries[x+y*4];
			invMat.subMultRow(x,y,mult);
			tmpMat.subMultRow(x,y,mult);
		}
		float diagVal = tmpMat.m_entries[y+y*4];
		invMat.multRow(y,1.0/diagVal);
		tmpMat.multRow(y,1.0/diagVal);
	}
	// we should now have ones in the diagonal and zeroes int the lower left half
	// let's fix the upper right half
	for (int y = 2; y >= 0; y--) {
		for (int x = 3; x > y; x--) {
			float mult = tmpMat.m_entries[x+y*4];
			tmpMat.subMultRow(x,y,mult);
			invMat.subMultRow(x,y,mult);
		}
	}
	return invMat;
}

void Matrix4x4::multRow(int row, float mult)
{
	for(int x = 0; x < 4; x++){
		m_entries[x+row*4] *= mult;
	}
}

void Matrix4x4::subMultRow(int srcRow,int destRow, float mult)
{
	for(int x = 0; x < 4; x++){
		m_entries[x+destRow*4] -= mult*m_entries[x+srcRow*4];
	}
}

Matrix4x4 Matrix4x4::operator*(Matrix4x4 &other)
{
	Matrix4x4 result;
	for(int y=0;y<4;y++){
		for(int x=0;x<4;x++){
			float val = 0;
			for (int i = 0; i < 4; i++) {
				val += m_entries[x+i*4]*other.m_entries[i+y*4];
			}
			result.m_entries[x+y*4] = val;
		}
	}
	return result;
}

Vec3 Matrix4x4::multVec3(Vec3 &vec)
{
	Vec3 result;
	for(int y=0;y<3;y++){
		float val = 0;
		for(int x=0;x<3;x++){
			val += m_entries[x+y*4]*vec[x];
		}
		result[y] = val;
	}
	return result;
}

Vec3 Matrix4x4::multPoint(Vec3 &point)
{
	Vec3 result = multVec3(point);
	for(int y=0;y<3;y++){
		result[y] += m_entries[3+y*4];
	}
	return result;
}

std::ostream& operator<<(std::ostream &out, Matrix4x4 &mat)
{
	out << "M";
	for(int y=0;y<4;y++){
		if(y>0){
			out << " ";
		}
		out << "[";
		for(int x=0;x<4;x++){
			if(x>0){
				out << ", ";
			}
			out << mat.m_entries[x+y*4];
		}
		out << "]" << std::endl;
	}
	return out;
}
