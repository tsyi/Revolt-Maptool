#pragma once
#include "cObject.h"

const std::string BaseFolder("Object/Maps");

class cMap : public cObject
{
private:

public:
	cMap();
	~cMap();

	void SetupPhyX();
	void Destroy();
	void Update();
	void Render();
	
};