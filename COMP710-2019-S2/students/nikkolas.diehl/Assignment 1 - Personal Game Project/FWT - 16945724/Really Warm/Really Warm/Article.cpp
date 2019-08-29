#include "Article.h"

Article::Article()
	:mo_f_Size(0)
{
}
Article::~Article()
{
	delete this;
}

void 
Article::SetCenter(float x, float y)
{
	m_x = x;
	m_y = y;
}

float
Article::GetSize() {
	return mo_f_Size;
}
