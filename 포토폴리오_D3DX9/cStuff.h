#pragma once
#include "cObject.h"

class cStuff : public cObject
{
public:
	cStuff();
	~cStuff();

	void Update()
	{
//		SetMeshBox();
		cObject::Update();
	}
	void Render()
	{
		cObject::Render();
	}
};