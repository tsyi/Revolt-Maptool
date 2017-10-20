#pragma once
#include "cTransform.h"


enum eOBJECT_TAG
{
	E_OBJECT_NONE,
	E_OBJECT_CAR,
	E_OBJECT_MAP,
	E_OBJECT_LIGHT,
	E_OBJECT_STUFF,
	E_OBJECT_CAMERA,
	E_OBJECT_END,
};

class NxActor;
class cMesh;

class cObject : public cTransform
{
protected :
	eOBJECT_TAG m_objTag;

	cObject();
	~cObject();
	cMesh* m_pMesh;
	NxActor* m_pActor;
	bool m_isActor;
public:

};

