#pragma once
#include "cTransform.h"
class cBox : public cTransform
{
private:
	std::vector<D3DXVECTOR3> m_vecPos;
	
public:
	cBox();
	~cBox();


};

