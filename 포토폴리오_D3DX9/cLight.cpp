#include "stdafx.h"
#include "cLight.h"


cLight::cLight()
{
	SetIsSelectColor(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
	SetUnSelectColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}


cLight::~cLight()
{
}
