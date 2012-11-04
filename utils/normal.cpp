#include "normal.h"

void Normal::operator+=(Normal &other)
{
	m_vec += other.m_vec;
}
